#include "stdafx.h"
#include "defs.h"
#include "common/lib_wrapper.h"

functions_cptr functions_instance()
{
    static lib_wrapper lib("library.dll");

    static functions_t f;
    f.do_something = lib.get_function<functions_t::do_something_t>("do_something");

    return &f;
}
