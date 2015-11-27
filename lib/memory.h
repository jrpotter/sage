//
// Created by jrpotter on 11/26/15.
//

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
