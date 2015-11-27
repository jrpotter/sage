/**
 * disjoint.h
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_DISJOINT_H
#define SAGE_DISJOINT_H

#include <map>

namespace sage {

    template<typename T>
    class DisjointSet<T> {

        public:

            DisjointSet();
            ~DisjointSet()=default;
            DisjointSet(const DisjointSet&);
            DisjointSet(DisjointSet&&);
            DisjointSet& operator= (DisjointSet);

            T findSet(T);
            void join(T, T);
            void createSet(T);

    private:

        template<typename U>
        struct Node<U> {
            U parent;
            int rank;
        };

        std::map<T, Node<T>> table;

    };

    /**
     * Constructor.
     */
    template<typename T>
    DisjointSet<T>::DisjointSet()
    {

    }

    /**
     * Copy Constructor.
     */
    template<typename T>
    DisjointSet<T>::DisjointSet(const DisjointSet<T>& other)
    {

    }

    /**
     * Move Constructor.
     */
    template<typename T>
    DisjointSet<T>::DisjointSet(DisjointSet<T>&& other)
    {

    }

    /**
     * Assignment Operator.
     */
    template<typename T>
    DisjointSet<T>& DisjointSet<T>::operator= (DisjointSet<T> other)
    {

    }

    /**
     * Find Set.
     *
     * Employs path compression for an amortized running cost inversely
     * proportional to the Ackermann function (when employed with rank comparison).
     */
    template<typename T>
    T DisjointSet<T>::findSet(T value)
    {
        Node<T> n = table[value];
        if(n == table.end) {
            return nullptr;
        }

        while(value != n.parent) {
            n.parent = findSet(n.parent);
        }

        return n.parent;
    }

    /**
     * Does rank comparison.
     *
     * This basic optimization ensures O(log(n)) runtime.
     */
    template<typename T>
    void DisjointSet<T>::join(T a, T b)
    {
        T first = findSet(a), second = findSet(b);
        if(first == nullptr || second == nullptr) {
            return;
        }

        if(table[first].rank < table[second].rank) {
            table[first].parent = second;
        } else if(table[first].rank > table[second].rank) {
            table[second].parent = first;
        } else {
            table[first].parent = second;
            table[second].rank++;
        }
    }

}

#endif //SAGE_DISJOINT_H
