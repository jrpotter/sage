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
        options.push_back(std::make_shared<Sequence>(def));
    }
}

/**
 * Processing
 * ================================
 *
 * According to PEG specifications, the following will try to process
 * each choice until one is successful.
 */
bool Choices::process(Scanner& s)
{
    for(auto option : options) {
        if(option->process(s)) {
            return true;
        }
    }
    return false;
}