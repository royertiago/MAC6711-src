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
    std::unique_ptr<node> * search( std::unique_ptr<node> & tree, int key ) {
        if( !tree ) // key is not in the tree.
            return &tree;
        if( key < tree->key )
            return search(tree->lchild, key);
        if( tree->key < key )
            return search(tree->rchild, key);
        return &tree; // key is here.
    }
}

#endif // TREAP_HPP
