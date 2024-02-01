//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUDFACTORY_H
#define ALTAIRINTERVIEW_NODECLOUDFACTORY_H
#include <list>
#include "NodeCloud2D.h"

class NodeCloudFactory {
    NodeCloudFactory();

    shared_ptr<NodeCloud2D> createNodeCloud(list<vector<double>> coordinatesList);

    shared_ptr<NodeCloud2D> createNodeCloud2D(unsigned domainLength1, unsigned domainLength2, unsigned numberOfNodes);
    
private:
    
    shared_ptr<vector<shared_ptr<Node>>> _initializeRandom2DNodesVector(unsigned domainLength1, unsigned domainLength2, unsigned numberOfNodes);
    
    void _sortNodesVector(shared_ptr<vector<shared_ptr<Node>>> nodesVector);
    //Seed the random device to the Mersenne Twister generator
    random_device _randomDevice;
    
    //Mersenne Twister generator
    mt19937 _generator;
};


#endif //ALTAIRINTERVIEW_NODECLOUDFACTORY_H

//Abstract Factory Pattern:
// _initalizeRandom2DNodesVector() can be take map<Direction, unsigned> as a parameter instead of domainLength1 and domainLength2