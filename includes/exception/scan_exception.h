/**
 * scan_exception.h
 *
 * The exception raised when scanning fails.
 *
 * Created by jrpotter (12/13/2015).
 */

#ifndef SAGE_SCAN_EXCEPTION_H
#define SAGE_SCAN_EXCEPTION_H

#include <exception>
#include <sstream>

namespace sage
{
    class ScanException : public std::exception
    {
        public:
            ScanException(std::string, unsigned int, unsigned int);
            virtual const char* what() const noexcept;

        private:
            std::string response;
    };
}

#endif //SAGE_SCAN_EXCEPTION_H
