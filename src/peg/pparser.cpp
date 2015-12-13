/**
 * pparser.cpp
 *
 * Created by jrpotter (12/05/2015).
 */

#include "peg/pparser.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
PParser::PParser(std::string filename)
    : stream(filename, std::ifstream::in)
{
    if(stream.is_open()) {
        Scanner input(stream);
        parse(input);
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
void PParser::parse(Scanner& input)
{
    // These regexes are used to read in a custom PEG grammar.
    // Refer to /grammars/arithmetic.peg for a more thorough explanation
    // on the grammar. Note all other terms can be manipulated just
    // by reading in the remainder of a line or reading in words.
    Regex arrowOperator = Regex::fromPool("pparser-arrow", "\\->");
    Regex markedWord = Regex::fromPool("pparser-marked-word", "\\A+'?");

    // On any given line, the first two terminals should be the nonterminal
    // being defined and the arrow operator or we've encountered a comment.
    while(input.peek() != EOF) {
        if(input.peek() == PPARSER_COMMENT) {
            input.readLine();
        } else {

            // First read in nonterminal and find start if possible
            std::string nonterminal = input.next(markedWord);
            if(nonterminal[nonterminal.length() - 1] == PPARSER_START) {
                if(start.empty()) {
                    start = nonterminal;
                } else {
                    throw PEGException("Multiple starting nonterminals", input.getLinePos(), input.getColumnPos());
                }
            }

            // Can now read in the arrow operator. Rest of line is dedicated to definition
            input.next(arrowOperator);
            table[nonterminal] = std::make_shared<Choices>(input.readLine());
        }
    }

    // Must have at least one starting nonterminal
    if(start.empty()) {
        throw PEGException("No starting nonterminal specified");
    }

}

/**
 * Definition Parsing
 * ================================
 *
 * The following will parse the passed in definition and construct the
 * corresponding possibilities
 */
std::shared_ptr<Definition> PParser::parseDefinition(std::string definition)
{

}
