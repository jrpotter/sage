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
Sequence::Sequence(Scanner& definition)
{
    bool finished_sub = false;
    while(definition.peek() != EOF && !finished_sub) {

        switch(definition.peek()) {

            // Encountered a regex; read until delimiter encountered
            case PPARSER_TERMINAL_DELIM: {
                definition.read();
                std::string term = definition.readUntil(PPARSER_TERMINAL_DELIM);
                order.emplace_back(std::make_shared<Terminal>(term.substr(0, term.length() - 1)));
                break;
            }

            // Encountered the end of a subexpression; just exit
            case PPARSER_SUB_END:
                definition.read();
                finished_sub = true;
                continue;

            // Encountered a subexpression
            case PPARSER_SUB_START: {
                definition.read();
                order.emplace_back(std::make_shared<Sequence>(definition));
                break;
            }

            // Encountered a nonterminal
            default: {
                std::string nonterminal = definition.nextWord();
                order.emplace_back(std::make_shared<Nonterminal>(nonterminal));
                break;
            }

        }

        // Determine repetition if possible
        if(definition.peek() != EOF) {
            definition.saveCheckpoint();
            switch(definition.read()) {
                case PPARSER_KLEENE_STAR:
                    order.back()->repeat = REPEAT_KLEENE_STAR;
                    break;
                case PPARSER_KLEENE_PLUS:
                    order.back()->repeat = REPEAT_KLEENE_PLUS;
                    break;
                case PPARSER_KLEENE_OPTIONAL:
                    order.back()->repeat = REPEAT_OPTIONAL;
                    break;
                default:
                    definition.restoreCheckpoint();
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
