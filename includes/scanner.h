/**
 * scanner.h
 *
 * Note the scanner is a wrapper around an istream object. As such, it is important
 * the stream the scanner is referring to stays in memory during the usage of the scanner.
 *
 * The scanner will read in characters from the stream according to the regular expression
 * being used to search.
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_SCANNER_H
#define SAGE_SCANNER_H

#include <istream>
#include <memory>

#include "regex.h"

namespace sage
{
    class Scanner
    {
        public:

            // Constructors
            Scanner(std::istream&, std::string="\\s+");
            Scanner(std::istream&, Regex);

            // Scanning Methods
            int nextInt();
            char nextChar();
            double nextDouble();
            std::string nextWord();
            std::string next(Regex);
            std::string readLine();

            // Peeking Methods
            char peekChar();

            // Testing Methods
            bool hasNextInt();
            bool hasNextChar();
            bool hasNextDouble();
            bool hasNext(Regex);
            bool eof();

        private:

            // The input source the scanner will read from
            std::istream& input;

            // Represents the regex matching the separator between tokens
            Regex delimiter;

            // Used to remove delimiter content between tokens in stream
            void clearDelimiterContent();

    };
}

#endif //SAGE_SCANNER_H
