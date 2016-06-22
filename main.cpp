#include <iostream>
#include <set>
#include "avl.hpp"
#include "speed_test.hpp"
#include "treap.hpp"
#include "xorshift.hpp"

int main() {
    test_case c = insert_then_search( 1'000'000, 800'000, 400'000, 0 );

    auto avl_maker = [](){ return avl::avl(); };
    auto treap_maker = [](){ return treap::treap<std::mt19937>{std::mt19937{}}; };
    auto xorshift_treap_maker = [](){ return treap::treap<xorshift>{xorshift{}}; };
    auto set_maker = [](){ return std::set<int>(); };

    std::cout << "1 million elements, 800k successful searches, 400k failures\n";

    std::cout << "\nAVL:\n";
    for( int i = 0; i < 10; i++ )
        std::cout << run_test_case(avl_maker, c) << "ms\n";

    std::cout << "\nTreap (Mersenne Twister):\n";
    for( int i = 0; i < 10; i++ )
        std::cout << run_test_case(treap_maker, c) << "ms\n";

    std::cout << "\nTreap (xorshift):\n";
    for( int i = 0; i < 10; i++ )
        std::cout << run_test_case(treap_maker, c) << "ms\n";

    std::cout << "\nstd::set:\n";
    for( int i = 0; i < 10; i++ )
        std::cout << run_test_case(set_maker, c) << "ms\n";

    return 0;
}
