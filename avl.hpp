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

        /* Recompute the attribute h from the lchild and rchild nodes.
         */
        void update_height() {
            int l = -1, r = -1;
            if( lchild ) l = lchild->h;
            if( rchild ) r = rchild->h;
            h = std::max(l, r) + 1;
        }
    };

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
        ptr->lchild->update_height();
        ptr->update_height();
    }

    /* Performs a right rotation.
     * Node heights are adjusted accordingly.
     * ptr->lchild is assumed to be non-null.
     */
    inline void rotate_right( std::unique_ptr<node> & ptr ) {
        circular_shift_unique_ptr(ptr, ptr->lchild, ptr->lchild->rchild);
        ptr->rchild->update_height();
        ptr->update_height();
    }
}
#endif // AVL_HPP
