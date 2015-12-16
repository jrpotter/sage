/**
 * definition.h
 *
 * Though I initially designed the definitions via classes that inherited from
 * a single base class, this seemed overly burdensome for what should ultimately
 * be a relatively simple matter of processing. Instead, I use a tagged union
 * and handle "inheritance" via switches.
 *
 * The definition constructor will parse a definition provided in a PEG grammar
 * for later parsing text into an AST.
 *
 * Created by jrpotter (12/13/2015).
 */

#ifndef SAGE_DEFINITION_H
#define SAGE_DEFINITION_H

#include <memory>
#include <new>
#include <vector>

#include "Scanner.h"

namespace sage
{
    class Definition;
    using symbol_table = std::map<std::string, std::shared_ptr<Definition>>;

    class Definition
    {
        public:

            // The following is a tagged union
            enum DEFINITION_TAG {
                TAG_TERMINAL,           // Refers to a regular expression
                TAG_NONTERMINAL,        // Refers to another definition
                TAG_SEQUENCE,           // Refers to a sequence of regular expressions and/or definitions
                TAG_CHOICES,            // Refers to the different sequences one can try
                TAG_NONE                // Used for default constructor only
            };

            // Used to represent repetition count
            enum E_REPEAT {
                REPEAT_KLEENE_STAR,     // Indicates a definition can repeat 0 or more times
                REPEAT_KLEENE_PLUS,     // Indicates a definition can repeat 1 or more times
                REPEAT_OPTIONAL,        // Indicates a definition can occur 0 or 1 times
                REPEAT_NONE             // Indicates that a definition should occur only once
            } repeat;

            // Constructors
            Definition();
            Definition(Scanner&);
            Definition(Scanner&&);
            Definition(DEFINITION_TAG);
            Definition(std::string, DEFINITION_TAG);
            ~Definition();

            // The following will run through and attempt to read in from
            // the scanner the corresponding definition if possible. It
            // will return whether or not it was successful.
            bool process(Scanner&, symbol_table&);

        private:

            // The following contains the potential types of a definition
            // and designates how the processing of each should be conducted.
            // Note that @order is used for both choices and sequences
            DEFINITION_TAG tag;
            union {
                Regex terminal;
                std::string nonterminal;
                std::vector<std::shared_ptr<Definition>> order;
            };

            // Utility Methods (to clean up code a bit)
            bool processTerminal(Scanner&);
            bool processNonterminal(Scanner&, symbol_table&);
            bool processSequence(Scanner&, symbol_table&);
            bool processChoices(Scanner&, symbol_table&);
    };
}

#endif //SAGE_DEFINITION_H
