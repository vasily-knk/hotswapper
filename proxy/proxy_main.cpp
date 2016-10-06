#include "stdafx.h"
#include "common/lib_wrapper.h"
#include "common/functions.h"
#include "executor/executor.h"
#include "defs.h"


namespace
{



}

bool do_something(uint32_t src, uint32_t *dst)
{
    auto executor = executor_instance();
    return executor->do_something(src, dst);
}
