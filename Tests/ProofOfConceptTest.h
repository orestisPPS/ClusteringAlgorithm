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

    /**
     * @brief Tests the proof of concept of the code.
     *
     * 1.	Given a point cloud in 2D, so basically a list of (x and y) coordinate pairs and a length scale.
     * 
     * 2.	Any points that are within this length, should be considered as belonging to the same cluster.
     *      (Please note that if point A and B are within this length scale, and so are points B and C, even though points A and C
     *      may not be within the length scale, they still belong to the same cluster. 
     *      
     * 3.	How many clusters exist?
     *      
     *      Example:
     *      A (0,0), B (1,0), C (0,1) (isosceles triangle)
     *      Length scale: 1
     *      Expected output: 1
     *  @note Passes if the calculated number of clusters equals the expected number of clusters and the cluster consists
     *        of 3 nodes. Fails otherwise.
     */
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
        auto clusters = nodeCloud->calculateClusters(1, 1);
        if (clusters.size() == 1 && clusters.front()->getNodes().size() == 3){
            _passed = true;
        }
        cout << "Expected number of clusters: 1" << " Calculated number of clusters: " << clusters.size() << endl;
        cout << "Expected number of nodes in the cluster: 3" << " Calculated number of nodes in the cluster: " << clusters.front()->getNodes().size() << endl;
        _consoleTestEnd();
        for (auto &cluster : clusters) {
            //cluster->printCluster();
        }
    }
};


#endif //ALTAIRINTERVIEW_PROOFOFCONCEPTTEST_H
