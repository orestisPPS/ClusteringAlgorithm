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
                {3.2, 5},    //0
                {11.4, 5},   //1
                {0, 5},      //2
                {3.2,0},     //3
                {12.8, 5}    //4
        };

        // Create a NodeCloud from the list of coordinates
        auto nodeCloud = NodeCloudFactory::createNodeCloud(coordinatesList);

        auto lengthScales = list<double>{0.7, 1.8, 4.8, 6.5, 10.6};
        auto expectedClusters = list<unsigned>{5, 4, 3, 2, 1};
        
        for (auto lengthScale : lengthScales) {
            auto cluster = nodeCloud->calculateClusters(lengthScale);
            _passed = cluster.size() == expectedClusters.front();
            cout << "Length Scale : " << lengthScale << " Calculated Cluster size : " << cluster.size() 
                 << " Expected Cluster size : " << expectedClusters.front() << endl;
            expectedClusters.pop_front();
            for (auto& object : cluster) {
                object->printCluster();
            }
        }
        _consoleTestEnd();
        
    }
};
#endif //ALTAIRINTERVIEW_ALTAIRTASK_H
