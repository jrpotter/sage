#include <iostream>

#include "regex.h"

using namespace std;
using namespace sage;

int main() {

    Regex r("a(b|c)*");
    cout << r.matches("acbcbe", 0) << endl;

    return 0;
}