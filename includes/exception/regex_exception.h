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
#include <string>
#include <sstream>

namespace sage
{
    class InvalidRegularExpression : public std::exception
    {
        public:
            InvalidRegularExpression(std::string, char, long);
            virtual const char* what() const noexcept;

        private:
            std::string message;
    };
}

#endif //SAGE_REGEX_EXCEPTION_H
