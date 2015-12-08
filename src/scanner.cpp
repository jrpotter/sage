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
{
    // Ensure our token is at the front of the stream
    clearDelimiterContent();
}

/**
 * Next Int
 * ================================
 *
 * Reads in an integer and converts it properly.
 */
int Scanner::nextInt()
{
    static Regex key(Regex::INTEGRAL);
    std::string tmp = next(key);
    return std::stoi(tmp.c_str());
}

/**
 * Next Char
 * ================================
 *
 * Reads in a single character.
 */
char Scanner::nextChar()
{
    static Regex key("[\\a\\U]");
    std::string tmp = next(key);
    if(tmp.empty()) {
        return -1;
    }

    return tmp[0];
}

/**
 * Next Double
 * ================================
 *
 * Reads in a double and converts it properly.
 */
double Scanner::nextDouble()
{
    static Regex key(Regex::FLOAT);
    std::string tmp = next(key);
    return std::stod(tmp);
}

/**
 * Next Word
 * ================================
 *
 * Reads in a word and converts it properly.
 */
std::string Scanner::nextWord()
{
    static Regex key(Regex::WORD);
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
        throw std::invalid_argument("Could not find specified regex");

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
    if(std::getline(input, buffer)) {
        return "";
    }

    return buffer;
}

/**
 * Peek Char
 * ================================
 *
 * Checks if a character exists in the remainder of the stream.
 * Returns EOF if no character is found.
 */
char Scanner::peekChar()
{
    long current = input.tellg();
    try {
        char tmp = nextChar();
        input.seekg(current);
        return tmp;
    } catch(const std::invalid_argument& ia) {
        return EOF;
    }
}

/**
 * Has Next Int
 * ================================
 *
 * Checks if an integer exists in the remainder of the stream.
 */
bool Scanner::hasNextInt()
{
    long current = input.tellg();
    try {
        int tmp = nextInt();
        input.seekg(current);
        return true;
    } catch(const std::invalid_argument& ia) {
        return false;
    }
}

/**
 * Has Next Char
 * ================================
 *
 * Checks if a character exists in the remainder of the stream.
 */
bool Scanner::hasNextChar()
{
    long current = input.tellg();
    try {
        char tmp = nextChar();
        input.seekg(current);
        return true;
    } catch(const std::invalid_argument& ia) {
        return false;
    }
}

/**
 * Has Next Double
 * ================================
 *
 * Checks if a double exists in the remainder of the stream.
 */
bool Scanner::hasNextDouble()
{
    long current = input.tellg();
    try {
        double tmp = nextDouble();
        input.seekg(current);
        return true;
    } catch(const std::invalid_argument& ia) {
        return false;
    }
}

/**
 * Has Next
 * ================================
 *
 * Checks if the passed regex exists in the remainder of the stream.
 */
bool Scanner::hasNext(Regex r)
{
    long current = input.tellg();
    try {
        std::string tmp = next(r);
        input.seekg(current);
        return true;
    } catch(const std::invalid_argument& ia) {
        return false;
    }
}

/**
 * EOF
 * ================================
 *
 * Checks if end of stream has been reached.
 */
bool Scanner::eof()
{
    return (input.peek() == EOF);
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