#pragma once

#include "common/functions.h"

struct executor
{
    virtual ~executor() {}

    virtual bool do_something(uint32_t src, uint32_t *dst) = 0;
};

typedef shared_ptr<executor> executor_ptr;

typedef executor* (__cdecl *create_executor_f)(functions_cptr functions);
typedef void (__cdecl *delete_executor_f)(executor *);