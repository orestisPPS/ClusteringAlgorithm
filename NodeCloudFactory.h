//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUDFACTORY_H
#define ALTAIRINTERVIEW_NODECLOUDFACTORY_H
#include <list>
#include <random>
#include "NodeCloud.h"

class NodeCloudFactory {
public:
    
    NodeCloudFactory();

    shared_ptr<NodeCloud> createNodeCloud(const list<vector<double>>& coordinatesList);

    shared_ptr<NodeCloud> createNodeCloud(const map<unsigned short, double>& directionToDomainLength, unsigned numberOfNodes);
    
private:
    
    static void _checkInputMap(const map<unsigned short, double>& directionToDomainLength);
    
    shared_ptr<list<shared_ptr<Node>>> _initializeRandomCoordinateNodesList(const map<unsigned short, double>& directionToDomainLength, unsigned numberOfNodes);

    static shared_ptr<vector<shared_ptr<Node>>> _initializeNodesVectorFromCoordinatesList(const list<vector<double>>& coordinatesList);
    
    static void _sortNodesList(const shared_ptr<list<shared_ptr<Node>>>& nodesList, unsigned maximumDirections);
    
    static shared_ptr<vector<shared_ptr<Node>>> _covertListToVectorAndAssignID(const shared_ptr<list<shared_ptr<Node>>>& nodesList);
    
    
    //Seed the random device to the Mersenne Twister generator
    random_device _randomDevice;
    
    //Mersenne Twister generator
    mt19937 _generator;
};


#endif //ALTAIRINTERVIEW_NODECLOUDFACTORY_H

//Abstract Factory Pattern:
// _initalizeRandom2DNodesVector() can be take map<Direction, unsigned> as a parameter instead of domainLength1 and domainLength2