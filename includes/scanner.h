/**
 * scanner.h
 *
 * Created by jrpotter (11/26/2015).
 */

#ifndef SAGE_SCANNER_H
#define SAGE_SCANNER_H

#include <istream>
#include <memory>

namespace sage {

    class Scanner {

        public:
            Scanner();
            ~Scanner()=default;
            Scanner(const Scanner&);
            Scanner(Scanner&&);
            Scanner& operator= (Scanner);
            void swap(Scanner&, Scanner&);

    };

}

#endif //SAGE_SCANNER_H
