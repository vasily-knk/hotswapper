#include "stdafx.h"
#include "common/lib_wrapper.h"
#include "common/functions.h"
#include "executor/executor.h"
#include "defs.h"
#include "executor_manager.h"


namespace
{

executor_ptr executor_instance()
{
    return executor_manager_instance()->get_executor();
}

}

bool do_something(uint32_t src, uint32_t *dst)
{
    auto executor = executor_instance();
    return executor->do_something(src, dst);
}
