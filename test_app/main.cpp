#include "stdafx.h"
#include "common/lib_wrapper.h"

int main()
{
    lib_wrapper lib(L"proxy.dll");

    typedef bool (__cdecl *do_something_t)(uint32_t src, uint32_t *dst);

    auto f = lib.get_function<do_something_t>("do_something");
    
    uint32_t src = 1, dst;
    
    bool result = true;
    while(result)
    {
        result = f(src, &dst);

        src = dst;

        cout << dst << endl;
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
    }

    return 0;
}
