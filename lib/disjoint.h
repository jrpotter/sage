/**
 * disjoint.h
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_DISJOINT_H
#define SAGE_DISJOINT_H

#include <map>
#include <vector>
#include <exception>

namespace sage {

    /**
     * DisjointSet
     *
     * The DisjointSet maintains a forest of trees, each of which eventually point
     * back to the parent node as the representative of the set. The following performs
     * both rank comparison and path compression for quick access times.
     */
    template<typename T>
    class DisjointSet {

        public:
            DisjointSet();
            ~DisjointSet() = default;
            DisjointSet(const DisjointSet &);
            DisjointSet(DisjointSet &&);
            DisjointSet &operator=(DisjointSet);
            void swap(DisjointSet &, DisjointSet &);

            // Exception thrown when search conducted for non-existing element
            class NoElementException : public std::exception {
            public:
                virtual const char *what() const throw() {
                    return "Could not find element in set";
                }
            } noElementException;

            // Basic Operations
            T findSet(T) throw(NoElementException);
            void join(T, T) throw(NoElementException);
            void createSet(T);

            // Convenience method meant to convert table into an actual table for lookup.
            // Note this method may perform path compression, but the actual relationships
            // are maintained nonetheless.
            //
            // The map has a key representing a parent and a value representing all nodes
            // (including the representative) tied to the key.
            const std::map<T, std::vector<T>> getTable();

        private:
            struct Node {
                int rank;
                T parent;
                Node(int rank, T parent);
            };

            std::map<T, std::shared_ptr<Node>> table;

    };

    /**
     * Node Constructor
     */
    template<typename T>
    DisjointSet<T>::Node::Node(int rank, T parent)
            : rank(rank), parent(parent) {

    }

    /**
     * Constructor.
     */
    template<typename T>
    DisjointSet<T>::DisjointSet() {

    }

    /**
     * Copy Constructor.
     */
    template<typename T>
    DisjointSet<T>::DisjointSet(const DisjointSet<T> &other) {
        for (auto pair : other.table) {
            table[pair.first] = std::make_shared<Node>(pair.second->rank, pair.second->parent);
        }
    }

    /**
     * Move Constructor.
     */
    template<typename T>
    DisjointSet<T>::DisjointSet(DisjointSet<T> &&other)
            : table(other.table) {

    }

    /**
     * Assignment Operator.
     */
    template<typename T>
    DisjointSet<T> &DisjointSet<T>::operator=(DisjointSet<T> other) {
        swap(*this, other);
        return *this;
    }

    /**
     * Swap Operator.
     */
    template<typename T>
    void DisjointSet<T>::swap(DisjointSet<T> &a, DisjointSet<T> &b) {
        using std::swap;
        swap(a.table, b.table);
    }

    /**
     * Find Set.
     *
     * Employs path compression for an amortized running cost inversely
     * proportional to the Ackermann function (when employed with rank comparison).
     */
    template<typename T>
    T DisjointSet<T>::findSet(T value) throw(NoElementException) {
        auto it = table.find(value);
        if (it == table.end()) {
            throw noElementException;
        }

        auto n = it->second;
        if (value != n->parent) {
            n->parent = findSet(n->parent);
        }

        return n->parent;
    }

    /**
     * Does rank comparison.
     *
     * This basic optimization ensures O(log(n)) runtime.
     */
    template<typename T>
    void DisjointSet<T>::join(T a, T b) throw(NoElementException) {
        T first = findSet(a), second = findSet(b);

        if (table[first]->rank < table[second]->rank) {
            table[first]->parent = second;
        } else if (table[first]->rank > table[second]->rank) {
            table[second]->parent = first;
        } else {
            table[first]->parent = second;
            table[second]->rank++;
        }
    }

    /**
     * Create Set.
     *
     * Initial operation to add element into disjoint forest.
     */
    template<typename T>
    void DisjointSet<T>::createSet(T value) {
        if(table.find(value) != table.end()) {
            table[value] = std::make_shared<Node>(1, value);
        }
    }

    /**
     * Construct Table.
     *
     * Used to determine properties of a given table.
     */
    template<typename T>
    const std::map<T, std::vector<T>> DisjointSet<T>::getTable()
    {
        std::map<T, std::vector<T>> mapping;
        for(auto pair : table) {
            T parent = findSet(pair.first);
            if(mapping.find(parent) == mapping.end()) {
                mapping.emplace(parent, std::vector<T>());
            }
            mapping[parent].push_back(pair.first);
        }

        return mapping;
    };

}

#endif //SAGE_DISJOINT_H
