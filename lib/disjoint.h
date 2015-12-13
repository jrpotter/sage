/**
 * disjoint.h
 *
 * The DisjointSet maintains a forest of trees, each of which eventually point
 * back to the parent node as the representative of the set. The following performs
 * both rank comparison and path compression for quick access times.
 *
 * Note this implementation could be improved by allowing more instant access with
 * an iterator. But as it stands, it is not really necessary to be able to start
 * from a node besides the start or end.
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_DISJOINT_H
#define SAGE_DISJOINT_H

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <vector>

namespace sage
{
    template<typename T, typename C = std::less<T>>
    class DisjointSet
    {
        struct Node;

        public:

            // Iterator
            // Merely serves as a wrapper/marker for elements
            // For this reason there is only an @end() method
            // and not a @begin()
            class iterator
            {
                public:
                    iterator();
                    iterator(T);
                    const T& operator*();
                    const T* operator->();
                    const bool operator==(const iterator&);
                    const bool operator!=(const iterator&);

                private:
                    std::shared_ptr<T> value;
            };

            // Constructors
            DisjointSet() = default;
            ~DisjointSet() = default;
            DisjointSet(const DisjointSet&);
            DisjointSet(DisjointSet&&);
            DisjointSet& operator=(DisjointSet);
            void swap(DisjointSet&, DisjointSet&);

            // Iterator Operations
            iterator end() const;

            // Basic Operations
            void join(T, T);
            void createSet(T);
            iterator findSet(T);

        private:

            // Represents the tree node in the disjoint forest
            struct Node
            {
                int rank;
                T parent;
                Node(int rank, T parent);
            };

            // The following pair maps every entry to its corresponding node.
            // This allows for quick finding of sets.
            std::map<T, std::shared_ptr<Node>, C> table;
    };

    /**
     * Iterator Constructor
     * ================================
     */
    template<typename T, typename C>
    DisjointSet<T, C>::iterator::iterator()
            : value(std::shared_ptr<T>())
    { }

    template<typename T, typename C>
    DisjointSet<T, C>::iterator::iterator(T value)
            : value(std::make_shared<T>(value))
    { }

    /**
     * Iterator Reference Operators
     * ================================
     */
    template<typename T, typename C>
    const T& DisjointSet<T, C>::iterator::operator*()
    {
        return *value;
    }

    template<typename T, typename C>
    const T* DisjointSet<T, C>::iterator::operator->()
    {
        return value.get();
    }

    /**
     * Iterator Equality Operators
     * ================================
     */
    template<typename T, typename C>
    const bool DisjointSet<T, C>::iterator::operator==(const iterator& other)
    {
        return value == other.value;
    }

    template<typename T, typename C>
    const bool DisjointSet<T, C>::iterator::operator!=(const iterator& other)
    {
        return value != other.value;
    }

    /**
     * Node Constructor
     * ================================
     */
    template<typename T, typename C>
    DisjointSet<T, C>::Node::Node(int rank, T parent)
            : rank(rank), parent(parent)
    { }

    /**
     * Copy Constructor
     * ================================
     */
    template<typename T, typename C>
    DisjointSet<T, C>::DisjointSet(const DisjointSet<T, C>& other)
    {
        for (auto pair : other.table) {
            table[pair.first] = std::make_shared<Node>(pair.second->rank, pair.second->parent);
        }
    }

    /**
     * Move Constructor
     * ================================
     */
    template<typename T, typename C>
    DisjointSet<T, C>::DisjointSet(DisjointSet<T, C>&& other)
            : table(other.table)
    { }

    /**
     * Assignment Operator
     * ================================
     */
    template<typename T, typename C>
    DisjointSet<T, C>& DisjointSet<T, C>::operator=(DisjointSet<T, C> other)
    {
        swap(*this, other);
        return *this;
    }

    /**
     * Swap Operator
     * ================================
     */
    template<typename T, typename C>
    void DisjointSet<T, C>::swap(DisjointSet<T, C>& a, DisjointSet<T, C>& b)
    {
        using std::swap;
        swap(a.table, b.table);
    }

    /**
     * Iterator Operations
     * ================================
     */
    template<typename T, typename C>
    typename DisjointSet<T, C>::iterator DisjointSet<T, C>::end() const
    {
        return iterator();
    }

    /**
     * Find Set
     * ================================
     *
     * Employs path compression for an amortized running cost inversely
     * proportional to the Ackermann function (when employed with rank comparison).
     */
    template<typename T, typename C>
    typename DisjointSet<T, C>::iterator DisjointSet<T, C>::findSet(T value)
    {
        auto it = table.find(value);
        if (it == table.end()) {
            return end();
        } else if (value != it->second->parent) {
            it->second->parent = *findSet(it->second->parent);
        }
        return iterator(it->second->parent);
    }

    /**
     * Rank Comparison
     * ================================
     *
     * This basic optimization ensures O(log(n)) runtime.
     * Higher ranked nodes are marked as the parent of that of lower ranked nodes.
     * Equally ranked nodes have the second element automatically assigned as
     * the new parent node.
     */
    template<typename T, typename C>
    void DisjointSet<T, C>::join(T a, T b)
    {
        iterator first = findSet(a);
        iterator second = findSet(b);

        // Invalid join request. Do nothing.
        if(first == end() || second == end()) {
            return;

        // The left most element should become parent
        } else if (table[*first]->rank > table[*second]->rank) {
            table[*second]->parent = a;

        // The rightmost element should become parent
        } else {
            table[*first]->parent = b;
            if(table[*first]->rank == table[*second]->rank) {
                table[*second]->rank++;
            }
        }
    }

    /**
     * Create Set.
     * ================================
     *
     * Initial operation to add element into disjoint forest.
     */
    template<typename T, typename C>
    void DisjointSet<T, C>::createSet(T value)
    {
        if (table.find(value) == table.end()) {
            table[value] = std::make_shared<Node>(1, value);
        }
    }

}

#endif //SAGE_DISJOINT_H
