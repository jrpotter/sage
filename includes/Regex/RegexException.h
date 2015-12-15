/**
 * regex_exception.h
 *
 * The exception raised when an invalid regular expression is attempted to be constructed.
 *
 * Created by jrpotter (12/13/2015).
 */

#ifndef SAGE_REGEX_EXCEPTION_H
#define SAGE_REGEX_EXCEPTION_H

#include <exception>
#include <sstream>

namespace sage
{
    class RegexException : public std::exception
    {
        public:
            RegexException(std::string, long);
            RegexException(std::string, char, long);
            virtual const char* what() const noexcept;

        private:
            std::string response;
    };
}

#endif //SAGE_REGEX_EXCEPTION_H
