#ifndef TREAP_HPP
#define TREAP_HPP

#include <tuple>
#include <memory>

namespace treap {
    struct node {
        int key;
        int priority;
        std::unique_ptr<node> lchild, rchild;

        node() = default;
        node( int k, int p ) : key(k), priority(p) {}
        node( int k, int p,
                std::unique_ptr<node>&& lchild, std::unique_ptr<node>&& rchild ) :
            key(k), priority(p), lchild(std::move(lchild)), rchild(std::move(rchild))
        {}
    };

    /* Assigns ptr2 to ptr1, ptr3 to ptr2, and ptr1 to ptr3,
     * without destroying any object.
     */
    void circular_shift_unique_ptr( std::unique_ptr<node> & ptr1,
            std::unique_ptr<node> & ptr2, std::unique_ptr<node> & ptr3 )
    {
        ptr1.swap(ptr2);
        ptr2.swap(ptr3);
    }

    /* Performs a left rotation.
     * n.rchild is assumed to be non-null.
     */
    void rotate_left( std::unique_ptr<node> & ptr ) {
        circular_shift_unique_ptr(ptr, ptr->rchild, ptr->rchild->lchild);
    }

    /* Performs a right rotation.
     * n.lchild is assumed to be non-null.
     */
    void rotate_right( std::unique_ptr<node> & ptr ) {
        circular_shift_unique_ptr(ptr, ptr->lchild, ptr->lchild->rchild);
    }

    /* Returns a pointer to the unique_ptr holding a tree
     * whose root has the requested key,
     * or a pointer to the place in the tree the key would be inserted
     * if it is not in the tree.
     */
    std::unique_ptr<node> & search( std::unique_ptr<node> & tree, int key ) {
        if( !tree ) // key is not in the tree.
            return tree;
        if( key < tree->key )
            return search(tree->lchild, key);
        if( tree->key < key )
            return search(tree->rchild, key);
        return tree; // key is here.
    }

    /* Inserts a node with the specified key and priority in the treap.
     * If the key already exists, the treap is not modified.
     */
    void insert( std::unique_ptr<node> & tree, int key, int priority ) {
        if( !tree ) {
            tree = std::make_unique<node>(key, priority);
            return;
        }
        if( key < tree->key ) {
            insert( tree->lchild, key, priority );
            if( tree->lchild->priority > tree->priority )
                rotate_right(tree);
        }
        if( tree->key < key ) {
            insert( tree->rchild, key, priority );
            if( tree->rchild->priority > tree->priority )
                rotate_left(tree);
        }
    }

    /* Delete the root of the given treap.
     * The tree is assumed to be non-null.
     */
    void root_delete( std::unique_ptr<node> & tree ) {
        if( !tree->lchild )
            tree = std::move(tree->rchild);
        else if( !tree->rchild )
            tree = std::move(tree->lchild);
        else if( tree->lchild->priority < tree->rchild->priority ) {
            rotate_left(tree);
            root_delete(tree->lchild);
        }
        else {
            rotate_right(tree);
            root_delete(tree->rchild);
        }
    }

    /* Erases the given key from the tree.
     */
    void remove( std::unique_ptr<node> & tree, int key ) {
        auto & ptr = search(tree, key);
        if( ptr ) // ptr is always non null; it points to another pointer
            root_delete( ptr );
    }
}

#endif // TREAP_HPP
