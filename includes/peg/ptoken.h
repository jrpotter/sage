/**
 * ptoken.h
 *
 * Though I initially designed the PEG tokens via classes that inherited from
 * a single base class, this seemed overly burdensome for what should ultimately
 * be a relatively simple matter of processing. Instead, I use a tagged union
 * and handle "inheritance" via switches.
 *
 * Created by jrpotter (12/13/2015).
 */

#ifndef SAGE_PTOKEN_H
#define SAGE_PTOKEN_H

#include <memory>
#include <new>
#include <vector>

#include "parser/scanner.h"

namespace sage
{
    class PToken
    {
        public:

            // The following is a tagged union
            enum PTOKEN_TAG {
                TAG_TERMINAL,
                TAG_NONTERMINAL,
                TAG_SEQUENCE,
                TAG_CHOICES,
                TAG_NONE
            };

            // Used to represent repetition count
            enum E_REPEAT {
                REPEAT_KLEENE_STAR,
                REPEAT_KLEENE_PLUS,
                REPEAT_OPTIONAL,
                REPEAT_NONE
            } repeat;

            // Constructors
            PToken();
            PToken(Scanner&);
            PToken(PTOKEN_TAG);
            PToken(std::string, PTOKEN_TAG);
            ~PToken();

            // The following will run through and attempt to read in from
            // the scanner the corresponding definition if possible. It
            // will return whether or not it was successful.
            bool process(Scanner&, std::map<std::string, PToken>&);

        private:

            // The following contains the potential types of a ptoken
            // and designates how the processing of each should be conducted.
            // Note that @order is used for both choices and sequences
            PTOKEN_TAG tag;
            union {
                Regex terminal;
                std::string nonterminal;
                std::vector<std::shared_ptr<PToken>> order;
            };
    };
}

#endif //SAGE_PTOKEN_H
