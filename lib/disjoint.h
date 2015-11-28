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

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <exception>

namespace sage
{
    template<typename T>
    class DisjointSet
    {
        struct Node;
        typedef std::map<T, std::shared_ptr<Node>> pair_t;
        typedef std::map<T, std::set<std::shared_ptr<Node>>> forest_t;

        public:

            // Iterator
            class iterator
            {
                public:
                    iterator(DisjointSet*);
                    iterator(DisjointSet*, typename forest_t::iterator);
                    iterator operator++();
                    iterator operator++(int);
                    const long operator-(const iterator&) const;
                    const T operator*();
                    const T* operator->();
                    const bool operator==(const iterator&);
                    const bool operator!=(const iterator&);

                private:
                    long index;
                    DisjointSet* set;
                    typename forest_t::iterator it;
            };

            // Constructors
            DisjointSet() = default;
            ~DisjointSet() = default;
            DisjointSet(const DisjointSet&);
            DisjointSet(DisjointSet&&);
            DisjointSet& operator=(DisjointSet);
            void swap(DisjointSet&, DisjointSet&);

            // Iterator Operations
            iterator begin();
            iterator end();

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
            pair_t table;

            // Mapping between each element and the means of determining
            // the representative of the passed element
            forest_t reprs;
    };


    /**
     * Iterator Constructor
     * ================================
     */
    template<typename T>
    DisjointSet<T>::iterator::iterator(DisjointSet* set)
            : index(0), set(set), it(set->reprs.begin())
    { }

    template<typename T>
    DisjointSet<T>::iterator::iterator(DisjointSet* set, typename forest_t::iterator itr)
            : set(set)
    {
        if(itr == set->reprs.end()) {
            index = set->reprs.size();
            it = itr;
        } else {
            index = 0;
            for(it = set->reprs.begin(); it != itr; it++, index++) { }
        }
    }


    /**
     * Iterator Increment Operators
     * ================================
     */
    template<typename T>
    typename DisjointSet<T>::iterator DisjointSet<T>::iterator::operator++()
    {
        ++it;
        ++index;
        return *this;
    }

    template<typename T>
    typename DisjointSet<T>::iterator DisjointSet<T>::iterator::operator++(int)
    {
        iterator tmp(*this);
        it++;
        index++;
        return tmp;
    }


    /**
     * Iterator Distance Operators
     * ================================
     *
     * Rather than using std::distance (which performs rather poorly!),
     * we keep track of the distance of a given iterator and return that
     * instead.
     */
    template<typename T>
    const long DisjointSet<T>::iterator::operator-(const iterator& other) const
    {
        return index - other.index;
    }


    /**
     * Iterator Reference Operators
     * ================================
     */
    template<typename T>
    const T DisjointSet<T>::iterator::operator*()
    {
        return it->first;
    }

    template<typename T>
    const T* DisjointSet<T>::iterator::operator->()
    {
        return &it->first;
    }


    /**
     * Iterator Equality Operators
     * ================================
     */
    template<typename T>
    const bool DisjointSet<T>::iterator::operator==(const iterator& other)
    {
        return index == other.index;
    }

    template<typename T>
    const bool DisjointSet<T>::iterator::operator!=(const iterator& other)
    {
        return index != other.index;
    }


    /**
     * Node Constructor
     * ================================
     */
    template<typename T>
    DisjointSet<T>::Node::Node(int rank, T parent)
            : rank(rank), parent(parent)
    { }


    /**
     * Copy Constructor
     * ================================
     */
    template<typename T>
    DisjointSet<T>::DisjointSet(const DisjointSet<T>& other)
    {
        for (auto pair : other.table) {
            table[pair.first] = std::make_shared<Node>(pair.second->rank, pair.second->parent);
        }
    }


    /**
     * Move Constructor
     * ================================
     */
    template<typename T>
    DisjointSet<T>::DisjointSet(DisjointSet<T>&& other)
            : table(other.table)
    { }


    /**
     * Assignment Operator
     * ================================
     */
    template<typename T>
    DisjointSet<T>& DisjointSet<T>::operator=(DisjointSet<T> other)
    {
        swap(*this, other);
        return *this;
    }


    /**
     * Swap Operator
     * ================================
     */
    template<typename T>
    void DisjointSet<T>::swap(DisjointSet<T>& a, DisjointSet<T>& b)
    {
        using std::swap;
        swap(a.table, b.table);
    }


    /**
     * Iterator Operations
     * ================================
     */
    template<typename T>
    typename DisjointSet<T>::iterator DisjointSet<T>::begin()
    {
        return iterator(this);
    }

    template<typename T>
    typename DisjointSet<T>::iterator DisjointSet<T>::end()
    {
        return iterator(this, reprs.end());
    }


    /**
     * Find Set
     * ================================
     *
     * Employs path compression for an amortized running cost inversely
     * proportional to the Ackermann function (when employed with rank comparison).
     */
    template<typename T>
    typename DisjointSet<T>::iterator DisjointSet<T>::findSet(T value)
    {
        auto it = table.find(value);
        if (it == table.end()) {
            return iterator(this, reprs.end());
        } else if (value != it->second->parent) {
            it->second->parent = *findSet(it->second->parent);
        }
        return iterator(this, reprs.find(it->second->parent));
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
    template<typename T>
    void DisjointSet<T>::join(T a, T b)
    {
        iterator first = findSet(a);
        iterator second = findSet(b);

        // Invalid join request. Do nothing.
        if(first == end() || second == end()) {
            return;

        // The left most element should become parent
        } else if (table[*first]->rank > table[*second]->rank) {
            table[*second]->parent = a;
            reprs[*first].insert(reprs[*second].begin(), reprs[*second].end());
            reprs.erase(*second);

        // The rightmost element shoudl become parent
        } else {
            table[*first]->parent = b;
            reprs[*second].insert(reprs[*first].begin(), reprs[*first].end());
            reprs.erase(*first);
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
    template<typename T>
    void DisjointSet<T>::createSet(T value)
    {
        if (table.find(value) == table.end()) {
            table[value] = std::make_shared<Node>(1, value);
            reprs[value] = std::set<std::shared_ptr<Node>>();
            reprs[value].insert(table[value]);
        }
    }

}

#endif //SAGE_DISJOINT_H
