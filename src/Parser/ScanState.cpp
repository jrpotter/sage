/**
 * scan_state.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "Parser/ScanState.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
ScanState::ScanState(std::istream& stream, unsigned int line, unsigned int column)
    : cursor(stream.tellg())
    , line(line), column(column)
    , buffer_state(stream.rdstate())
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

std::ios_base::iostate ScanState::getBufferState() const
{
    return buffer_state;
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

/**
 * Reset
 * ================================
 *
 * This is merely a convenience function to advance the state's position.
 */
void ScanState::reset(std::istream& stream, unsigned int n_line, unsigned int n_column)
{
    cursor = stream.tellg();
    buffer_state = stream.rdstate();
    line = (n_line > -1) ? n_line : line;
    column = (n_column > -1) ? n_column : column;
}