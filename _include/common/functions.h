#pragma once

struct functions_t
{
    typedef bool (__cdecl *do_something_t)(uint32_t, uint32_t *);

    do_something_t do_something;
};

typedef functions_t const* functions_cptr;