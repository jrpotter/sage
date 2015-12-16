/**
 * regex_exception.cpp
 *
 * Created by jrpotter (12/13/2015).
 */

#include "Regex/InvalidRegex.h"

using namespace sage;

/**
 * Constructor
 * ================================
 */
InvalidRegex::InvalidRegex(std::string message, long index)
{
    std::stringstream ss;
    ss << message << " at position " << index << '.' << std::endl;
    response = ss.str();
}

InvalidRegex::InvalidRegex(std::string message, char problem, long index)
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
    response = ss.str();
}

/**
 * What
 * ================================
 */
const char* InvalidRegex::what() const noexcept
{
    return response.c_str();
}