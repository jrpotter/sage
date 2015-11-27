/**
 * regex.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include <algorithm>
#include <memory>

#include "memory.h"
#include "regex.h"

using namespace sage;

/**
 * Constructor.
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
 * Copy Constructor.
 */
Regex::Regex(const Regex& other)
    :expr(other.expr)
{

}

/**
 * Move Constructor.
 */
Regex::Regex(Regex&& other)
{
    swap(*this, other);
}

/**
 * Assignment Operator.
 */
Regex& Regex::operator= (Regex other)
{
    swap(*this, other);
    return *this;
}

/**
 * Swap Operator.
 */
void Regex::swap(Regex& a, Regex &b)
{
    using std::swap;
    swap(a.expr, b.expr);
    swap(a.automaton, b.automaton);
}

/**
 * Begin building an NFA.
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

    // Join all NFAs together
    // Note we push back after polling the front so as not to create
    // too deep of a epsilon tree. That is, we are trying to balance
    // out the number of epsilon edges as much as possible.
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
 * Reads in Range.
 *
 * The following will construct a series of options as specified by the
 * range provided. Note this does not require just a single character.
 * If one would like, they could specify something like [ab-ae] to represent
 * 'ab', 'ac', 'ad', and 'ae'.
 */
std::shared_ptr<NFA> Regex::readRange(std::stringstream& ss)
{
    // Build range together. Note the following implementation technically
    // allows chaining of hyphenated values (e.g. 100-300-500) but this
    // functionality is maintained for simplicity sake.
    // TODO: Raise syntactic error for chaining


    // Build ranged NFA
    return std::make_shared<NFA>('a', 'b');
}