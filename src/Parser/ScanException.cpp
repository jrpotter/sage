/**
 * scan_exception.cpp
 *
 * Created by jrpotter (12/13/2015).
 */

#include "Parser/ScanException.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
ScanException::ScanException(std::string message, ScanState state)
{
    std::stringstream ss;
    ss << message << " at (line: " << state.getLine() << ", column: " << state.getColumn() << ")";
    response = ss.str();
}

/**
 * What
 * ================================
 */
const char* ScanException::what() const noexcept
{
    return response.c_str();
}