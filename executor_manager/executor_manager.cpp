#include "stdafx.h"
#include "executor_manager/executor_manager.h"
#include "executor_lib.h"

namespace executor_manager
{

struct manager_impl
    : manager
{
    explicit manager_impl(parameters_t const &parameters)
        : parameters_(parameters)
        , time_to_die_(false)
        , copy_time_(ready_time())
    {
        {
            boost::unique_lock<boost::shared_mutex> lock(mutex_);
            init_lib();
        }

        watch_thread_ = boost::thread(std::bind(&manager_impl::watch_thread_fn, this));
    }

    ~manager_impl()
    {
        time_to_die_ = true;
        watch_thread_.join();
    }

    executor_ptr get_executor() override
    {
        boost::shared_lock<boost::shared_mutex> lock(mutex_);
        return lib_->get_executor();
    }

private:
    void init_lib()
    {
        cout << "Copying lib" << endl;
        
        fs::path src_lib = parameters_.dll_name; 
        fs::path dst_lib = parameters_.dll_copy_name; 

        fs::path src_pdb = parameters_.pdb_name;
        fs::path dst_pdb = parameters_.pdb_copy_name;

        fs::copy_file(src_lib, dst_lib, fs::copy_option::overwrite_if_exists);

        if (fs::is_regular_file(src_pdb))
            fs::copy_file(src_pdb, dst_pdb, fs::copy_option::overwrite_if_exists);

        lib_ = make_unique<executor_lib_t>(parameters_.dll_copy_name.c_str(), parameters_.functions);
    }

    optional<std::time_t> need_update() const
    {
        if (auto t = ready_time())
            if (!copy_time_ || *t > *copy_time_)
                return *t;

        return none;
    }

    void watch_thread_fn() 
    {
        while(!time_to_die_)
        {
            boost::this_thread::sleep_for(boost::chrono::seconds(2));

            if (auto t = need_update())
            {
                copy_time_ = *t;

                boost::unique_lock<boost::shared_mutex> lock(mutex_);

                weak_ptr<executor> w = lib_->get_executor();
                lib_->shutdown();
                while(!w.expired())
                    boost::this_thread::sleep_for(boost::chrono::seconds(1));

                lib_.reset();
                init_lib();
            }
        }

        cout << "Good bye!" << endl;
    }

    optional<std::time_t> ready_time() const
    {
        fs::path ready_path(parameters_.ready_file_name);
        
        if (fs::is_regular_file(ready_path))
            return fs::last_write_time(ready_path);

        return none;
    }
        
private:
    parameters_t const parameters_;

    unique_ptr<executor_lib_t> lib_;
    boost::shared_mutex mutex_; 

    boost::thread watch_thread_;
    boost::atomic<bool> time_to_die_;

    optional<std::time_t> copy_time_;
};

manager_ptr create(parameters_t const &parameters)
{
    static auto g_manager = make_shared<manager_impl>(parameters);
    return g_manager;
}

} // namespace executor_manager
          