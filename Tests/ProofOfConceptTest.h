//
// Created by hal9000 on 2/9/24.
//

#ifndef ALTAIRINTERVIEW_PROOFOFCONCEPTTEST_H
#define ALTAIRINTERVIEW_PROOFOFCONCEPTTEST_H

#include "Test.h"

class ProofOfConceptTest : public Test {
public:
    ProofOfConceptTest() {
        _name = "Proof Of Concept";
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
        auto clusters = nodeCloud->calculateClusters(1);
        for (auto &cluster : clusters) {
            cluster->printCluster();
        }

    }
};


#endif //ALTAIRINTERVIEW_PROOFOFCONCEPTTEST_H
