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
    : Scanner(input, Regex(delimiter))
{ }

Scanner::Scanner(std::istream& input, Regex delimiter)
    : input(input), delimiter(delimiter)
{ }

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
    // Read until next token
    std::string separator;
    while(input.peek() != EOF && delimiter.matches(separator + (char) input.peek())) {
        separator += input.get();
    }

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
        return "";
    }

    return token;
}
