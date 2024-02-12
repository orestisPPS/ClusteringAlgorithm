//
// Created by hal9000 on 2/9/24.
//

#ifndef ALTAIRINTERVIEW_TEST_H
#define ALTAIRINTERVIEW_TEST_H

#include <string>
#include <iostream>
#include "../NodeCloudFactory.h"

/**
 * @class Test
 * @brief Base class for running and evaluating test cases. Provides visual indication of the test outcome in the console.
 */
class Test {
public:
    Test() = default;

    /**
    * @brief Runs the test case.
    * Derived classes should override this method to implement specific test logic. The method is responsible
    * for setting the _passed member.
    */
    virtual void run() = 0;

protected:
    std::string _name; ///< Name of the test case.
    
    bool _passed = false; ///< Indicates whether the test passed or failed.

    /**
    * @brief Prints the start message for a test case to the console.
    * Indicates the beginning of a test case execution, displaying the test name.
    */
    void _consoleTestStart() const; 

    /**
    * @brief Prints the end message for a test case to the console, indicating the result.
    * Displays a [PASSED] message in green for successful tests or a [FAILED] message in red for failed tests.
    */
    void _consoleTestEnd() const;
};






/**
 * @protected
 * @var std::string Test::_name
 * @brief Name of the test case.
 */

/**
 * @protected
 * @var bool Test::_passed
 * @brief Indicates whether the test passed or failed.
 */

/**
 * @fn void Test::_consoleTestStart() const
 * @brief Prints the start message for a test case to the console.
 *
 * Indicates the beginning of a test case execution, displaying the test name.
 */

/**
 * @fn void Test::_consoleTestEnd() const
 * @brief Prints the end message for a test case to the console, indicating the result.
 *
 * Displays a [PASSED] message in green for successful tests or a [FAILED] message in red for failed tests.
 */


#endif //ALTAIRINTERVIEW_TEST_H
