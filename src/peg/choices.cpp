/**
 * choices.cpp
 *
 * Created by jrpotter (12/13/2015).
 */

#include "peg/choices.h"

using namespace sage;

/**
 * Constructor
 * ================================
 *
 * The following performs the necessary splitting of the definition
 * into corresponding choices.
 */
Choices::Choices(std::string definition)
{
    auto defs = split(trim(definition), "||");
    for(auto def : defs) {
        std::stringstream tmp(def);
        Scanner tmp_scanner(tmp);
        options.push_back(std::make_shared<Sequence>(tmp_scanner));
    }
}

/**
 * Processing
 * ================================
 *
 * According to PEG specifications, the following will try to process
 * each choice until one is successful.
 */
bool Choices::process(Scanner& s, std::map<std::string, std::shared_ptr<Definition>>& table)
{
    for(auto option : options) {
        if(option->process(s, table)) {
            return true;
        }
    }
    return false;
}