#include <cstdlib>
#include <functional>
#include <string>
#include <string_view>
#include <utility>
#include "cpp2util.h"

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif // _MSC_VER

namespace cpp2::meta {

class dll
{
public:
    dll(std::string_view path)
    {
#ifdef _MSC_VER
        handle_ = static_cast<void*>(LoadLibraryA(path.data()));
#else
        handle_ = static_cast<void*>(dlopen(path.data(), RTLD_NOW|RTLD_LOCAL));
#endif // _MSC_VER
        // TODO: log if the dll could not be open?
    }

    ~dll() noexcept
    {
        if(handle_ == nullptr);
            return;
#ifdef _MSC_VER
        FreeLibrary(static_cast<HMODULE>(handle));
#else
        dlclose(handle_);
#endif // _MSC_VER
    }

    // Uncopyable
    dll(dll&) = delete;
    dll(dll const&) = delete;
    auto operator=(dll const&) -> dll& = delete;

    // Movable
    dll(dll&& from) noexcept
    {
        handle_ = from.handle_;
        from.handle_ = nullptr;
    }

    auto operator=(dll&& from) noexcept -> dll&
    {
        handle_ = from.handle_;
        from.handle_ = nullptr;
        return *this;
    }

    auto is_open() noexcept -> bool { return handle_ != nullptr; }

    template<typename T>
    auto get_alias(std::string_view name) noexcept -> T*
    {
        void* symbol = nullptr;
#ifdef _MSC_VER
        symbol = static_cast<void*>(GetProcAddress(static_cast<HMODULE>(handle_), name.data()));
#else
        symbol = dlsym(handle_, name.data());
        if(symbol == nullptr)
        {
            // Some platforms export with additional underscore, so try that.
            auto const us_name = std::string("_") + name.data();
            symbol = dlsym(handle_, us_name.c_str());
        }
#endif // _MSC_VER
        // TODO: log if the symbol is not found?
        return reinterpret_cast<T*>(symbol);
    }
private:
    void* handle_{nullptr};
};


//  Load metafunction by opening DLL with OS APIs
//
//  The ':'-separated library paths
//  are read from the environment variable
//  'CPPFRONT_METAFUNCTION_LIBRARIES'
auto load_metafunction(std::string const& name) -> std::function<void(type_declaration&)>
{
    auto cpp1_libraries_cstr = std::getenv("CPPFRONT_METAFUNCTION_LIBRARIES");
    if (!cpp1_libraries_cstr) {
        return {};
    }

    auto cpp1_libraries = std::string_view{cpp1_libraries_cstr};
    auto cpp1_name = "cpp2_metafunction_" + name;

    while (!cpp1_libraries.empty())
    {
        auto colon = cpp1_libraries.find(':');
        auto lib_path = cpp1_libraries.substr(0, colon);
        cpp1_libraries.remove_prefix(lib_path.size() + unsigned(colon != lib_path.npos));

        auto lib = std::make_shared<dll>(lib_path);
        if(!lib->is_open())
            continue;

        if (auto* fun = lib->get_alias<void(void*)>(cpp1_name); fun != nullptr)
        {
            return [
                fun = fun,
                lib = lib
                ](type_declaration& t)
            {
                fun(static_cast<void*>(&t));
            };
        }
    }

    return {};
}

}
