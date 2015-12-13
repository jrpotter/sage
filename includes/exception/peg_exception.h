/**
 * peg_exception.h
 *
 * The exception raised whenever the construction of a PEG fails.
 *
 * Created by jrpotter (11/26/2015).
 */
#ifndef SAGE_PEG_EXCEPTION_H
#define SAGE_PEG_EXCEPTION_H

#include <exception>
#include <string>

namespace sage
{
    class PEGException : public std::exception
    {
        public:
            PEGException(std::string);
            virtual const char* what() const noexcept;

        private:
            std::string message;
    };
}

#endif //SAGE_PEG_EXCEPTION_H
