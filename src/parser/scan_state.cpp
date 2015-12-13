/**
 * scan_state.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "parser/scan_state.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
ScanState::ScanState(long current, unsigned int line, unsigned int column)
    : cursor(current)
    , line(line)
    , column(column)
{ }

/**
 * Getters
 * ================================
 */
long ScanState::getCursor() const
{
    return cursor;
}

unsigned int ScanState::getLine() const
{
    return line;
}

unsigned int ScanState::getColumn() const
{
    return column;
}

/**
 * Advance
 * ================================
 *
 * This is merely a convenience function to advance the state's position.
 */
void ScanState::advance(const char c)
{
    cursor += 1;
    if(c == '\n') {
        line += 1;
        column = 1;
    } else {
        column += 1;
    }
}