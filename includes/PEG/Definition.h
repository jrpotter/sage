/**
 * Definition.h
 *
 * A definition serves as the abstract base class of the different components of a definition
 * in a PEG (parsing expression grammar), used in the symbols table when the parser parses the
 * .peg file passed to it.
 *
 * Created by jrpotter (12/16/2015).
 */

#ifndef SAGE_DEFINITION_H
#define SAGE_DEFINITION_H

#include <map>
#include <memory>
#include <string>

#include "Parser/AST.h"
#include "Parser/Scanner.h"

namespace sage
{
    // Convenience typedef to map nonterminals to their definitions
    class Definition;
    using symbol_table = std::map<std::string, std::shared_ptr<Definition>>;

    // Base class used for parsing input
    class Definition
    {
        public:
            Definition();
            virtual ~Definition() = default;

            // The following is the means by which parsing the stream (referenced from
            // within the input stream in the scanner) is handled. In particular, this
            // function manages the number of times processing should occur.
            std::shared_ptr<AST> parse(Scanner&, const symbol_table&);

            // Indicates how often a definition should be repeated. This mirrors the operators
            // present in a regular expression. We make this publicly accessible since, during the
            // reading in of the *.peg file, we need to modify the operators for each definition anyways
            enum DEFINITION_REPEAT
            {
                REPEAT_KLEENE_STAR,     // Indicates a definition can repeat 0 or more times
                REPEAT_KLEENE_PLUS,     // Indicates a definition can repeat 1 or more times
                REPEAT_OPTIONAL,        // Indicates a definition can occur 0 or 1 times
                REPEAT_NONE             // Indicates that a definition should occur only once
            } repeat_operator;

        protected:

            // Processing is the act of parsing once according to a given definition
            virtual std::shared_ptr<AST> process(Scanner&, const symbol_table&) = 0;

        private:

            // Utility methods for code cleanliness
            std::shared_ptr<AST> parseKleeneStar(Scanner&, const symbol_table&);
            std::shared_ptr<AST> parseKleenePlus(Scanner&, const symbol_table&);
            std::shared_ptr<AST> parseOptional(Scanner&, const symbol_table&);
            std::shared_ptr<AST> parseForced(Scanner&, const symbol_table&);

    };
}

#endif //SAGE_DEFINITION_H
