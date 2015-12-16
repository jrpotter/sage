/**
 * peg_exception.cpp
 *
 * Created by jrpotter (12/13/2015).
 */

#include "Parser/InvalidGrammar.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
InvalidGrammar::InvalidGrammar(std::string message)
    : response(message)
{ }

InvalidGrammar::InvalidGrammar(std::string message, ScanState state)
{
    std::stringstream ss(message);
    ss << " at (line: " << state.getLine() << ", column: " << state.getColumn() << ")";
    response = ss.str();
}

/**
 * What
 * ================================
 */
const char* InvalidGrammar::what() const noexcept
{
    return response.c_str();
}
