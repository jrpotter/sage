/**
 * interval.h
 *
 * The Interval Tree is a Red-Black tree in which values are expected
 * to be able to be found within a given range. Thus any newly inserted
 * interval must be comparable (by default, '<').
 *
 * In particular, this is used when traversing the edges of a given DFA
 * and ensuring that the passed value falls within some interval. That is,
 * this works as a map but in which values are equivalent if they fall
 * in the key's range (endpoints inclusive).
 *
 * The following invariants of a red black tree are as follows:
 * 1) All nodes must be red or black.
 * 2) The root node is black.
 * 3) All leaf nodes are regarded black;
 * 3) A red node cannot have a red child.
 * 4) All root-leaf paths must have the same number of black nodes.
 *
 * Created by jrpotter (11/27/2015).
 */

#ifndef SAGE_INTERVAL_H
#define SAGE_INTERVAL_H

#include <functional>
#include <cassert>
#include <memory>

namespace sage
{
    template<typename K, typename V, typename C=std::less_equal<K>>
    class IntervalTree {

        struct Node;

        public:

            // Iterator
            // Performs an in-order traversal on the tree, reading
            // an end if no more nodes are left to be traversed
            class iterator
            {
                public:
                    iterator(std::weak_ptr<Node>);
                    iterator operator++();
                    iterator operator++(int);
                    const V operator*();
                    const bool operator==(const iterator&);
                    const bool operator!=(const iterator&);

                private:
                    std::weak_ptr<Node> current;
            };

            // Constructors
            IntervalTree();
            ~IntervalTree() = default;

            // Iterator Methods
            iterator begin();
            iterator end();

            // Default Operation
            void remove(K, K, C = C());
            void insert(K, K, V, C = C());
            iterator find(K, K, C = C());

        private:

            struct Node {

                // Data
                V value;
                bool red;
                std::pair<K, K> bounds;

                // This allows correct navigation throughout the tree
                K max_upper_bound;

                // Pointers
                std::weak_ptr<Node> parent;
                std::shared_ptr<Node> left;
                std::shared_ptr<Node> right;

                // Constructor
                Node(bool, K, K, V, std::weak_ptr<Node>);

                // Utility Methods
                void updateMaximum(C = C());
                std::weak_ptr<Node> successor();
            };

            // The root of the tree; always black
            std::shared_ptr<Node> root;

            // Utility methods to correct the invariants of the tree after the
            // corresponding insertion/removal method respectively
            void insert_fixup(std::shared_ptr<Node>);

            // Rotation Methods
            void llRotate(std::shared_ptr<Node>);
            void lrRotate(std::shared_ptr<Node>);
            void rrRotate(std::shared_ptr<Node>);
            void rlRotate(std::shared_ptr<Node>);
    };


    /**
     * Iterator Constructor
     * ================================
     */
    template<typename K, typename V, typename C>
    IntervalTree<K, V, C>::iterator::iterator(std::weak_ptr<Node> current)
        : current(current)
    { }


    /**
     * Iterator Infix Operations
     * ================================
     *
     * This searches the tree in sorted order (this is ultimately the
     * successor function).
     */
    template<typename K, typename V, typename C>
    typename IntervalTree<K, V, C>::iterator IntervalTree<K, V, C>::iterator::operator++()
    {
        if(auto cur = current.lock()) {
            current = cur->successor();
        }

        return *this;
    }

    template<typename K, typename V, typename C>
    typename IntervalTree<K, V, C>::iterator IntervalTree<K, V, C>::iterator::operator++(int)
    {
        iterator tmp(*this);
        if(auto cur = current.lock()) {
            current = cur->successor();
        }
        return tmp;
    }


    /**
     * Iterator Reference Operators
     * ================================
     */
    template<typename K, typename V, typename C>
    const V IntervalTree<K, V, C>::iterator::operator*()
    {
        if(auto cur = current.lock()) {
            return cur->value;
        }

        return V();
    }


    /**
     * Iterator Equality Operators
     * ================================
     */
    template<typename K, typename V, typename C>
    const bool IntervalTree<K, V, C>::iterator::operator==(const iterator& other)
    {
        return !current.owner_before(other.current) && !other.current.owner_before(current);
    }

    template<typename K, typename V, typename C>
    const bool IntervalTree<K, V, C>::iterator::operator!=(const iterator& other)
    {
        return current.owner_before(other.current) || other.current.owner_before(current);
    }


    /**
     * Node Constructor
     * ================================
     *
     * When adding a new node, we must update the parent nodes to ensure
     * the maximum values are reflected correctly.
     */
    template<typename K, typename V, typename C>
    IntervalTree<K, V, C>::Node::Node(bool red, K lower_bound, K upper_bound, V value, std::weak_ptr<Node> parent)
        : red(red), bounds(std::make_pair(lower_bound, upper_bound))
        , max_upper_bound(upper_bound), value(value), parent(parent)
    {
        if(auto p = parent.lock()) {
            p->updateMaximum();
        }
    }


    /**
     * Node Maximum Update
     * ================================
     */
    template<typename K, typename V, typename C>
    void IntervalTree<K, V, C>::Node::updateMaximum(C compare)
    {
        // Find the largest bound (note it isn't necessarily the current max)
        K top = bounds.second;
        if(left && compare(top, left->max_upper_bound)) {
            top = left->max_upper_bound;
        }
        if(right && compare(top, right->max_upper_bound)) {
            top = right->max_upper_bound;
        }
        max_upper_bound = top;

        // Propagate the changes upward
        if(auto p = parent.lock()) {
            if(compare(p->max_upper_bound, max_upper_bound)) {
                p->updateMaximum();
            }
        }
    }


    /**
     * Node Successor Value
     * ================================
     */
    template<typename K, typename V, typename C>
    std::weak_ptr<struct IntervalTree<K, V, C>::Node> IntervalTree<K, V, C>::Node::successor()
    {
        // If there is a right child, we find the smallest element
        if(right) {
            auto current = right;
            while(current->left) {
                current = current->left;
            }
            return current;
        }

        // Otherwise we need to move up the tree
        auto current = this;
        auto p = parent.lock();
        while(p && p->right.get() == current) {
            current = p.get();
            p = p->parent.lock();
        }

        return std::weak_ptr<Node>(p);
    }


    /**
     * Constructor
     * ================================
     */
    template<typename K, typename V, typename C>
    IntervalTree<K, V, C>::IntervalTree()
        : root(std::shared_ptr<Node>())
    { }


    /**
     * Iterator Methods
     * ================================
     */
    template<typename K, typename V, typename C>
    typename IntervalTree<K, V, C>::iterator IntervalTree<K, V, C>::begin()
    {
        if(!root) {
            return end();
        }

        // Look to the left as much as possible for eventual successor calling
        auto current = root;
        while(current->left) {
            current = current->left;
        }

        return iterator(current);
    }

    template<typename K, typename V, typename C>
    typename IntervalTree<K, V, C>::iterator IntervalTree<K, V, C>::end()
    {
        return iterator(std::weak_ptr<Node>(std::shared_ptr<Node>()));
    }


    /**
     * Remove
     * ================================
     *
     * Removes the node matching the given interval exactly.
     *
     * Runs in O(log(n)) time.
     */
    template<typename K, typename V, typename C>
    void IntervalTree<K, V, C>::remove(K lower_bound, K upper_bound, C compare)
    {
        // TODO
        assert(false);
    }


    /**
     * Insert
     * ================================
     *
     * Constructs a new interval matching the given interval. Note
     * intervals can overlap; if one is looking for strict disjointedness,
     * it needs to be employed separately by the application code.
     *
     * Orderings of intervals are conducted by the left endpoint exclusively.
     *
     * Runs in O(log(n)) time.
     */
    template<typename K, typename V, typename C>
    void IntervalTree<K, V, C>::insert(K lower_bound, K upper_bound, V value, C compare)
    {
        // Find where to insert element
        auto current = root;
        auto parent = std::weak_ptr<Node>(std::shared_ptr<Node>());
        while(current) {
            parent = current;
            if(current->left && compare(lower_bound, current->left->max_upper_bound)) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        // Insert new red node
        // Otherwise must be at the root
        auto next = std::make_shared<Node>(true, lower_bound, upper_bound, value, parent);
        if(auto ptr = parent.lock()) {
            if(compare(lower_bound, ptr->max_upper_bound)) {
                ptr->left = next;
            } else {
                ptr->right = next;
            }
        } else {
            root = next;
        }

        // Apply fixes and change root color
        insert_fixup(next);
        root->red = false;

    }


    /**
     * Insert Fixup
     * ================================
     *
     * Corrects the invariants of the tree by applying any necessary
     * rotations upward (by examining the varying color combinations).
     */
    template<typename K, typename V, typename C>
    void IntervalTree<K, V, C>::insert_fixup(std::shared_ptr<Node> current)
    {
        auto parent = current->parent.lock();
        if(parent && parent->red) {

            // Since parent is red, we know it must have a parent itself
            auto g_parent = parent->parent.lock();
            auto uncle = (g_parent->left != parent) ? g_parent->left : g_parent->right;

            // Case 1: If uncle is red, just a matter of recoloring and propagating changes
            if(uncle && uncle->red) {
                g_parent->red = true;
                parent->red = false;
                uncle->red = false;
                insert_fixup(g_parent);

            // Case 2: Uncle is black and parent is left child
            } else if(g_parent->left == parent) {
                if(parent->right == current) {
                    lrRotate(g_parent);
                }
                llRotate(g_parent);
                g_parent->red = true;
                current->red = false;

            // Case 3: Uncle is black and parent is right child
            } else {
                if(parent->left == current) {
                    rlRotate(g_parent);
                }
                rrRotate(g_parent);
                g_parent->red = true;
                current->red = false;
            }
        }

    }


    /**
     * Find
     * ================================
     *
     * Looks through the tree to see if the passed value falls into
     * some interval. If not, returns an end iterator.
     *
     * Runs in O(log(n)) time.
     */
    template<typename K, typename V, typename C>
    typename IntervalTree<K, V, C>::iterator
    IntervalTree<K, V, C>::find(K lower_bound, K upper_bound, C compare)
    {
        auto current = root;

        while(current) {
            if(compare(current->bounds.first, lower_bound) && compare(upper_bound, current->bounds.second)) {
                return iterator(current);
            } else if(current->left && compare(lower_bound, current->left->max_upper_bound)) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return end();
    }


    /**
     * Rotation Methods
     * ================================
     *
     * The following methods are constant time operations used to reorganize
     * the level of the trees. These are used to properly balance our nodes.
     */
    template<typename K, typename V, typename C>
    void IntervalTree<K, V, C>::llRotate(std::shared_ptr<Node> A)
    {
        auto B = A->left;

        // Apply rotation
        A->left = B->right;
        B->right = A;

        // Adjust parents after rotation
        B->parent = A->parent;
        if(auto b_parent = B->parent.lock()) {
            if(b_parent->left == A) {
                b_parent->left = B;
            } else {
                b_parent->right = B;
            }
        } else {
            root = B;
        }

        A->parent = B;

        // Adjust maximums
        B->max_upper_bound = A->max_upper_bound;
        A->updateMaximum();
    }

    template<typename K, typename V, typename C>
    void IntervalTree<K, V, C>::lrRotate(std::shared_ptr<Node> A)
    {
        auto B = A->left;
        auto R = B->right;

        // Apply rotation
        A->left = R;
        B->right = R->left;
        R->left = B;

        // Adjust parents after rotation
        B->parent = R;
        R->parent = A;

        // Adjust maximums
        R->max_upper_bound = B->max_upper_bound;
        B->updateMaximum();
    }

    template<typename K, typename V, typename C>
    void IntervalTree<K, V, C>::rrRotate(std::shared_ptr<Node> A)
    {
        auto B = A->right;

        // Apply rotation
        A->right = B->left;
        B->left = A;

        // Adjust parents after rotation
        B->parent = A->parent;
        if(auto b_parent = B->parent.lock()) {
            if(b_parent->left == A) {
                b_parent->left = B;
            } else {
                b_parent->right = B;
            }
        } else {
            root = B;
        }

        A->parent = B;

        // Adjust maximums
        B->max_upper_bound = A->max_upper_bound;
        A->updateMaximum();
    }

    template<typename K, typename V, typename C>
    void IntervalTree<K, V, C>::rlRotate(std::shared_ptr<Node> A)
    {
        auto B = A->right;
        auto R = B->left;

        // Apply rotation
        A->right = R;
        B->left = R->right;
        R->right = B;

        // Adjust parents after rotation
        B->parent = R;
        R->parent = A;

        // Adjust maximums
        R->max_upper_bound = B->max_upper_bound;
        B->updateMaximum();
    }

}

#endif //SAGE_INTERVAL_H
