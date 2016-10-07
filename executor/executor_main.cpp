#include "stdafx.h"
#include "executor/executor.h"

namespace
{

    struct executor_impl
        : executor
    {
        explicit executor_impl(functions_cptr f)
            : f_(f)
        {
        }

        bool do_something(uint32_t src, uint32_t* dst) override
        {
            auto result = f_->do_something(src, dst);
            *dst = 21;

            return false;
        }

    private:
        functions_cptr f_;
    };
    
} // namespace


executor* create_executor(functions_cptr functions)
{
    return new executor_impl(functions);
}

void delete_executor(executor *e)
{
    delete e;
}