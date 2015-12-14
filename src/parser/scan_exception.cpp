/**
 * scan_exception.cpp
 *
 * Created by jrpotter (12/13/2015).
 */

#include "parser/scan_exception.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
ScanException::ScanException(std::string message, unsigned int line, unsigned int column)
{
    std::stringstream ss;
    ss << message << " at (line: " << line << ", column: " << column << ")";
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