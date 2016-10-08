#include "stdafx.h"
#include "executor/executor.h"
#include "defs.h"
#include "executor_manager/executor_manager.h"


namespace
{

    typedef executor_manager::manager<executor> manager_t;

    executor_ptr executor_instance()
    {
        auto create_parameters = []()
        {
            manager_t::parameters_t parameters;
            parameters.functions = functions_instance();
            parameters.dll_name = "executor.dll";
            parameters.pdb_name = "executor.pdb";
            parameters.dll_copy_name = "_executor.dll";
            parameters.pdb_copy_name = "_executor.pdb";
            parameters.ready_file_name = "ready.txt";

            return parameters;
        };

        static manager_t man(create_parameters());
        return man.get_executor();
    }

} // namespace

bool do_something(uint32_t src, uint32_t *dst)
{
    auto executor = executor_instance();
    return executor->do_something(src, dst);
}
