#include <functional>
#include <string>
#include "cpp2util.h"

namespace cpp2::meta {

auto load_metafunction(std::string const&)
    -> std::function<void(type_declaration&)>
{
    return {};
}

}
