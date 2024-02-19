//
// Created by hal9000 on 2/10/24.
//

#ifndef ALTAIRINTERVIEW_ALTAIREXAMPLE_H
#define ALTAIRINTERVIEW_ALTAIREXAMPLE_H

#include "Test.h"

class AltairExample : public Test {
public:
    AltairExample() {
        _name = "Altair Task";
    }

    /**
    * @brief Tests the code based on the provided example
    *
    *      Nodes : 1 (3.2, 5)  2 (11.4, 5)  3 (0, 5)  4 (3.2,0)  5 (12.8, 5)
    *      Length scales : Red : 0.7, Orange : 1.8, Green : 4.8, Blue : 6.5, Purple : 10.6
    *      Expected output: Red : 5, Orange : 4, Green : 3, Blue : 2, Purple : 1
    *  @note Passes if the number of clusters for each length scale is as expected and fails otherwise.
    */
    void run() override{
        _consoleTestStart();

        // Create a list of coordinates corresponding to the points of the assigned task's example
        array<array<double, 2>, 5> coordinates = {{
            {3.2, 5},
            {11.4, 5},
            {0, 5},
            {3.2,0},
            {12.8, 5}
        }};

        // Create a NodeCloud from the list of coordinates

        //Create a list of the example's length scales to calculate the clusters 
        auto lengthScales = list<double>{0.7, 1.8, 4.8, 6.5, 10.6};
        //Create a list of the expected cluster sizes
        auto expectedClusters = list<unsigned>{5, 4, 3, 2, 1};

        // Calculate the clusters of the NodeCloud for each length scale
        for (auto lengthScale : lengthScales) {
            auto cluster = NodeCloud<2,5>(coordinates).calculateClusters(lengthScale, 1);
            _passed = cluster.size() == expectedClusters.front();
            cout << "Length Scale : " << lengthScale << " Calculated Cluster size : " << cluster.size() 
                 << " Expected Cluster size : " << expectedClusters.front() << endl;
            expectedClusters.pop_front();
            for (auto& object : cluster) {
                //object->printCluster();
            }
        }
        _consoleTestEnd();

    }
};
#endif //ALTAIRINTERVIEW_ALTAIREXAMPLE_H
