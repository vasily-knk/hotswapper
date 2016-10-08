#pragma once

#include "executor/executor.h"
#include "common/lib_wrapper.h"

namespace executor_manager
{
    struct executor_lib_t
    {
        typedef shared_ptr<void> executor_ptr;
        typedef void const *functions_cptr;

        
        explicit executor_lib_t(char const *name, functions_cptr functions)
            : lib_(name)
            , executor_(create_executor(functions))
        {}

        executor_ptr get_executor() const
        {
            return executor_;
        }

        void shutdown()
        {
            executor_.reset();
        }

    private:
        executor_ptr create_executor(functions_cptr functions) const
        {
            typedef void* (__cdecl *create_executor_f)(functions_cptr functions);
            typedef void (__cdecl *delete_executor_f)(void *);
            
            auto f_create = lib_.get_function<create_executor_f>("create_executor");
            auto f_delete = lib_.get_function<delete_executor_f>("delete_executor");

            auto e = f_create(functions);
            return executor_ptr(e, f_delete);
        }

    private:
        lib_wrapper lib_;
        executor_ptr executor_;
    };
} // namespace executor_manager