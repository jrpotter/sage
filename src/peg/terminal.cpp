/**
 * terminal.cpp
 *
 * Created by jrpotter (12/13/2015).
 */

#include "peg/terminal.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
Terminal::Terminal(std::string expr)
    : value(expr)
{ }

/**
 * Process
 * ================================
 *
 * Must run through input stream and look for a match if possible.
 */
bool Terminal::process(Scanner& s, std::map<std::string, std::shared_ptr<Definition>>&)
{
    try {
        s.next(value);
        return true;
    } catch(std::invalid_argument) {
        return false;
    }
}