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

        //auto nodeCloud = new NodeCloud<2, 30000000>({1,1}, 12);
        auto nodeCloud = new NodeCloud<2, 100>({1,1}, 1);
        //for (auto& node : nodeCloud->getNodes())
            //std::cout << node << std::endl;

        //print the number of nodes with scientific notation
        //std::cout << std::scientific << nodeCloud->_nodes.size() << std::endl;
        delete nodeCloud;
    }
};

#endif //ALTAIRINTERVIEW_TEMPLATENODECREATIONTEST_H
