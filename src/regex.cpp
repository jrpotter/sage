/**
 * regex.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "regex.h"

using namespace sage;

/**
 * Constructor
 * ================================
 *
 * Constructs an NFA out of the given expression, and then converts it to a DFA
 * which is stored for later matching.
 */
Regex::Regex(std::string expr)
    :expr(expr)
    ,automaton(nullptr)
{
    std::stringstream ss(expr);
    std::shared_ptr<NFA> nfa = read(ss);
    automaton = sage::make_unique<DFA>(nfa);
}


/**
 * Copy Constructor
 * ================================
 */
Regex::Regex(const Regex& other)
    :expr(other.expr)
{ }


/**
 * Move Constructor
 * ================================
 */
Regex::Regex(Regex&& other)
{
    swap(*this, other);
}


/**
 * Assignment Operator
 * ================================
 */
Regex& Regex::operator= (Regex other)
{
    swap(*this, other);
    return *this;
}


/**
 * Swap Operator
 * ================================
 */
void Regex::swap(Regex& a, Regex &b)
{
    using std::swap;
    swap(a.expr, b.expr);
    swap(a.automaton, b.automaton);
}


/**
 * Find
 * ================================
 *
 * Returns the index of the string in which the substring starting
 * at the specified index matches.
 */
int Regex::find(std::string search)
{
    for(int i = 0; i < search.size(); i++) {
        if(matches(search, i)) {
            return i;
        }
    }
}


/**
 * Matches
 * ================================
 *
 * Determines if the string at the given index matches correctly.
 */
bool Regex::matches(std::string search, int index)
{
    automaton->reset();
    for(int i = index; i < search.size(); i++) {
        if(!automaton->traverse(search[i])) {
            return false;
        }
    }
    return automaton->final();
}


/**
 * Collapse NFAs
 * ================================
 *
 * Note we push back after polling the front so as not to create
 * too deep of a epsilon tree. That is, we are trying to balance
 * out the number of epsilon edges as much as possible
 */
std::shared_ptr<NFA> Regex::collapseNFAs(std::list<std::shared_ptr<NFA>>& components)
{
    while(components.size() > 1) {
        auto first = components.front();
        components.pop_front();
        auto second = components.front();
        components.pop_front();
        first->join(second);
        components.push_back(first);
    }

    return components.front();
}


/**
 * Begin building an NFA
 * ================================
 *
 * This function will parse in the stream of characters and
 * build the corresponding NFA. We build up the NFA in parts
 * (as indicated by '|') to then be joined together.
 */
std::shared_ptr<NFA> Regex::read(std::stringstream& ss)
{
    // Start the NFA to be built
    std::list<std::shared_ptr<NFA>> components;
    components.emplace_back(std::make_shared<NFA>());

    // Begin processing input
    char c;
    while(ss.get(c) && c != ')') {

        std::shared_ptr<NFA> next = nullptr;

        // Begin processing node
        switch(c) {
            case '|':
                components.emplace_back(std::make_shared<NFA>());
                continue;
            case '(':
                next = read(ss);
                break;
            case '[':
                next = readRange(ss);
                break;
            case '\\':
                next = std::make_shared<NFA>(ss.get());
                break;
            default:
                next = std::make_shared<NFA>(c);
                break;
        }

        // Allow for repetition operations
        if(ss.peek() == '*' || ss.peek() == '+' || ss.peek() == '?') {
            ss.get(c);
            switch(c) {
                case '*':
                    next->kleeneStar();
                    break;
                case '+':
                    next->kleenePlus();
                    break;
                case '?':
                    next->makeOptional();
                    break;
            }
        }

        // Continue building up our NFA
        if(next != nullptr) {
            components.back()->concatenate(next);
        }
    }

    return collapseNFAs(components);
}


/**
 * Reads in Range.
 * ================================
 *
 * The following will construct a series of options as specified by the
 * range provided. Note this does not require just a single character.
 * If one would like, they could specify something like [ab-ae] to represent
 * 'ab', 'ac', 'ad', and 'ae'.
 */
std::shared_ptr<NFA> Regex::readRange(std::stringstream& ss)
{
    std::list<std::shared_ptr<NFA>> components;
    components.emplace_back(std::make_shared<NFA>());

    // Build range together. Note any values not hyphenated across
    // are optional values. That is, they may instead be used to
    // represent the sub-NFA. For example, [15-8a] means either
    // '1', '5', '6', '7', '8', or 'a' matches the given NFA
    char begin;
    while(ss.get(begin) && begin != ']') {
        if(ss.peek() == '-') {
            char end; ss.get(); ss.get(end);
            components.emplace_back(std::make_shared<NFA>(begin, end));
        } else {
            components.emplace_back(std::make_shared<NFA>(begin));
        }
    }

    return collapseNFAs(components);
}