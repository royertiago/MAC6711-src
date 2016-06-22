namespace command_line {
    const char help_message[] =
" <data structure> <test case> [options]\n"
"Runs a speed test for the given data structure using the selected test case.\n"
"<data structure> must be one of\n"
"    avl - AVL self-balancing tree\n"
"    rb - std::set red-black self-balancing tree\n"
"    treap, treap-mersenne - Treap using Mersenne Twister as RNG\n"
"    treap-xorshift - Treap using xorshift as RNG\n"
"\n"
"<test case> must be one of\n"
"    insert-then-search\n"
"    ascending-insert-then-search\n"
"    insert-then-remove-then-search\n"
"    mixed-workload\n"
"\n"
"Options:\n"
"--show\n"
"    Show the resulting test case instead of running it.\n"
"\n"
"--runs <N>\n"
"    Number of times the test case must be run.\n"
"    Default: 10\n"
"\n"
"--seed <N>\n"
"    Chooses the seed used to generate the test set.\n"
"    Default: 0\n"
"\n"
"--treap-seed <N>\n"
"    Choose the seed used by the treap RNG.\n"
"    Default: 1\n"
"\n"
"--total-insertions <N>\n"
"    Total number of insertions that will be done in the tree.\n"
"    Default: 1 000 000\n"
"\n"
"--initial-insertions <N>\n"
"    Number of insertions done in the mixed workload before mixing operations.\n"
"    Default: 500 000\n"
"\n"
"--search-successes <N>\n"
"    Total number of search operations with keys known to be in the tree.\n"
"    Default: 800 000\n"
"\n"
"--search-failures <N>\n"
"    Total number of search operations with keys known not to be in the tree.\n"
"    Default: 400 000\n"
"\n"
"--removals <N>\n"
"    Total number of keys that will be removed from the tree.\n"
"    Default: 500 000\n"
"\n"
"--help\n"
"    Display this text and exit.\n"
;
} // namespace command_line

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <set>

#include "cmdline/args.hpp"

#include "avl.hpp"
#include "speed_test.hpp"
#include "treap.hpp"
#include "xorshift.hpp"

namespace command_line {
    int (* run_test_case)( const test_case & );
    test_case (* make_test_case)();
    int runs = 10;
    unsigned seed = 0;
    unsigned treap_seed = 1; // xorshift's seed must not be zero.
    int total_insertions = 1'000'000;
    int initial_insertions = 500'000;
    int search_successes = 800'000;
    int search_failures = 400'000;
    int removals = 500'000;
    bool show = false;

    void parse( cmdline::args && args ) {
        while( args.size() > 0 ) {
            std::string arg = args.next();
            if( arg == "avl" ) {
                run_test_case = []( const test_case & c ){
                    return ::run_test_case([](){ return avl::avl(); }, c );
                };
                continue;
            }
            if( arg == "rb" ) {
                run_test_case = []( const test_case & c ){
                    return ::run_test_case([](){ return std::set<int>(); }, c );
                };
                continue;
            }
            if( arg == "treap" || arg == "treap-mersenne" ) {
                run_test_case = []( const test_case & c ){
                    auto maker = [](){
                        return treap::treap<std::mt19937>{std::mt19937{treap_seed}};
                    };
                    return ::run_test_case( maker, c );
                };
                continue;
            }
            if( arg == "treap-xorshift" ) {
                run_test_case = []( const test_case & c ){
                    auto maker = [](){
                        return treap::treap<xorshift>{xorshift{treap_seed}};
                    };
                    return ::run_test_case( maker, c );
                };
                continue;
            }

            if( arg == "insert-then-search" ) {
                make_test_case = [](){
                    return insert_then_search( total_insertions, search_successes,
                                                search_failures, seed );
                };
                continue;
            }
            if( arg == "ascending-insert-then-search" ) {
                make_test_case = [](){
                    return ascending_insert_then_search(
                            total_insertions, search_successes,
                            search_failures, seed );
                };
                continue;
            }
            if( arg == "insert-then-remove-then-search" ) {
                make_test_case = [](){
                    return insert_then_remove_then_search( total_insertions,
                            removals, search_successes, search_failures, seed );
                };
                continue;
            }
            if( arg == "mixed-workload" ) {
                make_test_case = [](){
                    return mixed_workload( initial_insertions, total_insertions,
                            removals, search_successes, search_failures, seed );
                };
                continue;
            }

            if( arg == "--show" ) {
                show = true;
                continue;
            }
            if( arg == "--runs" ) {
                args.range(1) >> runs;
                continue;
            }
            if( arg == "--seed" ) {
                args >> seed;
                continue;
            }
            if( arg == "--treap-seed" ) {
                args >> treap_seed;
                continue;
            }
            if( arg == "--total-insertions" ) {
                args.range(1) >> total_insertions;
                continue;
            }
            if( arg == "--initial-insertions" ) {
                args.range(1) >> initial_insertions;
                continue;
            }
            if( arg == "--search-successes" ) {
                args.range(0) >> search_successes;
                continue;
            }
            if( arg == "--search-failures" ) {
                args.range(0) >> search_failures;
                continue;
            }
            if( arg == "--removals" ) {
                args.range(0) >> removals;
                continue;
            }
            if( arg == "--help" ) {
                std::cout << args.program_name() << help_message;
                std::exit(0);
            }

            std::cerr << args.program_name() << ": Unknown option " << arg << '\n';
            std::exit(1);
        }
    }
}

int main( int argc, char ** argv ) {
    command_line::parse( cmdline::args(argc, argv) );
    test_case c = command_line::make_test_case();

    if( command_line::show ) {
        for( operation & op : c ) {
            switch( op.type ) {
                case operation_type::insert:
                    std::cout << "Insert " << op.key << '\n';
                    break;
                case operation_type::erase:
                    std::cout << "Erase  " << op.key << '\n';
                    break;
                case operation_type::count:
                    std::cout << "Count  " << op.key << '\n';
                    break;
            }
        }
        return 0;
    }

    std::cout << "Test case prepared.\n";
    for( int i = 1; i <= command_line::runs; i++ ) {
        std::cout << "Run:" << std::setw(3) << i << " - Time: "
            << command_line::run_test_case(c) << "ms\n";
    }

    return 0;
}
