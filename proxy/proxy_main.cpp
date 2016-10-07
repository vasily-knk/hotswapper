#include "stdafx.h"
#include "executor/executor.h"
#include "defs.h"
#include "executor_manager/executor_manager.h"


namespace
{

    executor_ptr executor_instance()
    {
        auto create_executor = []()
        {
            executor_manager::parameters_t parameters;
            parameters.functions = functions_instance();
            parameters.dll_name = "executor.dll";
            parameters.pdb_name = "executor.pdb";
            parameters.dll_copy_name = "_executor.dll";
            parameters.pdb_copy_name = "_executor.pdb";

            return executor_manager::create(parameters);
        };
    
        static auto man = create_executor();
        return man->get_executor();
    }

} // namespace

bool do_something(uint32_t src, uint32_t *dst)
{
    auto executor = executor_instance();
    return executor->do_something(src, dst);
}
