#include "stdafx.h"
#include "executor_manager.h"
#include "common/lib_wrapper.h"
#include "defs.h"

namespace
{
    optional<std::time_t> ready_time() 
    {
        fs::path ready_path("ready.txt");
        
        if (fs::is_regular_file(ready_path))
            return fs::last_write_time(ready_path);

        return none;
    }

    struct executor_lib_t
    {
        explicit executor_lib_t(char const *name, functions_cptr functions)
            : lib_(name)
            , executor_(create_executor(functions))
        {}

        executor_ptr get_executor() const
        {
            return executor_;
        }

        void shutdown()
        {
            executor_.reset();
        }

    private:
        executor_ptr create_executor(functions_cptr functions) const
        {
            auto f_create = lib_.get_function<create_executor_f>("create_executor");
            auto f_delete = lib_.get_function<delete_executor_f>("delete_executor");

            executor *e = f_create(functions);
            return executor_ptr(e, f_delete);
        }

    private:
        lib_wrapper lib_;
        executor_ptr executor_;
    };

} // namespace

struct executor_manager_impl
    : executor_manager
{
    executor_manager_impl()
        : time_to_die_(false)
        , copy_time_(ready_time())
    {
        {
            boost::unique_lock<boost::shared_mutex> lock(mutex_);
            init_lib();
        }

        watch_thread_ = boost::thread(std::bind(&executor_manager_impl::watch_thread_fn, this));
    }

    ~executor_manager_impl()
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
   
    void copy_executor_lib()
    {
        fs::path src_lib = "executor.dll";
        fs::path dst_lib = "_executor.dll";

        fs::path src_pdb = "executor.pdb";
        fs::path dst_pdb = "_executor.pdb";

        fs::copy_file(src_lib, dst_lib, fs::copy_option::overwrite_if_exists);

        if (fs::is_regular_file(src_pdb))
            fs::copy_file(src_pdb, dst_pdb, fs::copy_option::overwrite_if_exists);
    }
    
    void init_lib()
    {
        cout << "Copying lib" << endl;
        copy_executor_lib();
        lib_ = make_unique<executor_lib_t>("_executor.dll", functions_instance());
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

private:
    unique_ptr<executor_lib_t> lib_;
    boost::shared_mutex mutex_; 

    boost::thread watch_thread_;
    boost::atomic<bool> time_to_die_;

    optional<std::time_t> copy_time_;
};

executor_manager_ptr executor_manager_instance()
{
    static auto em = make_shared<executor_manager_impl>();
    return em;
}
