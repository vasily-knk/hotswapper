#pragma once

namespace executor_manager
{

namespace internal
{
    struct parameters_base_t
    {
        string dll_name;
        string pdb_name;
        string dll_copy_name;
        string pdb_copy_name;
        string ready_file_name;
    };
    
    template<typename functions_cptr>
    struct parameters_generic_t
        : parameters_base_t
    {
        parameters_generic_t() = default;
    
        template<typename rhs_functions_cptr>
        parameters_generic_t(parameters_generic_t<rhs_functions_cptr> const &rhs)
            : parameters_base_t(rhs)
            , functions(rhs.functions)
        {
        }    

        template<typename rhs_functions_cptr>
        parameters_generic_t(parameters_generic_t<rhs_functions_cptr> &&rhs)
            : parameters_base_t(std::move(rhs))
            , functions(std::move(rhs.functions))
        {
        }    

        functions_cptr functions = nullptr;
    };

    typedef parameters_generic_t<void const *> parameters_t;

    struct manager
    {
        virtual ~manager() {}
                        
        virtual shared_ptr<void> get_executor() = 0;
    };
    
    typedef shared_ptr<manager> manager_ptr;
    manager_ptr create_manager(parameters_t const &parameters);

} // namespace internal
    

template<typename Executor>    
struct manager
{
    typedef Executor executor_type;
    typedef typename executor_type::functions_cptr functions_cptr;
    typedef internal::parameters_generic_t<functions_cptr> parameters_t;

    explicit manager(parameters_t const &parameters)
        : internal_(internal::create_manager(parameters))
    {}

    shared_ptr<Executor> get_executor() const
    {
        auto raw = internal_->get_executor();
        return std::static_pointer_cast<Executor>(raw);
    }
    
private:
    internal::manager_ptr internal_;
};

} // namespace executor_manager