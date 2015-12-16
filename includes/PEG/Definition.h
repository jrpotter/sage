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
    // Indicates how often a definition should be repeated.
    // This mirrors the operators present in a regular expression.
    enum DEFINITION_REPEAT
    {
        REPEAT_KLEENE_STAR,     // Indicates a definition can repeat 0 or more times
        REPEAT_KLEENE_PLUS,     // Indicates a definition can repeat 1 or more times
        REPEAT_OPTIONAL,        // Indicates a definition can occur 0 or 1 times
        REPEAT_NONE             // Indicates that a definition should occur only once
    };

    // Convenience typedef to map nonterminals to their definitions
    class Definition;
    using symbol_table = std::map<std::string, std::shared_ptr<Definition>>;

    // Base class used for parsing input
    class Definition
    {
        public:
            Definition();
            virtual ~Definition() = default;

            // Processing is the actual act of reading in (from the input stream
            // referenced in the passed scanner) and return an AST corresponding
            // to said stream.
            virtual std::shared_ptr<AST> process(Scanner&, const symbol_table&) = 0;

            // We make this publicly accessible since, during the reading in of the
            // *.peg file, we need to modify the operators for each definition anyways
            DEFINITION_REPEAT repeat_operator;
    };
}

#endif //SAGE_DEFINITION_H
