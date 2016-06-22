#include "avl.hpp"
#include <catch.hpp>

bool is_avl( const std::unique_ptr<avl::node> & tree ) {
    if( !tree )
        return true;
    if( std::abs(avl::height(tree->lchild) - avl::height(tree->rchild)) > 1 )
        return false;
    return is_avl( tree->lchild ) && is_avl( tree->rchild );
}

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

    update_height( tree->lchild->rchild );
    update_height( tree->lchild );
    update_height( tree->rchild->lchild->rchild );
    update_height( tree->rchild->lchild );
    update_height( tree->rchild->rchild );
    update_height( tree->rchild );
    update_height( tree );

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

TEST_CASE( "AVL insertion and invariant-keeping", "[avl]" ) {
    std::unique_ptr<avl::node> tree;
    CHECK( is_avl(tree) );
    avl::insert( tree, 10 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 20 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 30 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 40 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 50 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 60 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 70 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 49 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 48 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 47 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 46 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 45 );
    CHECK( is_avl(tree) );
    avl::insert( tree, 44 );
    CHECK( is_avl(tree) );

    avl::remove( tree, 30 );
    CHECK( is_avl(tree) );
    avl::remove( tree, 40 );
    CHECK( is_avl(tree) );
    avl::remove( tree, 44 );
    CHECK( is_avl(tree) );
    avl::remove( tree, 46 );
    CHECK( is_avl(tree) );
    avl::remove( tree, 49 );
    CHECK( is_avl(tree) );
    avl::remove( tree, 60 );
    CHECK( is_avl(tree) );
    avl::remove( tree, 20 );
    CHECK( is_avl(tree) );
    avl::remove( tree, 80 );
    CHECK( is_avl(tree) );
}

TEST_CASE( "AVL std::set-like interface", "[avl]" ) {
    avl::avl tree;
    CHECK( tree.count(5) == 0 );
    tree.insert( 1 );
    CHECK( tree.count(1) == 1 );
    tree.insert( 3 );
    tree.insert( 6 );
    tree.insert( 12 );
    tree.insert( 9 );
    tree.insert( 1 );
    CHECK( tree.count(3) == 1 );
    CHECK( tree.count(12) == 1 );
    CHECK( tree.count(9) == 1 );
    tree.erase( 3 );
    tree.erase( 12 );
    tree.insert( 3 );
    CHECK( tree.count(3) == 1 );
    CHECK( tree.count(12) == 0 );
}
