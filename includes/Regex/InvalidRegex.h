/**
 * invalid_regex.h
 *
 * The exception raised when an invalid regular expression is attempted to be constructed.
 *
 * Created by jrpotter (12/13/2015).
 */

#ifndef SAGE_INVALID_REGEX_H
#define SAGE_INVALID_REGEX_H

#include <exception>
#include <sstream>

namespace sage
{
    class InvalidRegex : public std::exception
    {
        public:
            InvalidRegex(std::string, long);
            InvalidRegex(std::string, char, long);
            virtual const char* what() const noexcept;

        private:
            std::string response;
    };
}

#endif //SAGE_INVALID_REGEX_H
