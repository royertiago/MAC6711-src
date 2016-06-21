#include "treap.hpp"
#include <catch.hpp>

TEST_CASE( "Treap rotation", "[treap]") {
    constexpr int A = 1, B = 2, alpha = 3, beta = 4, gamma = 5;
    auto tree =
        std::make_unique<treap::node>(A, 0,
            std::make_unique<treap::node>(alpha, 0),
            std::make_unique<treap::node>(B, 0,
                std::make_unique<treap::node>(beta, 0),
                std::make_unique<treap::node>(gamma, 0)));
    treap::rotate_left(tree);
    CHECK(tree->key == B);
    CHECK(tree->lchild->key == A);
    CHECK(tree->rchild->key == gamma);
    CHECK(tree->lchild->lchild->key == alpha);
    CHECK(tree->lchild->rchild->key == beta);

    treap::rotate_right(tree);
    CHECK(tree->key == A);
    CHECK(tree->lchild->key == alpha);
    CHECK(tree->rchild->key == B);
    CHECK(tree->rchild->lchild->key == beta);
    CHECK(tree->rchild->rchild->key == gamma);
}

TEST_CASE( "Treap rotation with null pointers", "[treap]" ) {
    constexpr int A = 1, B = 2;
    auto tree = std::make_unique<treap::node>(A, 0,
            std::make_unique<treap::node>(B, 0),
            nullptr);
    treap::rotate_right(tree);
    CHECK(tree->key == B);
    CHECK(tree->rchild->key == A);

    treap::rotate_left(tree);
    CHECK(tree->key == A);
    CHECK(tree->lchild->key == B);
}

TEST_CASE( "Treap search", "[treap]" ) {
    auto tree = std::make_unique<treap::node>(5, 0,
            std::make_unique<treap::node>(3, 0),
            std::make_unique<treap::node>(8, 0));

    CHECK( &treap::search(tree, 5) == &tree );
    CHECK( &treap::search(tree, 3) == &(tree->lchild) );
    CHECK( &treap::search(tree, 8) == &(tree->rchild) );
    CHECK( &treap::search(tree, 2) == &(tree->lchild->lchild) );
    CHECK( &treap::search(tree, 4) == &(tree->lchild->rchild) );
    CHECK( &treap::search(tree, 6) == &(tree->rchild->lchild) );
    CHECK( &treap::search(tree, 9) == &(tree->rchild->rchild) );
}

TEST_CASE( "Treap insert and remove", "[treap]" ) {
    auto tree = std::make_unique<treap::node>(5, 80,
            std::make_unique<treap::node>(2, 50),
            std::make_unique<treap::node>(9, 20));
    treap::insert( tree, 4, 40 );
    CHECK( tree->key == 5 );
    CHECK( tree->lchild->key == 2 );
    CHECK( tree->lchild->rchild->key == 4 );

    treap::insert( tree, 3, 70 );
    CHECK( tree->key == 5 );
    CHECK( tree->lchild->key == 3 );
    CHECK( tree->lchild->lchild->key == 2 );
    CHECK( tree->lchild->rchild->key == 4 );

    treap::insert( tree, 6, 90 );
    CHECK( tree->key == 6 );
    CHECK( tree->lchild->key == 5 );
    CHECK( tree->rchild->key == 9 );

    treap::insert( tree, 7, 10 );
    CHECK( tree->key == 6 );
    CHECK( tree->rchild->key == 9 );
    CHECK( tree->rchild->lchild->key == 7 );

    treap::insert( tree, 8, 60 );
    CHECK( tree->key == 6 );
    CHECK( tree->rchild->key == 8 );
    CHECK( tree->rchild->lchild->key == 7 );
    CHECK( tree->rchild->rchild->key == 9 );

    treap::remove( tree, 8 );
    CHECK( tree->key == 6 );
    CHECK( tree->rchild->key == 9 );
    CHECK( tree->rchild->lchild->key == 7 );

    treap::remove( tree, 7 );
    CHECK( tree->key == 6 );
    CHECK( tree->lchild->key == 5 );
    CHECK( tree->rchild->key == 9 );

    treap::remove( tree, 4 );
    treap::remove( tree, 3 );
    treap::remove( tree, 6 );
    CHECK( tree->key == 5 );
    CHECK( tree->lchild->key == 2 );
    CHECK( tree->rchild->key == 9 );

    treap::remove( tree, 5 );
    CHECK( tree->key == 2 );
    CHECK( tree->rchild->key == 9 );

    treap::remove( tree, 5 );
    CHECK( tree->key == 2 );
    CHECK( tree->rchild->key == 9 );
}
