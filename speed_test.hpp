#ifndef SPEED_TEST_HPP
#define SPEED_TEST_HPP

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

enum operation_type {
    insert,
    erase,
    count,
};

struct operation {
    operation_type type;
    int key;
};

typedef std::vector<operation> test_case;

/* A test case is simply a list of operations that must be performed by the trees.
 * This header contains tools to generate varied test cases,
 * and to run them with trees.
 *
 * Since the test cases are randomly generated,
 * the used seed is as a parameter.
 * The random number generator is fixed as std::mt19937.
 */

/* Runs the test case, constructing a new tree every time using the functor 'maker'.
 * Both construction and destruction times are timed.
 * 'runs' is the number of times the same test case is executed.
 * Each new run means a call to 'maker'.
 */
template< typename TreeMaker >
int run_test_case( TreeMaker maker, const test_case & test ) {
    int counter = 0;
    auto begin = std::chrono::steady_clock::now();
    {
        auto tree = maker();
        for( const operation & op : test ) {
            switch( op.type ) {
                case operation_type::insert:
                    tree.insert(op.key);
                    break;
                case operation_type::erase:
                    tree.erase(op.key);
                    break;
                case operation_type::count:
                    counter += tree.count(op.key); // To avoid compiler optimizations
                    break;
            }
        }
    }
    auto end = std::chrono::steady_clock::now();

    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    return ms + (counter == 0);
}

/* Returns a random vector with exactly 'zeros' values set to 0
 * and exacly 'ones' values set to 1.
 * (I've choosen to use unsigned char instead of bool
 * to avoid the std::vector<bool> specialization.)
 */
std::vector<unsigned char> random_bits( int zeros, int ones, std::mt19937 & rng ) {
    std::vector<unsigned char> ret( zeros + ones );
    for( int i = 0; i < ones; i++ )
        ret[i]++;
    std::shuffle( ret.begin(), ret.end(), rng );
    return ret;
}

/* Returns a test case which is a sequence of 'values' insertions,
 * and then a random mix of 'search_successes' searches for values actually inserted
 * and 'search_failures' searches for values never inserted.
 */
test_case insert_then_search(
    int values,
    int search_successes,
    int search_failures,
    unsigned int seed
) {
    std::mt19937 rng(seed);
    test_case ret( values + search_successes + search_failures );

    /* Simple trick to guarantee success/failure:
     * the values inserted will all be even,
     * and the search failures will all be odd.
     *
     * To guarantee no value is inserted twice,
     * we will simply insert all value from 2 to 2*values.
     * and shuffle the operation.
     */
    for( int i = 0; i < values; i++ )
        ret[i] = operation{ operation_type::insert, 2 * i + 2 };

    std::shuffle( ret.begin(), ret.begin() + values, rng );

    std::uniform_int_distribution<> success(1, values);
    std::uniform_int_distribution<> failure(0, values);

    auto bits = random_bits(search_failures, search_successes, rng);
    for( int i = 0; i < search_successes + search_failures; i++ )
        if( bits[i] )
            ret[i + values] = operation{ operation_type::count, 2*success(rng) };
        else
            ret[i + values] = operation{ operation_type::count, 2*failure(rng) + 1};

    return ret;
}

#endif // SPEED_TEST_HPP
