/**
 * regex.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "regex.h"

using namespace sage;

/**
 * Pooling
 * ================================
 */
Regex& Regex::fromPool(std::string key, std::string expr, int i)
{
    static std::map<std::string, Regex> pool;

    // Search for the key and check for a match. Otherwise we repeat with a modified key and continue the search.
    // Note I add this somewhat convoluted means of continued search because our above map is ordered and I want 
    // to avoid any potential clustering
    auto it = pool.find(key);
    if(it != pool.end()) {
        Regex& r = it->second;
        if(r.expr != expr) {
            char prepend = static_cast<char>('a' + (expr.size() % 26));
            return fromPool(prepend + key, expr, (i + 17) % 26);
        }
        return r;
    }

    pool[key] = Regex(expr);
    return pool[key];
}

/**
 * Constructor
 * ================================
 *
 * Constructs an NFA out of the given expression, and then converts it to a DFA
 * which is stored for later matching. Bracketed expressions are handled in a manner
 * similar to format strings; that is, once a bracketed word has been found, this
 * word is mapped to the regex corresponding to the index of the bracketed word.
 * For example, if there is a regex "{a}{b}", then "a" is the name of the regex
 * at the 0th index of our vector, and "b" is the name of the regex at the 1st index.
 * If the same named regex is later found, it refers to the element already mapped
 * to and not the next indexed value.
 */
Regex::Regex(std::string expr)
{
    std::stringstream ss(expr);
    std::shared_ptr<NFA> nfa = read(ss);
    automaton = std::make_shared<DFA>(nfa);
}

/**
 * Copy Constructor
 * ================================
 */
Regex::Regex(const Regex& other)
    : expr(other.expr)
    , automaton(std::make_shared<DFA>(*other.automaton))
{ }

/**
 * Move Constructor
 * ================================
 */
Regex::Regex(Regex&& other)
    : Regex()
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
void Regex::swap(Regex& a, Regex& b)
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
const std::shared_ptr<NFA> Regex::collapseNFAs(std::list<std::shared_ptr<NFA>>& components) const
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
std::shared_ptr<NFA> Regex::read(std::stringstream& ss, int counter) const
{
    // Start the NFA to be built
    // This will continue to be expanded as '|'s are encountered
    std::list<std::shared_ptr<NFA>> components;
    components.emplace_back(std::make_shared<NFA>());

    // Begin processing input
    // Note that an empty regex is not considered an error. Rather
    // it is the regex that matches the empty string. Nonetheless,
    // caution must be taken when using this (if this is something
    // one would ever use...)
    char c;
    while(ss.get(c) && c != REGEX_SUB_END) {

        std::shared_ptr<NFA> next = nullptr;

        // Begin processing node
        switch(c) {
            case REGEX_CHOOSE:
                components.emplace_back(std::make_shared<NFA>());
                continue;
            case REGEX_RANGE_START:
                next = readRange(ss);
                break;
            case REGEX_SPECIAL:
                next = readSpecial(ss);
                break;
            case REGEX_SUB_START:
                next = read(ss, counter+1);
                break;
            case REGEX_HYPHEN:
            case REGEX_KLEENE_PLUS:
            case REGEX_KLEENE_STAR:
            case REGEX_OPTIONAL:
            case REGEX_RANGE_END:
                throw InvalidRegularExpression("Unexpected '%c'", c, ss.tellg());
            case REGEX_WILDCARD:
                next = std::make_shared<NFA>(0, std::numeric_limits<char>::max());
                break;
            default:
                next = std::make_shared<NFA>(c);
                break;
        }

        // Allow for repetition operations
        if(ss.peek() == REGEX_KLEENE_PLUS || ss.peek() == REGEX_KLEENE_STAR || ss.peek() == REGEX_OPTIONAL) {
            switch(ss.get()) {
                case REGEX_KLEENE_PLUS:
                    next->kleenePlus();
                    break;
                case REGEX_KLEENE_STAR:
                    next->kleeneStar();
                    break;
                case REGEX_OPTIONAL:
                    next->makeOptional();
                    break;
                default:
                    break;
            }
        }

        // Continue building up our NFA
        if(next != nullptr) {
            components.back()->concatenate(next);
        }
    }

    // If we finished reading in input even though there is still more to
    // read on the stream, we must have encountered an extra ')' and need
    // to report the error. Otherwise if we reached an end but we're not
    // back at the root call, then we have nested ourselves too far.
    if(counter == 0 && c == REGEX_SUB_END) {
        throw InvalidRegularExpression("Encountered extra '%c' character", REGEX_SUB_END, ss.tellg());
    } else if(counter == 1 && ss.peek() == EOF && c != REGEX_SUB_END) {
        throw InvalidRegularExpression("Encountered extra '%c' character", REGEX_SUB_START, ss.tellg());
    } else if(counter > 1 && ss.peek() == EOF) {
        throw InvalidRegularExpression("Encountered extra '%c' character", REGEX_SUB_START, ss.tellg());
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
 *
 * Note within a range it is valid to use any regex characters besides hyphens
 * as if they were regular characters. Prefixing them with a backslash will make
 * no difference. That being said, one can still use the special characters
 * (e.g. '\s').
 */
std::shared_ptr<NFA> Regex::readRange(std::stringstream& ss) const
{
    auto head = std::make_shared<NFA>();
    std::list<std::shared_ptr<NFA>> components;

    // Build range together. Note any values not hyphenated across
    // are optional values. That is, they may instead be used to
    // represent the sub-NFA. For example, [15-8a] means either
    // '1', '5', '6', '7', '8', or 'a' matches the given NFA
    char begin;
    while(ss.get(begin) && begin != REGEX_RANGE_END) {

        // Special characters are expanded to what they represent
        if(begin == REGEX_SPECIAL) {
            components.emplace_back(readSpecial(ss));

        // If we ever read in a hyphen not preceded by a character then an
        // invalid range has been specified. Ranges between characters are
        // perfectly valid, but it is important that they are actually in order
        } else if(begin != REGEX_HYPHEN) {
            if(ss.peek() == '-') {
                char end; ss.get();
                if(!ss.get(end)) {
                    throw InvalidRegularExpression("End range of '%c' not specified", REGEX_HYPHEN, ss.tellg());
                } else if(begin > end) {
                    throw InvalidRegularExpression("Range starting at '%c' not ordered correctly", begin, ss.tellg());
                } else {
                    components.emplace_back(std::make_shared<NFA>(begin, end));
                }
            } else {
                components.emplace_back(std::make_shared<NFA>(begin));
            }

        // Otherwise we encountered a hypher, but this should only occur after reading in a character
        } else {
            throw InvalidRegularExpression("Encountered non-paired '%c'", REGEX_HYPHEN, ss.tellg());
        }
    }

    // Did not encounter the end of the range
    if(begin != REGEX_RANGE_END) {
        throw InvalidRegularExpression("Expected '%c'", REGEX_RANGE_END, ss.tellg());
    }

    // Need to join all together
    // Note if our components is empty, we essentially have an empty range and thus anything matches
    // (because our initial head is a final state). This may not be desired behavior, but I'll just
    // settle on the idea that it is 'undefined' since what's an empty range mean anyways?
    if(components.size() > 0) {
        head->concatenate(collapseNFAs(components));
        head->concatenate(std::make_shared<NFA>());
    }

    return head;
}

/**
 * Reads Special
 * ================================
 *
 * Any unrecognized characters immediately throw an error. Unfortunately, because
 * '\' is used to escape in both C++ and the following regex, it is necessary to
 * pass '\\\\' in order to indicate a backslash in the regex.
 */
std::shared_ptr<NFA> Regex::readSpecial(std::stringstream& ss) const
{
    std::stringstream range;

    char c;
    if(ss.get(c)) {
        switch(c) {
            case 's': // Whitespace ([ \t\v\r\n])
                range << " \t\v\r\n]";
                break;
            case 'd': // Digits
                range << "0-9]";
                break;
            case 'a': // Lowercase Characters
                range << "a-z]";
                break;
            case 'U': // Uppercase Characters
                range << "A-Z]";
                break;
            case 'A': // Alphabetical Characters
                range << "a-zA-Z]";
                break;
            case 'w': // Alphanumeric Characters
                range << "a-zA-Z0-9]";
                break;
            case REGEX_CHOOSE:
            case REGEX_HYPHEN:
            case REGEX_KLEENE_PLUS:
            case REGEX_KLEENE_STAR:
            case REGEX_OPTIONAL:
            case REGEX_RANGE_END:
            case REGEX_RANGE_START:
            case REGEX_SPECIAL:
            case REGEX_SUB_END:
            case REGEX_SUB_START:
            case REGEX_WILDCARD:
                return std::make_shared<NFA>(c);
            default:
                throw InvalidRegularExpression("Unrecognized special character '%c'", c, ss.tellg());
        }
    } else {
        throw InvalidRegularExpression("Expected character after '%c'", REGEX_SPECIAL, ss.tellg());
    }

    return readRange(range);
}