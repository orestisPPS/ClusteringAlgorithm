//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUD_H
#define ALTAIRINTERVIEW_NODECLOUD_H

#include <map>
#include <unordered_map>
#include <list>
#include <cmath>
#include "HardwareAcceleration/HardwareAccelerationConfig.h"
#include "Node.h"

class NodeCloud {
public:
    explicit NodeCloud(shared_ptr<vector<shared_ptr<Node>>> nodesVector);
    
    unsigned dimensions();
    
    list<shared_ptr<Node>> findNeighboursOfNode(const shared_ptr<Node>& node, double radius);

    list<shared_ptr<list<shared_ptr<Node>>>> calculateClusters(double radius);
    
    HardwareAccelerationConfig hardwareAccelerationConfig;
    
    ~NodeCloud();
    
private:

    shared_ptr<vector<shared_ptr<Node>>> _nodes;

    unsigned _dimensions;
    
    unique_ptr<unordered_map<shared_ptr<Node>, list<shared_ptr<Node>>>> _nodeToNeighbours;

    unique_ptr<unordered_map<shared_ptr<Node>, bool>> _visitedNodes;
    
    unique_ptr<vector<map<double, shared_ptr<Node>>>> _coordinateComponentToNodeMaps;
    
    void _calculateDistancesCuda(const shared_ptr<Node>& node, double radius, shared_ptr<list<shared_ptr<Node>>>& consideredNeighbors) const;
    
    void _searchNeighboursRecursively(const shared_ptr<Node> &node, double radius,
                                      unordered_map<shared_ptr<Node>, bool> &visitedNodes,
                                      const shared_ptr<list<shared_ptr<Node>>>& cluster);
};


#endif //ALTAIRINTERVIEW_NODECLOUD_H
