/**
 * definition.cpp
 *
 * A definition serves as the abstract base class of the different components of a definition
 * in a PEG (parsing expression grammar), used in the symbols table when the parser parses the
 * .peg file passed to it.
 *
 * Created by jrpotter (12/16/2015).
 */

#include "PEG/Definition.h"

using namespace sage;

/**
 * Default Constructor
 * ================================
 */
Definition::Definition()
    : repeat_operator(REPEAT_NONE)
{ }

/**
 * Parsing
 * ================================
 *
 * The following performs the parsing of the stream referenced within the scanner.
 * It does so by managing the number of times the process method should be called,
 * according to the repetition tag assigned to the given definition.
 *
 * As a reminder, an empty AST is valid. A nullptr indicates failure in parsing.
 */
std::shared_ptr<AST> Definition::parse(Scanner& s, const symbol_table& table) {
    switch (repeat_operator) {
        case REPEAT_KLEENE_STAR:
            return parseKleeneStar(s, table);
        case REPEAT_KLEENE_PLUS:
            return parseKleenePlus(s, table);
        case REPEAT_OPTIONAL:
            return parseOptional(s, table);
        case REPEAT_NONE:
            return parseForced(s, table);
    }
}

/**
 * Parsing Kleene Star
 * ================================
 *
 * Repeat 0 or more times. If we cannot match, this is a perfectly valid scan
 * and we simply return an empty AST tree.
 */
std::shared_ptr<AST> Definition::parseKleeneStar(Scanner& s, const symbol_table& table)
{
    std::vector<std::shared_ptr<AST>> nodes;
    while(auto result = process(s, table)) {
        nodes.push_back(result);
    }

    if(nodes.empty()) {
        return std::make_shared<AST>();
    } else if(nodes.size() == 1) {
        return nodes[0];
    } else {
        return std::make_shared<AST>(nodes);
    }
}

/**
 * Parsing Kleene Plus
 * ================================
 *
 * Repeat 1 or more times. If we cannot match once then this is regarded as an
 * error and we return a nullptr.
 */
std::shared_ptr<AST> Definition::parseKleenePlus(Scanner& s, const symbol_table& table)
{
    std::vector<std::shared_ptr<AST>> nodes;
    while(auto result = process(s, table)) {
        nodes.push_back(result);
    }

    if(nodes.empty()) {
        return nullptr;
    } else if(nodes.size() == 1) {
        return nodes[0];
    } else {
        return std::make_shared<AST>(nodes);
    }
}

/**
 * Parsing Optional
 * ================================
 *
 * Either parse or don't. If we cannot match, this is a perfectly valid scan
 * and we simply return an empty AST tree.
 */
std::shared_ptr<AST> Definition::parseOptional(Scanner& s, const symbol_table& table)
{
    auto result = process(s, table);
    return (result) ? result : std::make_shared<AST>();
}

/**
 * Parsing Forced
 * ================================
 *
 * Must parse. If we cannot match once then this is regarded as an
 * error and we return a nullptr.
 */
std::shared_ptr<AST> Definition::parseForced(Scanner& s, const symbol_table& table)
{
    auto result = process(s, table);
    return (result) ? result : nullptr;
}