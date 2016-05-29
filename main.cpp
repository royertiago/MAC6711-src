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
}

int main() {
    test();
    return 0;
}
