//
// Created by jrpotter on 12/16/15.
//

#include "PEG/Choices.h"

using namespace sage;

/**
 * Scanner Constructor
 * ================================
 *
 * Since Choices is the top-level of the definition hierarchy, it is also reponsible with performing the parsing
 * of the actual definition specification.
 */
Choices::Choices(Scanner& definition)
{
    // We must ensure there is always at least one sequence in place
    options.emplace_back(std::make_shared<Sequence>());

    // Used to determine if we should read in the character from input
    Regex letter = Regex::fromPool(REGEX_POOL_LETTER, REGEX_EXPR_LETTER);
    while(definition.peek() != EOF) {

        // We read in the next character if it doesn't belong to a nonterminal
        // This is important so reading in the next word when scanning doesn't
        // skip over any content (in the case of single letter nonterminals)
        char next = definition.peek();
        if(!letter.matches(std::string(1, next))) {
            next = definition.read();
        }

        switch(next) {

            // Read in a regular expression
            case PPARSER_TERMINAL_DELIM: {
                std::string term = definition.readUntil(PPARSER_TERMINAL_DELIM);
                term.pop_back(); // Remove the trailing delimiter character
                options.back()->append(std::make_shared<Terminal>(term));
                break;
            }

            // Move to next sequence now
            case PPARSER_CHOOSE: {
                options.emplace_back(std::make_shared<Sequence>());
                break;
            }

            // Recursively build up the next choice.
            // This is why we must pass the scanner by reference.
            case PPARSER_SUB_START: {
                options.back()->append(std::make_shared<Choices>(definition));
                break;
            }

            // Break out of current choice and go back up a level
            case PPARSER_SUB_END: {
                return;
            }

            // Repeat last expression 0 or more times
            case PPARSER_KLEENE_STAR:
            case PPARSER_KLEENE_PLUS:
            case PPARSER_KLEENE_OPTIONAL: {
                options.back()->setLastOperator(next);
                break;
            }

            // Add a nonterminal
            default: {
                options.back()->append(std::make_shared<Nonterminal>(definition.nextWord()));
                break;
            }

        }
    }
}

/**
 * Scanner Constructor Forwarding
 * ================================
 *
 * Delegates outward to pass by reference.
 */
Choices::Choices(Scanner&& definition)
    : Choices(definition)
{ }

/**
 * Processing
 * ================================
 *
 * Since any option is perfectly valid, so long as one of the options returns successful, we
 * deem the processing to be successful. This is done in a FCFS manner.
 */
std::shared_ptr<AST> Choices::process(Scanner& s, const symbol_table& table)
{
    for(auto option : options) {
        if(auto result = option->parse(s, table)) {
            return result;
        }
    }

    return nullptr;
}