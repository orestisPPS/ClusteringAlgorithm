//
// Created by hal9000 on 2/9/24.
//

#include "Test.h"

void Test::_consoleTestStart() const {
    std::cout << "Running " << _name << "... " << std::endl;
}

void Test::_consoleTestEnd() const {
    if (_passed)
        std::cout << "\033[1;32m[PASSED]\033[0m\n" << std::endl;  // This adds a green [PASSED] indicator
    else
        std::cout << "\033[1;31m[FAILED]\033[0m\n" << std::endl;  // This adds a red [FAILED] indicator
}