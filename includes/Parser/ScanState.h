/**
 * scan_state.h
 *
 * The following allows keeping track of the current position in the scanner.
 * At any one point, there should be a current scan state. If requested, one
 * can save states and restore them at a later point.
 *
 * Created by jrpotter (11/26/2015).
 */
#ifndef SAGE_SCAN_STATE_H
#define SAGE_SCAN_STATE_H

#include <ios>
#include <istream>

namespace sage
{
    class ScanState
    {
        public:
            ScanState(std::istream&, unsigned int, unsigned int);

            // Getters
            long getCursor() const;
            unsigned int getLine() const;
            unsigned int getColumn() const;
            std::ios_base::iostate getBufferState() const;

            // Utility methods to modify buffer state
            void advance(const char = 0);
            void reset(std::istream&, unsigned int = -1, unsigned int = -1);

        private:
            long cursor;
            unsigned int line;
            unsigned int column;
            std::ios_base::iostate buffer_state;
    };
}

#endif //SAGE_SCAN_STATE_H
