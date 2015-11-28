/**
 * memory.h
 *
 * Utility header provided to fulfill the deficiencies of the C++11 standard.
 * Note some of these are fixed in C++14, but the compiler I'm currently using
 * does not provide support yet.
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_MEMORY_H
#define SAGE_MEMORY_H

#include <memory>
#include <set>

namespace sage {

    template<typename T>
    using weak_set = std::set<std::weak_ptr<T>, std::owner_less<std::weak_ptr<T>>>;

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

}

#endif //SAGE_MEMORY_H
