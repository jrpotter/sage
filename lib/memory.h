/**
 * memory.h
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_MEMORY_H
#define SAGE_MEMORY_H

#include <memory>

namespace sage {

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

}

#endif //SAGE_MEMORY_H
