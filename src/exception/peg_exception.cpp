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
        : message(message)
{ }

/**
 * What
 * ================================
 */
const char* PEGException::what() const noexcept
{
    return message.c_str();
}
