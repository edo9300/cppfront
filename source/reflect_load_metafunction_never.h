#include <functional>
#include <string>
#include "cpp2util.h"

namespace cpp2::meta {

//  Protocol of cppfront for program-defined metafunctions
//
//  Cppfront lowers a metafunction named 'f' as the `extern "C"` symbol 'cpp2_metafunction_f'
//
//  The input to 'load_metafunction' is the id-expression in the meta-functions-list
//
//  `load_metafunction' looks up the metafunction in an implementation-defined way
//
//  The output of 'load_metafunction'
//  is a function that forwards to the looked up metafunction, if found,
//  and an empty function otherwise

//  Returns an empty function
auto load_metafunction(std::string const&)
    -> std::function<void(type_declaration&)>
{
    return {};
}

}
