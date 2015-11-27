/**
 * token.h
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_TOKEN_H
#define SAGE_TOKEN_H

#include <string>

namespace sage {

    enum class TokenType {
        EMPTY,
        IDENTIFIER,
        APOSTROPHE,
        KEYWORD
    };

    class Token {

        public:
            Token(std::string, TokenType);
            Token(const Token&);
            Token(Token&&);
            Token& operator= (Token);
            void swap(Token&, Token&);

        private:
            std::string value;
            TokenType type;

    };

}

#endif //SAGE_TOKEN_H
