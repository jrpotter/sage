/**
 * regex.h
 *
 * The following is the main regular expression interface, used to verify
 * if strings match a given regex. Underneath the class builds an NFA and
 * converts it to a corresponding DFA which it traverses to provide
 * matching functionality.
 *
 * Special keywords are also provided as follows:
 * - \s: Whitespace ([ \t\v\r\n])
 * - \d: Digits ([0-9])
 * - \a: Lowercase Characters ([a-z])
 * - \A: Uppercase Characters ([A-Z])
 * - \w: Alphanumeric Characters ([a-zA-Z0-9])
 *
 * The following also provides an exception class providing details on any invalid regexes.
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_REGEX_H
#define SAGE_REGEX_H

#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <exception>

#include "dfa.h"

namespace sage
{
    class InvalidRegularExpression : public std::exception
    {
        public:
            InvalidRegularExpression(std::string, long);
            virtual const char* what() const;

        private:
            std::string message;
            long index;
    };

    class Regex
    {
        public:

            // Constructors
            Regex(std::string);
            ~Regex()=default;
            Regex(const Regex&);
            Regex(Regex&&);
            Regex& operator= (Regex);
            void swap(Regex&, Regex&);

            // Basic operations
            int find(std::string);
            bool matches(std::string, int);

        private:

            // Reference Members
            std::string expr;
            std::unique_ptr<DFA> automaton;

            // Reads in a stream of characters and converts it to a corresponding NFA
            std::shared_ptr<NFA> read(std::stringstream&) const;

            // Reads in a range of characters (i.e. [-])
            std::shared_ptr<NFA> readRange(std::stringstream&) const;

            // Utility method to combine NFAs together
            const std::shared_ptr<NFA> collapseNFAs(std::list<std::shared_ptr<NFA>>&) const;
    };

}

#endif //SAGE_REGEX_H
