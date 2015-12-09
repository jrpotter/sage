/**
 * pparser.cpp
 *
 * Created by jrpotter (12/05/2015).
 */

#include "pparser.h"

using namespace sage;

/**
 * Exception Constructor
 * ================================
 */
PEGException::PEGException(std::string response)
    : response(response)
{ }

/**
 * Exception Message
 * ================================
 */
const char* PEGException::what() const noexcept
{
    return response.c_str();
}

/**
 * Constructor
 * ================================
 */
PParser::PParser(std::string filename)
    : stream(filename, std::ifstream::in)
    , input(stream)
{
    if(stream.is_open()) {
        parse();
    }
}

/**
 * Destructor
 * ================================
 */
PParser::~PParser()
{
    if(stream.is_open()) {
        stream.close();
    }
}

/**
 * Parsing
 * ================================
 *
 * The following reads in the PEG in the passed filename and converts the
 * grammar specification into an AST for traversal when parsing other files
 * according to said grammar.
 */
void PParser::parse()
{
    // These regexes are used to read in a custom PEG grammer.
    // Refer to /grammars/arithmetic.peg for a more thorough explanation
    // on the grammar. Note all other terms can be manipulated just
    // by reading in the remainder of a line or reading in words.
    static Regex arrowOperator("\\->");

    // To begin with, we load in all nonterminals and construct the
    // corresponding ASTs, as well as maintain the definitions
    // corresponding to each nonterminal.
    std::map<std::string, std::string> prototypes;
    while(!input.eof()) {

        // Reads in a comment and clears out line
        if(input.peekChar() == PPARSER_COMMENT) {
            input.readLine();

        // Reads in a PEG definition
        } else {

            // Read in the nonterminal, and check for a starting indicator
            std::string nonterminal = input.nextWord();
            if(input.peekChar() == PPARSER_START) {
                if(!start.empty()) {
                    throw PEGException("Multiple starting nonterminals");
                } else {
                    start = nonterminal;
                }
            }

            // Read in the next set of expressions
            input.next(arrowOperator);

            // Lastly, read through the given definition and break up.
            // The outer vector refers to the different options corresponding to
            // a definition (separated via the '|' operator) while each nested
            // vector refers to the actual...
            table[nonterminal] = std::vector();
        }
    }

    // Must have a starting nonterminal
    if(start.empty()) {
        throw PEGException("No starting nonterminal specified.");
    }
}