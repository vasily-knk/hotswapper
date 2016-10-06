#include "stdafx.h"
#include "defs.h"
#include "common/lib_wrapper.h"

#include <boost/filesystem.hpp>

namespace
{
    struct executor_lib_t
    {
        explicit executor_lib_t(char const *name)
            : lib(name)
            , create_executor(lib.get_function<create_executor_f>("create_executor"))
            , delete_executor(lib.get_function<delete_executor_f>("delete_executor"))
        {}
        
        const lib_wrapper lib;
        const create_executor_f create_executor;
        const delete_executor_f delete_executor;
    };

    void copy_executor_lib(unique_ptr<executor_lib_t> &p)
    {
        p.reset();
        
        namespace fs = boost::filesystem;

        fs::path src_lib = "executor.dll";
        fs::path dst_lib = "_executor.dll";

        fs::path src_pdb = "executor.pdb";
        fs::path dst_pdb = "_executor.pdb";

        fs::copy_file(src_lib, dst_lib, fs::copy_option::overwrite_if_exists);

        if (fs::is_regular_file(src_pdb))
            fs::copy_file(src_pdb, dst_pdb, fs::copy_option::overwrite_if_exists);

        p = make_unique<executor_lib_t>(dst_lib.string().c_str());
    }

    executor_ptr create_executor(functions_cptr functions)
    {
        static unique_ptr<executor_lib_t> executor_lib;

        copy_executor_lib(executor_lib);

        executor *e = executor_lib->create_executor(functions);
        return executor_ptr(e, executor_lib->delete_executor);
    };

} // namespace

shared_ptr<executor> executor_instance()
{
    static auto e = create_executor(functions_instance());
    return e;
}
