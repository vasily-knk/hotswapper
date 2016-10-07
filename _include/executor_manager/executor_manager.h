#pragma once

#include "executor/executor.h"

namespace executor_manager
{

struct parameters_t
{
    functions_cptr functions = nullptr;
    string dll_name;
    string pdb_name;
    string dll_copy_name;
    string pdb_copy_name;
    string ready_file_name;
};

struct manager
{
    virtual ~manager() {}

    virtual executor_ptr get_executor() = 0;
};

typedef shared_ptr<manager> manager_ptr;

manager_ptr create(parameters_t const &parameters);

} // namespace executor_manager