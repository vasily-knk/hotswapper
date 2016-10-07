#pragma once

#include "executor/executor.h"

struct executor_manager
{
    virtual ~executor_manager() {}

    virtual executor_ptr get_executor() = 0;
};

typedef shared_ptr<executor_manager> executor_manager_ptr;

executor_manager_ptr executor_manager_instance();
