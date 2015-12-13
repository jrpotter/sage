/**
 * exception.h
 *
 * The following contains exception classes used throughout the sage project.
 *
 * Created by jrpotter (12/13/15).
 */
#ifndef SAGE_EXCEPTION_H
#define SAGE_EXCEPTION_H

#include <exception>
#include <string>
#include <sstream>

namespace sage
{
    // The following class is used by the PParser class to indicate errors
    // when parsing a PEG grammar.
    class PEGException : public std::exception
    {
        public:
            PEGException(std::string);
            virtual const char* what() const noexcept;

        private:
            std::string message;
    };

    // The following class is used by the Regex class to indicate
    // errors when constructing a regular expression.
    class InvalidRegularExpression : public std::exception
    {
        public:
            InvalidRegularExpression(std::string, char, long);
            virtual const char* what() const noexcept;

        private:
            std::string message;
    };
}

#endif //SAGE_EXCEPTION_H
