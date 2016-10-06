#pragma once

struct lib_wrapper
{
    lib_wrapper(lib_wrapper const&) = delete;
    lib_wrapper &operator=(lib_wrapper const&) = delete;
    
    explicit lib_wrapper(wchar_t const *filename)
        : lib_(LoadLibraryW(filename))
    {}
    
    explicit lib_wrapper(char const *filename)
        : lib_(LoadLibraryA(filename))
    {}

    template<typename Func>
    Func get_function(char const *name)
    {
        return reinterpret_cast<Func>(GetProcAddress(lib_, name));
    }

    ~lib_wrapper()
    {
        if (lib_)
            FreeLibrary(lib_);
    }

    HMODULE lib() const
    {
        return lib_;
    }

    bool valid() const
    {
        return lib_ != nullptr;
    }
    
private:
    HMODULE lib_;
};