/**
 * Terminal.cpp
 *
 * Created by jrpotter (12/16/2015).
 */

#include "PEG/Terminal.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
Terminal::Terminal(std::string expr)
    : expr(expr)
{ }

/**
 * Processing
 * ================================
 *
 * We attempt to try and read the regex if possible. If not, then an error must have occurred.
 * Note we do not need the symbol_table; that is merely included to make the class concrete.
 */
std::shared_ptr<AST> Terminal::process(Scanner& s, const symbol_table&)
{
    try {
        return std::make_shared<AST>(s.next(expr));
    } catch(ScanException) {
        return nullptr;
    }
}
