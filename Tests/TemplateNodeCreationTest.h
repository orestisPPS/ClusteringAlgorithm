//
// Created by hal9000 on 2/18/24.
//

#ifndef ALTAIRINTERVIEW_TEMPLATENODECREATIONTEST_H
#define ALTAIRINTERVIEW_TEMPLATENODECREATIONTEST_H

#include "Test.h"
#include "../NodeCloud.h"

class TemplateNodeCreationTest : public Test {
public:
    TemplateNodeCreationTest() {
        _name = "Template Node Creation Test";
    }

    void run() override{
        _consoleTestStart();
        // Define the length of each dimension of the domain, in this case a unit square
        
        // Define the number of nodes to be generated

        auto nodeCloud = NodeCloud<2, 3>({1,1}, 3);
    }
};

#endif //ALTAIRINTERVIEW_TEMPLATENODECREATIONTEST_H
