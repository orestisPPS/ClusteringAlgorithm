//
// Created by hal9000 on 2/10/24.
//

#ifndef ALTAIRINTERVIEW_ALTAIRTASK_H
#define ALTAIRINTERVIEW_ALTAIRTASK_H

#include "Test.h"

class AltairTask : public Test {
public:
    AltairTask() {
        _name = "Altair Task";
    }

    void run() override{
        _consoleTestStart();
        // Create a list of coordinates with a triangle with 2 sides of equal length
        list<vector<double>> coordinatesList = {
                {0, 0},
                {1, 0},
                {0, 1}
        };
        // Create a NodeCloud from the list of coordinates
        auto nodeCloud = NodeCloudFactory::createNodeCloud(coordinatesList);
        // Calculate the clusters of the NodeCloud
        auto clusters = nodeCloud->calculateClusters(0.5);
        // Print the clusters
        if (clusters.size() == 1) {
            cout << "The NodeCloud has one cluster" << endl;
        } else {
            cout << "The NodeCloud has " << clusters.size() << " clusters" << endl;
        }
    }
};
#endif //ALTAIRINTERVIEW_ALTAIRTASK_H
