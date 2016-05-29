#include <cassert>
#include "treap.hpp"

// Simple assert-based test.
void test() {
    { // Rotation test
        constexpr int A = 1, B = 2, alpha = 3, beta = 4, gamma = 5;
        auto tree =
            std::make_unique<treap::node>(A, 0,
                std::make_unique<treap::node>(alpha, 0),
                std::make_unique<treap::node>(B, 0,
                    std::make_unique<treap::node>(beta, 0),
                    std::make_unique<treap::node>(gamma, 0)));
        treap::rotate_left(tree);
        assert(tree->key == B);
        assert(tree->lchild->key == A);
        assert(tree->rchild->key == gamma);
        assert(tree->lchild->lchild->key == alpha);
        assert(tree->lchild->rchild->key == beta);

        treap::rotate_right(tree);
        assert(tree->key == A);
        assert(tree->lchild->key == alpha);
        assert(tree->rchild->key == B);
        assert(tree->rchild->lchild->key == beta);
        assert(tree->rchild->rchild->key == gamma);
    }
    { // Rotation with null pointers
        constexpr int A = 1, B = 2;
        auto tree = std::make_unique<treap::node>(A, 0,
                std::make_unique<treap::node>(B, 0),
                nullptr);
        treap::rotate_right(tree);
        assert(tree->key == B);
        assert(tree->rchild->key == A);
        treap::rotate_left(tree);
        assert(tree->key == A);
        assert(tree->lchild->key == B);
    }
    { // search
        auto tree = std::make_unique<treap::node>(5, 0,
                std::make_unique<treap::node>(3, 0),
                std::make_unique<treap::node>(8, 0));
        assert( treap::search(tree, 5) == &tree );
        assert( treap::search(tree, 3) == &(tree->lchild) );
        assert( treap::search(tree, 8) == &(tree->rchild) );
        assert( treap::search(tree, 2) == &(tree->lchild->lchild) );
        assert( treap::search(tree, 4) == &(tree->lchild->rchild) );
        assert( treap::search(tree, 6) == &(tree->rchild->lchild) );
        assert( treap::search(tree, 9) == &(tree->rchild->rchild) );
    }
    { // insert and remove
        auto tree = std::make_unique<treap::node>(5, 80,
                std::make_unique<treap::node>(2, 50),
                std::make_unique<treap::node>(9, 20));
        treap::insert( tree, 4, 40 );
        assert( tree->key == 5 );
        assert( tree->lchild->key == 2 );
        assert( tree->lchild->rchild->key == 4 );
        treap::insert( tree, 3, 70 );
        assert( tree->key == 5 );
        assert( tree->lchild->key == 3 );
        assert( tree->lchild->lchild->key == 2 );
        assert( tree->lchild->rchild->key == 4 );
        treap::insert( tree, 6, 90 );
        assert( tree->key == 6 );
        assert( tree->lchild->key == 5 );
        assert( tree->rchild->key == 9 );
        treap::insert( tree, 7, 10 );
        assert( tree->key == 6 );
        assert( tree->rchild->key == 9 );
        assert( tree->rchild->lchild->key == 7 );
        treap::insert( tree, 8, 60 );
        assert( tree->key == 6 );
        assert( tree->rchild->key == 8 );
        assert( tree->rchild->lchild->key == 7 );
        assert( tree->rchild->rchild->key == 9 );
        treap::remove( tree, 8 );
        assert( tree->key == 6 );
        assert( tree->rchild->key == 9 );
        assert( tree->rchild->lchild->key == 7 );
        treap::remove( tree, 7 );
        assert( tree->key == 6 );
        assert( tree->lchild->key == 5 );
        assert( tree->rchild->key == 9 );
        treap::remove( tree, 4 );
        treap::remove( tree, 3 );
        treap::remove( tree, 6 );
        assert( tree->key == 5 );
        assert( tree->lchild->key == 2 );
        assert( tree->rchild->key == 9 );
        treap::remove( tree, 5 );
        assert( tree->key == 2 );
        assert( tree->rchild->key == 9 );
        treap::remove( tree, 5 );
        assert( tree->key == 2 );
        assert( tree->rchild->key == 9 );
    }
}

int main() {
    test();
    return 0;
}
