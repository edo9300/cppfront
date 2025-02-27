

//=== Cpp2 type declarations ====================================================


#include "cpp2util.h"

#line 1 "mixed-initialization-safety-3-contract-violation.cpp2"


//=== Cpp2 type definitions and function declarations ===========================

#line 1 "mixed-initialization-safety-3-contract-violation.cpp2"
#include <random>
#include <string>
#include <vector>

#line 5 "mixed-initialization-safety-3-contract-violation.cpp2"
[[nodiscard]] auto main() -> int;

#line 18 "mixed-initialization-safety-3-contract-violation.cpp2"
auto fill(
    cpp2::out<std::string> x, 
    cpp2::in<std::string> value, 
    cpp2::in<int> count
    ) -> void;

#line 28 "mixed-initialization-safety-3-contract-violation.cpp2"
auto print_decorated(auto const& x) -> void;

// for test determinism, force "fill" branch
bool flip_a_coin() {
    // Change std::mt19937 to std::random_device for non-deterministic PRNG
    static std::mt19937 rand;
    return rand() % 2 == 1;
}


//=== Cpp2 function definitions =================================================

#line 1 "mixed-initialization-safety-3-contract-violation.cpp2"

#line 5 "mixed-initialization-safety-3-contract-violation.cpp2"
[[nodiscard]] auto main() -> int{
    std::set_terminate(std::abort);

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
#line 25 "mixed-initialization-safety-3-contract-violation.cpp2"
    x.construct(CPP2_UFCS(substr)(value, 0, count));
}

auto print_decorated(auto const& x) -> void { std::cout << ">> [" << x << "]\n";  }

