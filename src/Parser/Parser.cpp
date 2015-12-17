/**
 * parser.cpp
 *
 * Created by jrpotter (12/05/2015).
 */

#include "Parser/Parser.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
Parser::Parser(std::string filename)
    : init_stream(filename, std::ifstream::in)
{
    if(init_stream.is_open()) {
        Scanner input(init_stream);
        initializeTable(input);
    }
}

/**
 * Destructor
 * ================================
 */
Parser::~Parser()
{
    if(init_stream.is_open()) {
        init_stream.close();
    }
}

/**
 * Parsing
 * ================================
 *
 * Jumpstarts the parsing method by initiating parsing from the starting
 * nonterminal specified in the *.peg grammar.
 */
std::shared_ptr<AST> Parser::parse(std::istream& input)
{
    // Begin parsing
    Scanner wrapper(input);
    auto result = table[start]->parse(wrapper, table);

    // We must go through the entirety of the input stream for me to regard
    // the above as a successful parse. Otherwise, return failure
    return (input.peek() == EOF) ? result : nullptr;
}

/**
 * Initialize Table
 * ================================
 *
 * The following reads in the PEG in the passed filename and converts the
 * grammar specification into a set of tokens for traversal when parsing other files
 * according to said grammar.
 */
void Parser::initializeTable(Scanner& input)
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
            if(nonterminal.back() == PPARSER_START) {
                nonterminal.pop_back();
                if(start.empty()) {
                    start = nonterminal;
                } else {
                    throw InvalidGrammar("Multiple starting nonterminals", input.getCurrentState());
                }
            }

            // Can now read in the arrow operator
            input.next(arrowOperator);

            // Rest of line is dedicated to definition
            std::stringstream ss(input.readLine());
            table[nonterminal] = std::make_shared<Choices>(Scanner(ss));
        }
    }

    // Must have at least one starting nonterminal
    if(start.empty()) {
        throw InvalidGrammar("No starting nonterminal specified");
    }

}
