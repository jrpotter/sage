/**
 * definition.cpp
 *
 * Created by jrpotter (12/13/2015).
 */

#include "Parser/Definition.h"

using namespace sage;

/**
 * Default Constructor
 * ================================
 *
 * The default constructor is necessary for inclusion in the STL.
 */
Definition::Definition()
    : tag(TAG_NONE)
{ }

/**
 * Scanner Constructor
 * ================================
 *
 * The following constructor builds the definition by parsing the text referenced within the
 * scanner. This involves recursively building up all the necessary tokens, sub-definitions,
 * repeat counts, etc.
 */
Definition::Definition(Scanner& definition)
    : Definition(TAG_CHOICES)
{
    // Used to determine if we should read in the character from input
    Regex letter = Regex::fromPool(REGEX_POOL_LETTER, REGEX_EXPR_LETTER);

    // Recursively build up choices
    while(definition.peek() != EOF) {

        // We read in the next character if it doesn't belong to a nonterminal
        // This is important so reading in the next word when scanning doesn't
        // skip over any content (in the case of single letter nonterminals)
        char next = definition.peek();
        if(!letter.matches(std::string(static_cast<unsigned long>(next), 1))) {
            next = definition.read();
        }

        // At this point, just process @next like normal
        switch(next) {

            // Read in a regular expression
            case PPARSER_TERMINAL_DELIM: {
                std::string term = definition.readUntil(PPARSER_TERMINAL_DELIM);
                term.pop_back(); // Remove the trailing delimiter character
                order.back()->order.emplace_back(std::make_shared<Definition>(term, TAG_TERMINAL));
                break;
            }

            // Move to next sequence if possible
            case PPARSER_CHOOSE: {
                auto def = std::make_shared<Definition>(TAG_SEQUENCE);
                order.push_back(def);
                break;
            }

            // Recursively build up the next choice
            case PPARSER_SUB_START: {
                auto def = std::make_shared<Definition>(definition);
                order.back()->order.push_back(def);
                break;
            }

            // Break out of next choice
            case PPARSER_SUB_END: {

                return;
            }

            // Repeat last expression 0 or more times
            case PPARSER_KLEENE_STAR: {
                order.back()->repeat = REPEAT_KLEENE_STAR;
                break;
            }

            // Repeat last expression 1 or more times
            case PPARSER_KLEENE_PLUS: {
                order.back()->repeat = REPEAT_KLEENE_PLUS;
                break;
            }

            // Either include the last expression or don't
            case PPARSER_KLEENE_OPTIONAL: {
                order.back()->repeat = REPEAT_OPTIONAL;
                break;
            }

            // Add nonterminal
            default: {
                auto def = std::make_shared<Definition>(definition.nextWord(), TAG_NONTERMINAL);
                order.back()->order.push_back(def);
                break;
            }

        }
    }
}

/**
 * Scanner Constructor Forwarding
 * ================================
 *
 * The first constructor is merely a forwarding method intended to pass in a temporary
 * scanner by reference.
 */
Definition::Definition(Scanner&& definition)
        : Definition(definition)
{ }

/**
 * Direct Constructor
 * ================================
 *
 * The following directly builds the corresponding token, according
 * to the @tag passed in. Now, for my own sake, and a reminder on how
 * unrestricted unions work, I'll write out my understanding on why
 * I need to use the placement new.
 *
 * In essence, placement new is taking in the passed address and setting
 * that as the "this" of the given object. I need to do this because, unless
 * I initialize the necessary variant from the beginning, all elements of
 * the given union will be undefined. As a result, trying to assign the
 * member of the element will not make sense, since the operator = is actually
 * a member of a given object.
 */
Definition::Definition(DEFINITION_TAG tag)
    : tag(tag)
    , repeat(REPEAT_NONE)
    , order({})
{
    // No choice should have an empty set of sequences
    if(tag == TAG_CHOICES) {
        order.emplace_back(std::make_shared<Definition>(TAG_SEQUENCE));
    }
}

Definition::Definition(std::string expr, DEFINITION_TAG tag)
    : tag(tag)
    , repeat(REPEAT_NONE)
{
    // Must apply placement new according to tag
    if(tag == TAG_TERMINAL) {
        new (&terminal) Regex(expr);
    } else if(tag == TAG_NONTERMINAL) {
        new (&nonterminal) std::string(expr);
    }
}

/**
 * Destructor
 * ================================
 */
Definition::~Definition()
{
    using namespace std;

    if(tag == TAG_TERMINAL) {
        terminal.~Regex();
    } else if(tag == TAG_NONTERMINAL) {
        nonterminal.~string();
    } else if(tag != TAG_NONE) {
        using namespace std;
        order.~vector<std::shared_ptr<Definition>>();
    }
}

/**
 * Process
 * ================================
 *
 * The following runs through a given stream (referenced internally by the passed scanner)
 * and constructs an AST if possible corresponding to the read content. The table used
 * will provide a mapping between nonterminals and further definitions (this should be
 * saved internally by the Parser class, which takes in a PEG grammar filename upon construction).
 */
bool Definition::process(Scanner& s, symbol_table& table)
{
    switch(tag) {
        case TAG_TERMINAL:
            return processTerminal(s);
        case TAG_NONTERMINAL:
            return processNonterminal(s, table);
        case TAG_SEQUENCE:
            return processSequence(s, table);
        case TAG_CHOICES:
            return processChoices(s, table);
        case TAG_NONE:
            return false;
    }
}

/**
 * Process Terminal
 * ================================
 *
 * Must run through input stream and look for a match if possible.
 */
bool Definition::processTerminal(Scanner& s)
{
    try {
        s.next(terminal);
        return true;
    } catch(std::invalid_argument) {
        return false;
    }
}

/**
 * Process Nonterminal
 * ================================
 *
 * Reference corresponding map and try to process instead
 */
bool Definition::processNonterminal(Scanner& s, symbol_table& table)
{
    if (table.find(nonterminal) != table.end()) {
        return table[nonterminal]->process(s, table);
    } else {
        return false;
    }
}

/**
 * Process Sequence
 * ================================
 *
 * We must run through each element in the sequence and attempt to process.
 * Note though that if one fails, we must revert back all of the elements.
 */
bool Definition::processSequence(Scanner& s, symbol_table& table)
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

/**
 * Process Choices
 * ================================
 *
 * According to PEG specifications, the following will try to process
 * each choice until one is successful.
 */
bool Definition::processChoices(Scanner& s, symbol_table& table)
{
    for(auto option : order) {
        if(option->process(s, table)) {
            return true;
        }
    }
    return false;
}
