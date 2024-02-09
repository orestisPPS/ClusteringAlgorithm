//
// Created by hal9000 on 2/9/24.
//

#ifndef ALTAIRINTERVIEW_TEST_H
#define ALTAIRINTERVIEW_TEST_H

#include <string>
#include <iostream>
#include "../NodeCloudFactory.h"


class Test {
public:
    Test() = default;
    
    virtual void run() {}

protected:
    std::string _name;
    
    bool _passed = false;
    
    void _consoleTestStart() const;

    void _consoleTestEnd() const;
};


#endif //ALTAIRINTERVIEW_TEST_H
