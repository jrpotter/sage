/**
 * scanner.h
 *
 * Note the scanner is a wrapper around an istream object. As such, it is important
 * the stream the scanner is referring to stays in memory during the usage of the scanner.
 * The scanner will read in characters from the stream according to the regular expression
 * being used to search.
 *
 * Created by jrpotter (11/26/2015).
 */
#ifndef SAGE_SCANNER_H
#define SAGE_SCANNER_H

#include <istream>
#include <memory>
#include <stack>

#include "regex/regex.h"
#include "scan_exception.h"
#include "scan_state.h"
#include "string.h"

namespace sage
{
    class Scanner
    {
        public:

            // Constructors
            Scanner(std::istream&, std::string=REGEX_EXPR_WHITESPACE);

            // Scanning Methods
            int nextInt();
            char nextChar();
            char nextLetter();
            double nextDouble();
            std::string nextWord();
            std::string next(Regex);
            std::string readLine();
            std::string readUntil(char);

            // Direct Methods
            char read();
            char peek(int = 0);

            // Checkpoints
            // Allows returning back to a given state
            void saveCheckpoint();
            ScanState restoreCheckpoint();
            ScanState getCurrentState();

        private:

            // The input source the scanner will read from
            std::istream& input;

            // Maintain position
            std::stack<ScanState> states;

            // Represents the regex matching the separator between tokens
            Regex delimiter;

            // Used to remove delimiter content between tokens in stream
            void clearDelimiterContent();

    };
}

#endif //SAGE_SCANNER_H
