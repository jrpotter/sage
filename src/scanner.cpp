//
// Created by jrpotter on 11/26/15.
//

#include "memory.h"
#include "scanner.h"

using namespace sage;

/**
 * Constructor.
 */
Scanner::Scanner()
{

}

/**
 * Copy Constructor.
 */
Scanner::Scanner(const Scanner& other)
{

}

/**
 * Move Constructor.
 */
Scanner::Scanner(Scanner&& other)
{

}

/**
 * Assignment Operator.
 */
Scanner& Scanner::operator= (Scanner other)
{
    swap(*this, other);
    return *this;
}

/**
 * Swap Operator.
 */
void Scanner::swap(Scanner& a, Scanner& b)
{
    using std::swap;
}