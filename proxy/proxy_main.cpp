#include "stdafx.h"
#include "common/lib_wrapper.h"
#include "common/functions.h"
#include "executor/executor.h"


namespace
{
    functions_cptr functions_instance()
    {
        static lib_wrapper lib("library.dll");

        static functions_t f;
        f.do_something = lib.get_function<functions_t::do_something_t>("do_something");

        return &f;
    }


    shared_ptr<executor> executor_instance()
    {
        auto create_executor = [](functions_cptr functions)
        {
            static lib_wrapper executor_lib("executor.dll");
        
            auto create_executor_impl = executor_lib.get_function<create_executor_f>("create_executor");
            auto delete_executor_impl = executor_lib.get_function<delete_executor_f>("delete_executor");

            executor *e = create_executor_impl(functions);
            return shared_ptr<executor>(e, delete_executor_impl);
        };
        
        static auto e = create_executor(functions_instance());
        return e;
    }
}

bool do_something(uint32_t src, uint32_t *dst)
{
    auto executor = executor_instance();
    return executor->do_something(src, dst);
}
