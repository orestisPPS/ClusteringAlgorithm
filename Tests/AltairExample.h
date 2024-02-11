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

    void run() override{
        _consoleTestStart();
        
        // Create a list of coordinates corresponding to the points of the assigned task's example
        list<vector<double>> coordinatesList = {
                {3.2, 5},    //0
                {11.4, 5},   //1
                {0, 5},      //2
                {3.2,0},     //3
                {12.8, 5}    //4
        };

        // Create a NodeCloud from the list of coordinates
        auto nodeCloud = NodeCloudFactory::createNodeCloud(coordinatesList);

        //Create a list of the example's length scales to calculate the clusters 
        auto lengthScales = list<double>{0.7, 1.8, 4.8, 6.5, 10.6};
        //Create a list of the expected cluster sizes
        auto expectedClusters = list<unsigned>{5, 4, 3, 2, 1};
        
        // Calculate the clusters of the NodeCloud for each length scale
        for (auto lengthScale : lengthScales) {
            auto cluster = nodeCloud->calculateClusters(lengthScale, 1);
            _passed = cluster.size() == expectedClusters.front();
            cout << "Length Scale : " << lengthScale << " Calculated Cluster size : " << cluster.size() 
                 << " Expected Cluster size : " << expectedClusters.front() << endl;
            cout << endl;
            expectedClusters.pop_front();
            for (auto& object : cluster) {
                //object->printCluster();
            }
        }
        _consoleTestEnd();
        
    }
};
#endif //ALTAIRINTERVIEW_ALTAIREXAMPLE_H
