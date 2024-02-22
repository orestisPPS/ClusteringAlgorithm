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

        // Create a list of coordinates corresponding to the points of the assigned task's example
        list<array<double, 2>> coordinates = {{
            {3.2, 5},
            {11.4, 5},
            {0, 5},
            {3.2, 0},
            {12.8, 5}
        }};

        // Create a NodeCloud from the list of coordinates

        auto algorithms = vector<ClusteringAlgorithmType> {
                UNION_FIND_PER_NODE,
                UNION_FIND_BUNCH,
                DEPTH_FIRST_SEARCH
        };
        auto algorithmNames = vector<string>{
                "Union Find Per Node",
                "Union Find Bunch",
                "Depth First Search"
        };
        
        for (unsigned i = 0 ; i < algorithms.size(); i++) {
            //Create a list of the example's length scales to calculate the clusters 
            auto lengthScales = vector<double>{0.7, 1.8, 4.8, 6.5, 10.6};
            //Create a list of the expected cluster sizes
            auto expectedClusters = vector<unsigned>{5, 4, 3, 2, 1};
            _name = "Altair Example " + algorithmNames[i];
            _consoleTestStart();
            _passed = true;
            for (unsigned j = 0; j < lengthScales.size(); j++) {
                auto clusters = NodeCloud<2, 5>(coordinates, 1).findClusters(lengthScales[j], algorithms[i]);
                if (clusters.size() != expectedClusters[j]) {
                    _passed = false;
                }
                cout << "Length Scale " << lengthScales[j] << " : Clusters [Expected / Calculated] : [" << expectedClusters[j] << " / " << clusters.size() << "]" << endl;
            }
            _consoleTestEnd();
        }
        // Calculate the clusters of the NodeCloud for each length scale
        

    }
};
#endif //ALTAIRINTERVIEW_ALTAIREXAMPLE_H
