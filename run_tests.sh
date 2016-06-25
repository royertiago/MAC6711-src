#!/bin/bash
trees="avl rb treap-mersenne treap-xorshift"
configurations=(
# Simply insertion
    "insert-then-search --total-insertions 100000 --search-successes 0 --search-failures 0"
    "insert-then-search --search-successes 0 --search-failures 0"
    "ascending-insert-then-search --total-insertions 100000 --search-successes 0 --search-failures 0"
    "ascending-insert-then-search --search-successes 0 --search-failures 0"

# Insertion then search
    "insert-then-search --total-insertions 100000"
    "insert-then-search"
    "ascending-insert-then-search --total-insertions 100000"
    "ascending-insert-then-search"

# Modification
    "insert-then-remove-then-search --total-insertions 100000 --initial-insertions 50000 --search-successes 80000 --search-failures 40000 --removals 50000"
    "insert-then-remove-then-search"
    "mixed-workload --total-insertions 100000 --initial-insertions 50000 --search-successes 80000 --search-failures 40000 --removals 50000"
    "mixed-workload"
)

for config in "${configurations[@]}"; do
    for tree in $trees; do
        echo $tree $config
        ./main $tree $config --runs 12
        echo
    done
done
