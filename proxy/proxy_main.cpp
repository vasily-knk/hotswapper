#include "stdafx.h"
#include "common/lib_wrapper.h"

namespace
{
    lib_wrapper &get_library()
    {
        static lib_wrapper lib("library.dll");
        return lib;
    }

}

bool do_something(uint32_t src, uint32_t *dst)
{
    typedef bool (__cdecl *f_t)(uint32_t, uint32_t *);
    static f_t f = get_library().get_function<f_t>("do_something");

    bool result = f(src, dst);

    *dst *= 3;
    return result;
}
