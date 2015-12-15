/**
 * scanner.cpp
 *
 * Created by jrpotter (11/26/2015).
 */
#include "Parser/Scanner.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
Scanner::Scanner(std::istream& input, std::string delimiter)
    : input(input)
    , states({ ScanState(0, 1, 1) })
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
 * Reads in as much of the regex as possible by considering delimited regions.
 * In particular, we break up the input into tokens and then expand upon the
 * token for a proper match.
 *
 * Note we manually check for word boundaries since we are managing a stream
 * and regex verifications are only functional in the contexts of strings.
 */

std::string Scanner::next(Regex r)
{
    long cursor = input.tellg();

    // If the regex is aligned to match along a word boundary at the front, we should
    // immediately check if we are along a boundary and continue only if this is the case
    if(r.getFrontWordBounded() && cursor > 0) {
        Regex whitespace = Regex::fromPool(REGEX_POOL_WHITESPACE, REGEX_EXPR_WHITESPACE);
        input.seekg(cursor - 1);
        if(!whitespace.matches(std::string(1, static_cast<char>(input.get())))) {
            throw ScanException("Could not align along word boundary", states.top());
        }
    }

    // We manually keep track of the current state and revert if necessary
    // Note the size of the columns stack will correspond to the number of lines read
    // during this function call, while the actual values maintains the last column
    // of the corresponding line read (or the column delta for the first line)
    std::stack<unsigned int> columns({1});

    // Read in input until next separator or end
    // This is necessary to avoid problems regarding the above comment before the function
    // signature. That is, we read until we encounter our delimiter and then verify the
    // token we just read in matches (at least in part) with the passed Regex.
    std::string token;
    while(input.peek() != EOF && !delimiter.matches(std::string(1, (char) input.peek()))) {
        cursor += 1;
        token += input.get();
        if(token.back() == '\n') {
            columns.push(1);
        } else {
            columns.top() += 1;
        }
    }

    // If we expect an alignment along the back of the string, we simply check if a match occurs
    // since the scanner naturally delimits via word boundaries (i.e. whitespace)
    std::string token_copy = token;
    if(r.getBackWordBounded()) {
        if(r.matches(token)) {
            return token;
        }

    // Otherwise we now work backwards through the string, testing for matches with the largest token possible
    // We revert the state if necessary, and then save the current state to match the changes accordingly
    } else {
        while(!token.empty()) {
            if(r.matches(token)) {
                unsigned int next_line = states.top().getLine() + (int) columns.size() - 1;
                unsigned int next_column = (columns.size() == 1) ? states.top().getColumn() + columns.top() : columns.top();
                states.top() = ScanState(cursor, next_line, next_column);
                clearDelimiterContent();
                return token;
            } else {
                token.pop_back();
                input.seekg(--cursor);
                if(columns.top() == 1) {
                    columns.pop();
                } else {
                    columns.top() -= 1;
                }
            }
        }
    }

    // Could not find a match, so return error
    std::string message = "Could not match token " + token_copy + " with Regex";
    throw ScanException(message, states.top());
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
        throw ScanException("Could not extract line", states.top());
    }

    states.top().advance('\n');
    buffer = rtrim(buffer);
    clearDelimiterContent();
    return buffer;
}

/**
 * Read Until
 * ================================
 *
 * The following is a convenience function used to read in characters until encountering
 * the delimiter passed or EOF encountered. Note that escaped characters that would otherwise
 * match the delimiter are ignored.
 */
std::string Scanner::readUntil(char delim)
{
    // Build up buffer until end
    std::string buffer;
    while(input.peek() != EOF && input.peek() != delim) {
        buffer += input.get();
        states.top().advance(buffer.back());
        if(buffer.back() == '\\' && input.peek() == delim) {
            buffer.back() = (char) input.get();
            states.top().advance(buffer.back());
        }
    }

    // Read in delimiter
    if(input.peek() != EOF) {
        buffer += input.get();
        states.top().advance(buffer.back());
    }

    clearDelimiterContent();
    return buffer;
}

/**
 * Read
 * ================================
 *
 * Checks if a character exists in the remainder of the stream.
 * And reads in this character if this is the case.
 */
char Scanner::read()
{
    char buffer[1];
    input.read(buffer, 1);
    states.top().advance(buffer[0]);
    clearDelimiterContent();
    return buffer[0];
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
 * Checkpoint Methods
 * ================================
 */
void Scanner::saveCheckpoint()
{
    states.push(states.top());
    states.top().cursor = input.tellg();
}

ScanState Scanner::restoreCheckpoint()
{
    ScanState top = states.top();
    states.pop();
    input.seekg(states.top().cursor);
    return top;
}

ScanState Scanner::getCurrentState() const
{
    return states.top();
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
        states.top().advance(separator.back());
    }
}