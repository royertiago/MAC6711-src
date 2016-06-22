#ifndef AVL_HPP
#define AVL_HPP

#include <memory>
#include <algorithm>

namespace avl {
    /* C-like structure representing an AVL tree node.
     * To have a std::set-like interface, see the avl class below.
     */
    struct node {
        int key;
        int h;
        std::unique_ptr<node> rchild, lchild;

        node() = default;
        node( int k ) : key(k) {}
        node( int k, std::unique_ptr<node>&& lchild, std::unique_ptr<node>&& rchild ) :
            key(k), lchild(std::move(lchild)), rchild(std::move(rchild))
        {}

    };

    /* Returns the height of the given node.
     * If the node is a null pointer, -1 is returned.
     */
    int height( const std::unique_ptr<node> & ptr ) {
        return ptr ? ptr->h : -1;
    }

    /* Recompute and set the height attribute from the lchild and rchild nodes.
     */
    void update_height( std::unique_ptr<node> & ptr ) {
        ptr->h = std::max( height(ptr->lchild), height(ptr->rchild) ) + 1;
    }

    /* Assigns ptr2 to ptr1, ptr3 to ptr2, and ptr1 to ptr3,
     * without destroying any object.
     */
    inline void circular_shift_unique_ptr( std::unique_ptr<node> & ptr1,
            std::unique_ptr<node> & ptr2, std::unique_ptr<node> & ptr3 )
    {
        ptr1.swap(ptr2);
        ptr2.swap(ptr3);
    }

    /* Performs a left rotation.
     * Node heights are adjusted accordingly.
     * ptr->rchild is assumed to be non-null.
     */
    inline void rotate_left( std::unique_ptr<node> & ptr ) {
        circular_shift_unique_ptr(ptr, ptr->rchild, ptr->rchild->lchild);
        update_height( ptr->lchild );
        update_height( ptr );
    }

    /* Performs a right rotation.
     * Node heights are adjusted accordingly.
     * ptr->lchild is assumed to be non-null.
     */
    inline void rotate_right( std::unique_ptr<node> & ptr ) {
        circular_shift_unique_ptr(ptr, ptr->lchild, ptr->lchild->rchild);
        update_height( ptr->rchild );
        update_height( ptr );
    }

    /* Make the tree rooted at ptr an AVL tree,
     * provided that both ptr->lchild and ptr->rchild are AVL trees
     * whose height differ by at most two.
     */
    inline void fix_avl( std::unique_ptr<node> & ptr ) {
        if( height(ptr->lchild) < height(ptr->rchild) - 1 ) {
            // There is too much weight in the right.
            if( height(ptr->rchild->lchild) > height(ptr->rchild->rchild) )
                rotate_right( ptr->rchild );
            rotate_left( ptr );
        }
        else if( height(ptr->rchild) < height(ptr->lchild) - 1 ) {
            // Mirrorred situation.
            if( height(ptr->lchild->rchild) > height(ptr->lchild->lchild) )
                rotate_left( ptr->lchild );
            rotate_right( ptr );
        }
        else
            update_height( ptr );
    }

    /* Inserts the given key in the given tree
     * and adjust it so that it continues to be an AVL tree.
     * tree->h is increased by at most one.
     */
    inline void insert( std::unique_ptr<node> & tree, int key ) {
        if( !tree )
            tree = std::make_unique<node>(key);
        else if( key < tree->key )
            insert( tree->lchild, key );
        else if( tree->key < key )
            insert( tree->rchild, key );

        fix_avl(tree);
    }
}
#endif // AVL_HPP
