#include <boost/dll/shared_library.hpp>
#include <cstdlib>
#include <functional>
#include <string>
#include <string_view>
#include <utility>
#include "cpp2util.h"

namespace cpp2::meta {

//  Load metafunction using Boost.DLL
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

        auto lib = boost::dll::shared_library(lib_path, boost::dll::load_mode::append_decorations);
        if (lib.has(cpp1_name))
        {
            return [
                fun = lib.get_alias<void(type_declaration&)>(cpp1_name),
                lib = std::move(lib)
                ](type_declaration& t)
            {
                fun(t);
            };
        }
    }

    return {};
}

}
