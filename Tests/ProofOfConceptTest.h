//
// Created by hal9000 on 2/9/24.
//

#ifndef ALTAIRINTERVIEW_PROOFOFCONCEPTTEST_H
#define ALTAIRINTERVIEW_PROOFOFCONCEPTTEST_H

#include "Test.h"


class ProofOfConceptTest : public Test {
public:
    ProofOfConceptTest() {
        _name = "Proof of Concept";
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
        // Create a list of coordinates with a triangle with 2 sides of equal length
        list<array<double, 2>> coordinates = {{
            {0, 0},
            {1, 0},
            {0, 1}
        }};
        auto algorithms = list<ClusteringAlgorithmType>{
            UNION_FIND_PER_NODE,
            UNION_FIND_BUNCH,
            DEPTH_FIRST_SEARCH
        };
        auto algorithmNames = list<string>{
            "Union Find Per Node",
            "Union Find Bunch",
            "Depth First Search"
        };
        
        for (auto algorithm : algorithms) {
            _name = "Proof of concept " + algorithmNames.front();
            _consoleTestStart();
            // Create a NodeCloud from the list of coordinates
            auto clusters = NodeCloud<2, 3>(coordinates, 1).findClusters(1, algorithm);
            if (clusters.size() == 1 && clusters.front().items.size() == 3)
                _passed = true;
            cout << "Clusters [Expected / Calculated] : [1 / " << clusters.size() << "]" << endl;
            cout << "Cluster size [Expected / Calculated] : [3 / " << clusters.front().items.size() << "]" << endl;
            
            _consoleTestEnd();
            algorithmNames.pop_front();
        }

    }
};


#endif //ALTAIRINTERVIEW_PROOFOFCONCEPTTEST_H
