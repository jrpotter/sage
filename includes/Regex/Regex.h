/**
 * regex.h
 *
 * The following is the main, greedy regular expression interface, used to verify
 * if strings match a given regex. Underneath the class builds an NFA and converts it
 * to a corresponding DFA which it traverses to provide matching functionality.
 *
 * Special keywords are also provided as follows:
 * - \b: Word boundary (whitespace or EOF)
 * - \s: Whitespace ([ \t\v\r\n])
 * - \d: Digits ([0-9])
 * - \a: Lowercase Characters ([a-z])
 * - \U: Uppercase Characters ([A-Z])
 * - \A: Alphabetical Characters ([a-zA-Z])
 * - \w: Alphanumeric Characters ([a-zA-Z0-9])
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_REGEX_H
#define SAGE_REGEX_H

#include <algorithm>
#include <list>
#include <limits>
#include <sstream>

#include "macro.h"

#include "DFA.h"
#include "InvalidRegex.h"

namespace sage
{
    class Regex
    {
        public:

            // Constructors
            Regex() = default;
            Regex(std::string);

            // Other Constructors
            virtual ~Regex() = default;
            Regex(const Regex&);
            Regex(Regex&&);
            Regex& operator= (Regex);
            void swap(Regex&, Regex&);

            // Basic operations
            int find(std::string);
            bool matches(std::string, int=0);

            // Regex operations
            bool getFrontWordBounded() const;
            bool getBackWordBounded() const;

            // The following enables reuse of regexes over time by saving
            // the regexes in a map for return later on.
            static Regex& fromPool(std::string, std::string, int = 0);

        private:

            // Indicates the regex should be aligned on a word.
            bool front_word_bounded;
            bool back_word_bounded;

            // Reference Members
            std::string expr;
            std::shared_ptr<DFA> automaton;

            // Reads in a stream of characters and converts it to a corresponding NFA
            std::shared_ptr<NFA> read(std::stringstream&, int=0);

            // Reads in a range of characters (i.e. [-])
            std::shared_ptr<NFA> readRange(std::stringstream&);

            // Reads in special values (those following a '\')
            std::shared_ptr<NFA> readSpecial(std::stringstream&);

            // Utility method to combine NFAs together
            const std::shared_ptr<NFA> collapseNFAs(std::list<std::shared_ptr<NFA>>&) const;
    };

}

#endif //SAGE_REGEX_H
