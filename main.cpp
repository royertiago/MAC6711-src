#include <iostream>
#include <set>
#include "speed_test.hpp"
#include "avl.hpp"
#include "treap.hpp"

int main() {
    test_case c = insert_then_search( 1'000'000, 800'000, 400'000, 0 );

    auto avl_maker = [](){ return avl::avl(); };
    auto treap_maker = [](){ return treap::treap<std::mt19937>{std::mt19937{}}; };
    auto set_maker = [](){ return std::set<int>(); };

    std::cout << "1 million elements, 120k searches, 10 runs - AVL:\n";
    for( int i = 0; i < 10; i++ )
        std::cout << run_test_case(avl_maker, c) << "ms\n";
    std::cout << "1 million elements, 120k searches, 10 runs - Treap:\n";
    for( int i = 0; i < 10; i++ )
        std::cout << run_test_case(treap_maker, c) << "ms\n";
    std::cout << "1 million elements, 120k searches, 10 runs - std::set:\n";
    for( int i = 0; i < 10; i++ )
        std::cout << run_test_case(set_maker, c) << "ms\n";

    return 0;
}
