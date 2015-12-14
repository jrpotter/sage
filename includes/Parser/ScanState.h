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

namespace sage
{
    class ScanState
    {
        friend class Scanner;

        public:
            ScanState(long, unsigned int, unsigned int);
            void advance(const char = 0);

            long getCursor() const;
            unsigned int getLine() const;
            unsigned int getColumn() const;

        private:
            long cursor;
            unsigned int line;
            unsigned int column;
    };
}

#endif //SAGE_SCAN_STATE_H
