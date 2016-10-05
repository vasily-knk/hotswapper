#include "stdafx.h"
#include "common/lib_wrapper.h"

int main()
{
    lib_wrapper lib(L"proxy.dll");

    typedef bool (__cdecl *do_something_t)(uint32_t src, uint32_t *dst);

    auto f = lib.get_function<do_something_t>("do_something");
    
    uint32_t dst;
    bool result = f(5, &dst);

    return 0;
}