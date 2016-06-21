#include "avl.hpp"
#include <catch.hpp>

TEST_CASE( "AVL height and rotation", "[avl]" ) {
    auto tree = 
        std::make_unique<avl::node>( 10,
            std::make_unique<avl::node>( 8,
                nullptr,
                std::make_unique<avl::node>( 9, nullptr, nullptr )),
            std::make_unique<avl::node>( 15,
                std::make_unique<avl::node>( 12,
                    nullptr,
                    std::make_unique<avl::node>( 13, nullptr, nullptr )),
                std::make_unique<avl::node>( 20, nullptr, nullptr )));

    tree->lchild->rchild->update_height();
    tree->lchild->update_height();
    tree->rchild->lchild->rchild->update_height();
    tree->rchild->lchild->update_height();
    tree->rchild->rchild->update_height();
    tree->rchild->update_height();
    tree->update_height();

    SECTION( "Initial heights" ) {
        CHECK( tree->lchild->rchild->h == 0 );
        CHECK( tree->lchild->h == 1 );
        CHECK( tree->rchild->lchild->rchild->h == 0 );
        CHECK( tree->rchild->lchild->h == 1 );
        CHECK( tree->rchild->rchild->h == 0 );
        CHECK( tree->rchild->h == 2 );
        CHECK( tree->h == 3 );
    }

    SECTION( "After rotations" ) {
        avl::rotate_left( tree->lchild );
        CHECK( tree->lchild->key == 9 );
        REQUIRE( tree->lchild->lchild != nullptr );
        CHECK( tree->lchild->lchild->key == 8 );
        CHECK( tree->lchild->h == 1 );
        CHECK( tree->lchild->lchild->h == 0 );

        avl::rotate_right( tree->rchild );
        CHECK( tree->rchild->key == 12 );
        CHECK( tree->rchild->lchild == nullptr );
        REQUIRE( tree->rchild->rchild != nullptr );
        CHECK( tree->rchild->rchild->key == 15 );
        REQUIRE( tree->rchild->rchild->lchild != nullptr );
        CHECK( tree->rchild->rchild->lchild->key == 13 );
        CHECK( tree->rchild->h == 2 );
        CHECK( tree->rchild->rchild->h == 1 );

        avl::rotate_left( tree ); // Technically this is not an AVL tree anymore, but...
        CHECK( tree->key == 12 );
        CHECK( tree->h == 3 );
        CHECK( tree->lchild->h == 2 );
    }
}
