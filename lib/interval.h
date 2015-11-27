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
#include <memory>

namespace sage
{
    template<typename K, typename V, typename Compare=std::less_equal<K>>
    class IntervalTree {

        public:

            // Constructors
            IntervalTree();
            ~IntervalTree() = default;
            IntervalTree(const IntervalTree &);
            IntervalTree(IntervalTree &&);
            IntervalTree &operator=(IntervalTree);
            void swap(IntervalTree &, IntervalTree &);
    
            // Default Operation
            V find(K);
            void remove(K, K);
            void insert(K, K, V);
            bool containsInterval(K, K);

        private:

            struct Node {
                bool red;
                std::weak_ptr<Node> parent;
                std::shared_ptr<Node> left;
                std::shared_ptr<Node> right;
            };

            std::shared_ptr<Node> root;
    };

}

#endif //SAGE_INTERVAL_H
