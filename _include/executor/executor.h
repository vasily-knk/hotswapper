#pragma once

#include "common/functions.h"

struct executor
{
    virtual ~executor() {}

    virtual bool do_something(uint32_t src, uint32_t *dst) = 0;

    typedef ::functions_cptr functions_cptr;
};

typedef shared_ptr<executor> executor_ptr;

