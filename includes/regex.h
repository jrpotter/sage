/**
 * regex.h
 *
 * The following is the main, greedy regular expression interface, used to verify
 * if strings match a given regex. Underneath the class builds an NFA and converts it
 * to a corresponding DFA which it traverses to provide matching functionality.
 *
 * Special keywords are also provided as follows:
 * - \s: Whitespace ([ \t\v\r\n])
 * - \d: Digits ([0-9])
 * - \a: Lowercase Characters ([a-z])
 * - \U: Uppercase Characters ([A-Z])
 * - \A: Alphabetical Characters ([a-zA-Z])
 * - \w: Alphanumeric Characters ([a-zA-Z0-9])
 *
 * In addition, by bracketing words (i.e. '{' and '}') and passing a regex corresponding to
 * this parameterized word, we can place sub-regexes into our given code. For instance, I can
 * represent a character as r = Regex("[a-zA-Z]") and a word as w = Regex("{Character}+", r).
 *
 * The following also provides an exception class providing details on any invalid regexes.
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_REGEX_H
#define SAGE_REGEX_H

#include <list>
#include <limits>
#include <string>
#include <sstream>
#include <algorithm>
#include <exception>

#include "dfa.h"

#define REGEX_CHOOSE      '|'
#define REGEX_HYPHEN      '-'
#define REGEX_KLEENE_PLUS '+'
#define REGEX_KLEENE_STAR '*'
#define REGEX_OPTIONAL    '?'
#define REGEX_RANGE_END   ']'
#define REGEX_RANGE_START '['
#define REGEX_SPECIAL     '\\'
#define REGEX_SUB_END     ')'
#define REGEX_SUB_START   '('
#define REGEX_WILDCARD    '.'

namespace sage
{
    class InvalidRegularExpression : public std::exception
    {
        public:
            InvalidRegularExpression(std::string, char, long);
            virtual const char* what() const noexcept;

        private:
            std::string response;
    };

    class Regex
    {
        public:

            // Constructors
            template <typename... Args>
            Regex(std::string, Args...);

            // Basic operations
            int find(std::string);
            bool matches(std::string, int=0);

            // "Precompiled" Expressions
            // By precompiled I do not mean I generate the regex for each of these expressions.
            // This would prove much too heavy in terms of memory usage (the construction process
            // of NFA to DFA, at least at the moment, is fairly hefty). Instead, these are
            // strings that can be passed into the Regex constructor for simplicity sake.
            static std::string FLOAT;
            static std::string INTEGRAL;
            static std::string WHITESPACE;
            static std::string WORD;

        private:

            // Reference Members
            std::string expr;
            std::shared_ptr<DFA> automaton;

            // Reads in a stream of characters and converts it to a corresponding NFA
            std::shared_ptr<NFA> read(std::stringstream&, int=0) const;

            // Reads in a range of characters (i.e. [-])
            std::shared_ptr<NFA> readRange(std::stringstream&) const;

            // Reads in special values (those following a '\')
            std::shared_ptr<NFA> readSpecial(std::stringstream&) const;

            // Utility method to combine NFAs together
            const std::shared_ptr<NFA> collapseNFAs(std::list<std::shared_ptr<NFA>>&) const;
    };

}

#endif //SAGE_REGEX_H
