//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUD_H
#define ALTAIRINTERVIEW_NODECLOUD_H

#include "Node.h"
#include <map>
#include <unordered_map>
#include <list>
#include <cmath>
class NodeCloud {
public:
    explicit NodeCloud(shared_ptr<vector<shared_ptr<Node>>> nodesVector);
    
    unsigned dimensions();
    
    list<shared_ptr<Node>> findNeighboursOfNode(const shared_ptr<Node>& node, double radius);

    unsigned calculateClusters(double radius);

    ~NodeCloud();
    
private:

    shared_ptr<vector<shared_ptr<Node>>> _nodes;

    unsigned _dimensions;
    
    unique_ptr<unordered_map<shared_ptr<Node>, list<shared_ptr<Node>>>> _nodeToNeighbours;
    
    unique_ptr<vector<map<double, shared_ptr<Node>>>> _coordinateComponentToNodeMaps;

    unique_ptr<vector<map<double, shared_ptr<Node>>>> _initializeCoordinateComponentToNodeMaps();

    //void calculateNodalNeighbours(shared_ptr<node
    
    
};


#endif //ALTAIRINTERVIEW_NODECLOUD_H
