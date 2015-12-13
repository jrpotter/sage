/**
 * scanner.cpp
 *
 * Created by jrpotter (11/26/2015).
 */
#include "memory.h"
#include "scanner.h"
#include <iostream>

using namespace sage;

/**
 * Constructor
 * ================================
 */
Scanner::Scanner(std::istream& input, std::string delimiter)
    : input(input)
    , delimiter(Regex(delimiter))
{
    // Ensure our token is at the front of the stream
    clearDelimiterContent();
}

/**
 * Next Methods
 * ================================
 *
 * The following methods will, without checking, reads in the next token
 * (as distinguished by the delimiter) and attempts to convert it to
 * the corresponding type. If this fails in any case, an exception is
 * thrown.
 */
int Scanner::nextInt()
{
    Regex key = Regex::fromPool(REGEX_POOL_INTEGRAL, REGEX_EXPR_INTEGRAL);
    return std::stoi(next(key));
}

char Scanner::nextChar()
{
    Regex key = Regex::fromPool(REGEX_POOL_CHAR, REGEX_EXPR_CHAR);
    std::string tmp = next(key);
    return tmp[0];
}

char Scanner::nextLetter()
{
    Regex key = Regex::fromPool(REGEX_POOL_LETTER, REGEX_EXPR_LETTER);
    std::string tmp = next(key);
    return tmp[0];
}

double Scanner::nextDouble()
{
    Regex key = Regex::fromPool(REGEX_POOL_FLOAT, REGEX_EXPR_FLOAT);
    return std::stod(next(key));
}

std::string Scanner::nextWord()
{
    Regex key = Regex::fromPool(REGEX_POOL_WORD, REGEX_EXPR_WORD);
    return next(key);
}

/**
 * Next
 * ================================
 * Must read in as much of a delimiter as possible and then advance forward. This does
 * present some limitations though. For instance, suppose my delimiter was actually
 * a floating number, where having a decimal point was optional, but, if included, must
 * have trailing numbers following it. That is:
 *
 *  delimiter = [+\-]?(0|[1-9]\d*)?(\.\d+)?
 *
 * Then when we encounter a number we continue to read, but once we read in the decimal
 * point we denote the scan as a failure and continue from this point on. But that means
 * input that was '87.34' finished reading until '.34'! Thus, when deciding on a delimiter,
 * make sure there is no possibility of confusion. One could potentially modify the
 * above regex like so (note the Kleene star):
 *
 *  delimiter = [+\-]?(0|[1-9]\d*)?(\.\d*)?
 *
 * to avoid any confusion, but this may also be undesirable. Consider all possible tradeoffs.
 */
std::string Scanner::next(Regex r)
{
    // Read in input until next separator or end
    // This is necessary to avoid problems regarding the above comment before the function
    // signature. That is, we read until we encounter our delimiter and then verify the
    // token we just read in matches the passed regex. If not, we reset the inputstream back
    // to where it was.
    std::string token;
    long current = input.tellg();
    while(input.peek() != EOF && !delimiter.matches(std::string(1, (char) input.peek()))) {
        token += input.get();
    }

    // Reset position, return empty string
    if(!r.matches(token)) {
        input.seekg(current);
        throw std::invalid_argument("Could not find specified regex.");

    // Otherwise parse content afterward for next character
    } else {
        clearDelimiterContent();
    }

    return token;
}

/**
 * Read Line
 * ================================
 *
 * Reads in the remainder of the line (until the corresponding end of line character is found).
 */
std::string Scanner::readLine()
{
    std::string buffer;
    if(!std::getline(input, buffer)) {
        throw std::invalid_argument("Could not extract line.");
    }

    buffer.erase(buffer.find_last_not_of(" \n\r\t\v") + 1);
    clearDelimiterContent();
    return buffer;
}

/**
 * Peek
 * ================================
 *
 * Checks if a character exists in the remainder of the stream.
 * This will seek to the desired location and then look for a
 * character not corresponding to the delimiter, unless @force
 * is set to true.
 *
 * Returns EOF if no character is found.
 */
char Scanner::peek(int pos)
{
    long current = input.tellg();
    input.seekg(current + pos);
    char tmp = (char) input.peek();
    input.seekg(current);
    return tmp;
}

/**
 * Clear Delimiter Content
 * ================================
 *
 * Convenience method to remove any delimiter content (such as whitespace)
 * between tokens in the specified stream. This is necessary to ensure
 * that the end of the stream has been reached at times.
 */
void Scanner::clearDelimiterContent()
{
    std::string separator;
    while(input.peek() != EOF && delimiter.matches(separator + (char) input.peek())) {
        separator += input.get();
    }
}