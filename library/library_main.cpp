#include <cstdint>

bool do_something(int32_t src, int32_t *dst)
{
    *dst = src * 2;
    return true;
}
