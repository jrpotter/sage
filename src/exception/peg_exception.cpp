/**
 * peg_exception.cpp
 *
 * Created by jrpotter (12/13/2015).
 */

#include "exception/peg_exception.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
PEGException::PEGException(std::string message)
    : response(message)
{ }

PEGException::PEGException(std::string message, unsigned int line, unsigned int column)
{
    std::stringstream ss(message);
    ss << " at (line: " << line << ", column: " << column << ")";
    response = ss.str();
}

/**
 * What
 * ================================
 */
const char* PEGException::what() const noexcept
{
    return response.c_str();
}
