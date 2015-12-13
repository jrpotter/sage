/**
 * pparser.cpp
 *
 * Created by jrpotter (12/05/2015).
 */

#include "pparser.h"
#include <iostream>

using namespace sage;

/**
 * Definition Constructor
 * ================================
 */
PParser::Definition::Definition(Regex r)
    : type(TAG_TERMINAL)
    , terminal(r)
{ }

PParser::Definition::Definition(std::string s)
    : type(TAG_NONTERMINAL)
    , nonterminal(s)
{ }

/**
 * Definition Copy Constructor
 * ================================
 */
PParser::Definition::Definition(const Definition &other)
    : type(other.type)
{
    switch(type) {
        case TAG_TERMINAL:
            terminal = other.terminal;
            break;
        case TAG_NONTERMINAL:
            nonterminal = other.nonterminal;
            break;
    }
}

/**
 * Definition Destructor
 * ================================
 */
PParser::Definition::~Definition()
{ }

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

    // To begin with, we load in all nonterminals and construct the
    // corresponding ASTs, as well as maintain the definitions
    // corresponding to each nonterminal.
    while(input.peek() != EOF) {

        // Reads in a comment and clears out line
        if(input.peek() == PPARSER_COMMENT) {
            input.readLine();

        // Reads in a PEG definition
        } else {

            // Read in the nonterminal, and check for a starting indicator
            std::string nonterminal = input.next(markedWord);
            if(nonterminal[nonterminal.length()-1] == PPARSER_START) {
                if(!start.empty()) {
                    throw PEGException("Multiple starting nonterminals");
                } else {
                    start = nonterminal;
                }
            }

            // Read in the next set of expressions
            input.next(arrowOperator);

            // Lastly, we read through the given definition and distinguish between
            // nonterminals (bracketed) or primitive regexes (anything other than a
            // nonterminal). For instance, "{Expression}" matches the regex as defined
            // by Expression but "Expression" tries to match a sequence of characters
            // that spell out "Expression."
            table[nonterminal] = parseDefinition(input.readLine());
        }
    }

    // Must have a starting nonterminal
    if(start.empty()) {
        throw PEGException("No starting nonterminal specified.");
    }
}

/**
 * Definition Parsing
 * ================================
 *
 * The following will parse the passed in definition and construct the
 * corresponding possibilities
 */
PParser::t_choice PParser::parseDefinition(std::string definition)
{
    // Convert to stream for ability to use scanner instead
    std::stringstream def_stream(definition);
    Scanner def_scanner(def_stream);

    // Read in all choices until we reach the end of file
    t_choice choices = {{}};
    while(def_scanner.peek() != EOF) {

        switch(def_scanner.peek()) {

            // Nonterminal; simply read in this expression
            case REGEX_REPL_START: {
                Regex repl = Regex::fromPool(REGEX_POOL_REPL, REGEX_EXPR_REPL);
                std::cout << def_scanner.next(repl);
                Definition tmp(def_scanner.next(repl));
                choices.back().push_back(tmp);
                break;
            }

            // Append a new choice to build from if two pipes
            case REGEX_CHOOSE: {
                if (def_scanner.peek(1) == REGEX_CHOOSE) {
                    Regex r = Regex::fromPool("peg_choice", "\\|\\|");
                    def_scanner.next(r);
                    break;
                }
                // FALL THROUGH
            }

            // Read in until whitespace (next token)
            default: {
                Regex r = Regex::fromPool(REGEX_POOL_WHITESPACE, REGEX_EXPR_WHITESPACE);
                std::string token;
                char c = def_scanner.nextChar();
                for(; c != EOF && !r.matches(std::string(1, c)); c = def_scanner.peek()) {
                    token += def_scanner.nextChar();
                }
                choices.back().push_back(Definition(Regex(token)));
                break;
            }
        }

    }

    return choices;
}
