//
// Created by jrpotter on 12/13/15.
//

#include "peg/sequence.h"
#include <iostream>

using namespace sage;

/**
 * Constructor
 * ================================
 *
 * Must read in the corresponding values and build up sequence.
 */
Sequence::Sequence(std::string definition)
{
    std::stringstream ss(definition);
    Scanner s(ss);

    while(s.peek() != EOF) {

        switch(s.peek()) {

            // Encountered a regex; read until delimiter encountered
            case PPARSER_TERMINAL_DELIM: {
                s.nextChar();
                std::string term = s.readUntil(PPARSER_TERMINAL_DELIM);
                order.emplace_back(std::make_shared<Terminal>(term.substr(0, term.length() - 1)));
                break;
            }

            // Encountered a subexpression
            case PPARSER_SUB_START: {
                s.nextChar();
                std::string sub = s.readUntil(PPARSER_SUB_END);
                order.emplace_back(std::make_shared<Choices>(sub.substr(0, sub.length() - 1)));
                break;
            }

            // Encountered a nonterminal
            default: {
                std::string nonterminal = s.nextWord();
                order.emplace_back(std::make_shared<Nonterminal>(nonterminal));
                break;
            }

        }

        // Determine repetition if possible
        if(s.peek() != EOF) {
            switch(s.peek()) {
                case PPARSER_KLEENE_STAR:
                    s.nextChar();
                    order.back()->repeat = REPEAT_KLEENE_STAR;
                    break;
                case PPARSER_KLEENE_PLUS:
                    s.nextChar();
                    order.back()->repeat = REPEAT_KLEENE_PLUS;
                    break;
                case PPARSER_KLEENE_OPTIONAL:
                    s.nextChar();
                    order.back()->repeat = REPEAT_OPTIONAL;
                    break;
                default:
                    break;
            }
        }

    }
}

/**
 * Process
 * ================================
 *
 * We must run through each element in the sequence and attempt to process.
 * Note though that if one fails, we must revert back all of the elements.
 */
bool Sequence::process(Scanner& s, std::map<std::string, std::shared_ptr<Definition>>& table)
{
    s.saveCheckpoint();
    for(auto seq : order) {
        if(!seq->process(s, table)) {
            s.restoreCheckpoint();
            return false;
        }
    }

    return true;
}
