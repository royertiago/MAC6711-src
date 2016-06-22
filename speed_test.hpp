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

test_case ascending_insert_then_search(
    int values,
    int search_successes,
    int search_failures,
    unsigned int seed
) {
    std::mt19937 rng(seed);
    test_case ret( values + search_successes + search_failures );
    for( int i = 0; i < values; i++ )
        ret[i] = operation{ operation_type::insert, 2 * i + 2 };

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

/* Structure to efficiently pick a random number known to be in the tree.
 */
struct efficiently_choose_target_to_remove {
    std::vector<std::pair<int, bool>> keys;
    // The boolean tells whether the specified key was already choosen or not.
    int available_keys;

    int get_key( std::mt19937 & rng ) {
        std::uniform_int_distribution<> new_index(0, keys.size() - 1);
        int index = new_index(rng);
        while( !keys[index].second )
            index = new_index(rng);

        int key = keys[index].first;
        keys[index].second = false;
        available_keys--;

        // Resize the vector
        if( keys.size() > 2 * available_keys )
            keys.erase( std::remove_if( keys.begin(), keys.end(),
                        [](auto x){ return !x.second; }
                        ));

        return key;
    }

    int peek_key( std::mt19937 & rng ) {
        std::uniform_int_distribution<> new_index(0, keys.size() - 1);
        int index = new_index(rng);
        while( !keys[index].second )
            index = new_index(rng);
        return keys[index].first;
    }

    void push_key( int key ) {
        keys.push_back( std::make_pair(key, true) );
        available_keys++;
    }
};

test_case insert_then_remove_then_search(
    int insertions,
    int removals,
    int search_successes,
    int search_failures,
    unsigned int seed
) {
    std::mt19937 rng(seed);
    test_case ret( insertions + search_successes + search_failures + removals );
    auto rem = efficiently_choose_target_to_remove{
        std::vector<std::pair<int,bool>>(insertions),
        insertions
    };

    // Generate the insertions
    for( int i = 0; i < insertions; i++ ) {
        ret[i] = operation{ operation_type::insert, 2 * i + 2 };
        rem.keys[i] = std::make_pair( 2 * i + 2, true );
    }
    std::shuffle( ret.begin(), ret.begin() + insertions, rng );

    // Generate the removals
    for( int i = 0; i < removals; i++ )
        ret[i+insertions] = operation{ operation_type::erase, rem.get_key(rng) };

    // Generate the searches
    std::uniform_int_distribution<> success_index(0, rem.keys.size()-1);
    std::uniform_int_distribution<> failure(0, insertions);

    auto bits = random_bits(search_failures, search_successes, rng);
    for( int i = 0; i < search_successes + search_failures; i++ )
        if( bits[i] )
            ret[i + insertions+removals] =
                operation{ operation_type::count, rem.keys[success_index(rng)].first };
        else
            ret[i + insertions+removals] =
                operation{ operation_type::count, 2*failure(rng) + 1};

    return ret;
}

test_case mixed_workload(
    int initial_insertions,
    int total_insertions,
    int removals,
    int search_successes,
    int search_failures,
    unsigned int seed
) {
    std::mt19937 rng(seed);
    test_case ret( total_insertions + removals + search_successes + search_failures );
    auto rem = efficiently_choose_target_to_remove{
        std::vector<std::pair<int,bool>>(initial_insertions),
        initial_insertions
    };

    for( int i = 0; i < total_insertions; i++ )
        ret[i] = operation{ operation_type::insert, 2 * i + 2 };
    std::shuffle( ret.begin(), ret.begin() + total_insertions, rng );
    /* The first initial_insertions values will not be changed anymore.
     * The other values will be shuffled together with the other values.
     */

    for( int i = 0; i < initial_insertions; i++ )
        rem.keys[i] = std::make_pair( ret[i].key, true );
    // rem is usable.

    for( int i = 0; i < removals; i++ )
        ret[i+total_insertions].type = operation_type::erase;
    for( int i = 0; i < search_successes + search_failures; i++ )
        ret[i+total_insertions+removals].type = operation_type::count;
    std::shuffle( ret.begin()+initial_insertions, ret.end(), rng );
    /* All the operation types are correctly set,
     * and every insert operation has the correct key set.
     * Now, we will set the keys of the removals and the searches.
     */

    auto bits = random_bits(search_failures, search_successes, rng);
    int decision = 0;

    std::uniform_int_distribution<> failure(0, total_insertions);

    for( int i = initial_insertions; i < ret.size(); i++ ) {
        switch( ret[i].type ) {
            case operation_type::insert:
                rem.push_key( ret[i].key );
                break;
            case operation_type::erase:
                ret[i].key = rem.get_key(rng);
                break;
            case operation_type::count:
                if( bits[decision++] )
                    ret[i].key = rem.peek_key(rng);
                else
                    ret[i].key = 2*failure(rng) + 1;
                break;
        }
    }

    return ret;
}

#endif // SPEED_TEST_HPP
