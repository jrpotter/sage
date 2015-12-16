//
// Created by jrpotter on 12/13/15.
//

#ifndef SAGE_STRING_H
#define SAGE_STRING_H

#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>
#include <sstream>
#include <vector>

namespace sage
{
    // Trim from left
    // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    static inline std::string ltrim(std::string s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // Trim from right
    // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    static inline std::string rtrim(std::string s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    // Trim both sides
    // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    static inline std::string trim(std::string s)
    {
        return ltrim(rtrim(s));
    }

    // Split based on delimiter
    static std::vector<std::string> split(const std::string s, std::string delim)
    {
        unsigned long pos = 0;
        std::vector<std::string> elems;

        // Loop through and add all substrings
        while(s.find(delim, pos) != std::string::npos) {
            size_t next = s.find(delim, pos);
            elems.push_back(s.substr(pos, next));
            pos += next + delim.length();
        }

        // Append remainder of string
        if(pos < s.length()) {
            elems.push_back(s.substr(pos));
        }

        return elems;
    }

}

#endif //SAGE_STRING_H
