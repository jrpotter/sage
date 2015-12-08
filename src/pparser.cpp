/**
 * pparser.cpp
 *
 * Created by jrpotter (12/05/2015).
 */

#include "pparser.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
PParser::PParser(std::string filename)
    : stream(filename, std::ifstream::in)
    , input(stream)
    , arrowOperator("\\->")
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
    while(!input.eof()) {

        // Reads in a comment and clears out line
        if(input.peekChar() == PPARSER_COMMENT) {
            input.readLine();

        // Read in nonterminal and regex
        } else {

            // Get nonterminal and check if its the starting word
            std::string nonterminal = input.nextWord();
            if(input.peekChar() == PPARSER_START) {
                if(!start.empty()) {
                    // TODO: Throw error on invalid usage of "'"
                } else {
                    start = nonterminal;
                }
            }

            // Read in the next set of expressions
            input.next(arrowOperator);
            std::string definition = input.readLine();
        }


    }
}