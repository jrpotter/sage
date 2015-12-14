//
// Created by jrpotter on 12/13/15.
//

#ifndef SAGE_MACRO_H
#define SAGE_MACRO_H

// Regex Operators
// These are used in the construction of regexes
#define REGEX_CHOOSE              '|'
#define REGEX_HYPHEN              '-'
#define REGEX_KLEENE_PLUS         '+'
#define REGEX_KLEENE_STAR         '*'
#define REGEX_OPTIONAL            '?'
#define REGEX_RANGE_END           ']'
#define REGEX_RANGE_START         '['
#define REGEX_REPL_START          '{'
#define REGEX_REPL_END            '}'
#define REGEX_SPECIAL             '\\'
#define REGEX_SUB_END             ')'
#define REGEX_SUB_START           '('
#define REGEX_WILDCARD            '.'

// Pool Keys
// Used to reference static keys in the regex pool
#define REGEX_POOL_CHAR           "char"
#define REGEX_POOL_FLOAT          "float"
#define REGEX_POOL_INTEGRAL       "integral"
#define REGEX_POOL_REPL           "repl"
#define REGEX_POOL_LETTER         "letter"
#define REGEX_POOL_WHITESPACE     "whitespace"
#define REGEX_POOL_WORD           "word"

// Preconstructed Expressions
// By preconstructed I do not mean I generate the regex for each of these expressions.
// This would prove much too heavy in terms of memory usage (the construction process
// of NFA to DFA, at least at the moment, is fairly hefty). Instead, these are
// strings that can be passed into the Regex constructor for simplicity sake.
#define REGEX_EXPR_CHAR           "."
#define REGEX_EXPR_FLOAT          "[+\\-]?(0|[1-9]\\d*)?(\\.\\d*)?"
#define REGEX_EXPR_INTEGRAL       "[+\\-]?(0|[1-9]\\d*)"
#define REGEX_EXPR_LETTER         "[\\a\\U]"
#define REGEX_EXPR_REPL           "{\\A+}"
#define REGEX_EXPR_WHITESPACE     "\\s+"
#define REGEX_EXPR_WORD           "\\A+"

// PEG Parser Definitions
// These are specific PParser characters used when parsing
#define PPARSER_CHOOSE            '|'
#define PPARSER_COMMENT           '#'
#define PPARSER_KLEENE_STAR       '*'
#define PPARSER_KLEENE_PLUS       '+'
#define PPARSER_KLEENE_OPTIONAL   '?'
#define PPARSER_START             '\''
#define PPARSER_SUB_START         '('
#define PPARSER_SUB_END           ')'
#define PPARSER_TERMINAL_DELIM    '"'

#endif //SAGE_MACRO_H
