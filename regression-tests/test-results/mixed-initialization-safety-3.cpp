

//=== Cpp2 type declarations ====================================================


#include "cpp2util.h"

#line 1 "mixed-initialization-safety-3.cpp2"


//=== Cpp2 type definitions and function declarations ===========================

#line 1 "mixed-initialization-safety-3.cpp2"
#include <random>
#include <string>
#include <vector>

#line 5 "mixed-initialization-safety-3.cpp2"
[[nodiscard]] auto main() -> int;

#line 16 "mixed-initialization-safety-3.cpp2"
auto fill(
    cpp2::out<std::string> x, 
    cpp2::in<std::string> value, 
    cpp2::in<int> count
    ) -> void;

#line 26 "mixed-initialization-safety-3.cpp2"
auto print_decorated(auto const& x) -> void;

// for test determinism, force "xyzzy" branch
// the standard mandates that std::mt19937()() == 3499211612
[[nodiscard]] auto flip_a_coin() -> bool;

//=== Cpp2 function definitions =================================================

#line 1 "mixed-initialization-safety-3.cpp2"

#line 5 "mixed-initialization-safety-3.cpp2"
[[nodiscard]] auto main() -> int{
    cpp2::deferred_init<std::string> x; // note: uninitialized!

    if (flip_a_coin()) {
        x.construct("xyzzy");
    }else {
        fill(cpp2::out(&x), "plugh", 40);// note: constructs x!
    }
    print_decorated(std::move(x.value()));
}

auto fill(
    cpp2::out<std::string> x, 
    cpp2::in<std::string> value, 
    cpp2::in<int> count
    ) -> void

{
    cpp2::Default.expects(cpp2::cmp_greater_eq(CPP2_UFCS(ssize)(value),count), "fill: value must contain at least count elements");
#line 23 "mixed-initialization-safety-3.cpp2"
    x.construct(CPP2_UFCS(substr)(value, 0, count));
}

auto print_decorated(auto const& x) -> void { std::cout << ">> [" << x << "]\n";  }

#line 30 "mixed-initialization-safety-3.cpp2"
[[nodiscard]] auto flip_a_coin() -> bool { return std::mt19937()() % 2 == 0;  }

