/**
 * token.cpp
 *
 * Created by jrpotter (11/26/2015).
 */

#include "token.h"

using namespace sage;

/**
 * Constructor.
*/
Token::Token(std::string value, TokenType type)
      :value(value)
      ,type(type)
{

}

/**
 * Copy Constructor.
 */
Token::Token(const Token& other)
      :value(other.value)
      ,type(other.type)
{

}

/**
 * Move Constructor.
 */
Token::Token(Token&& other)
      :type(TokenType::EMPTY)
{
    swap(*this, other);
}

/**
 * Assignment Operator.
 */
Token& Token::operator= (Token other)
{
    swap(*this, other);
    return *this;
}

/**
 * Swap Operator.
*/
void Token::swap(Token& a, Token& b)
{
    using std::swap;
    swap(a.value, b.value);
    swap(a.type, b.type);
}