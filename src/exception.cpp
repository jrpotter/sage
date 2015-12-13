//
// Created by jrpotter on 12/13/15.
//

#include "exception.h"

using namespace sage;

/**
 * PEGException Constructor
 * ================================
 */
PEGException::PEGException(std::string message)
    : message(message)
{ }

/**
 * PEGException What
 * ================================
 */
const char* PEGException::what() const noexcept
{
    return message.c_str();
}

/**
 * InvalidRegularExpression Constructor
 * ================================
 */
InvalidRegularExpression::InvalidRegularExpression(std::string message, char problem, long index)
{
    std::stringstream ss;

    // First format string
    char* buffer = new char[message.size()];
    int result = snprintf(buffer, sizeof(char) * message.size(), message.c_str(), problem);
    if(result < sizeof(char) * message.size()) {
        ss << buffer;
        if(index == EOF) {
            ss << " by end of expression.";
        } else {
            ss << " at position " << index << '.';
        }
        ss << std::endl;
    } else {
        ss << "An error occurred with Sage." << std::endl;
    }

    // Cleanup
    delete[] buffer;
    message = ss.str();
}

/**
 * InvalidRegularExpression What
 * ================================
 */
const char* InvalidRegularExpression::what() const noexcept
{
    return message.c_str();
}