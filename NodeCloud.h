//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUD_H
#define ALTAIRINTERVIEW_NODECLOUD_H

#include <map>
#include <unordered_map>
#include <list>
#include <cmath>
#include <algorithm>
#include "NodeCluster.h"
#include "HardwareAcceleration/HardwareAcceleration.h"

class NodeCloud {
public:
    explicit NodeCloud(shared_ptr<vector<shared_ptr<Node>>> nodesVector);

    list<shared_ptr<Node>> findNeighboursOfNode(const shared_ptr<Node>& node, double radius);

    list<shared_ptr<NodeCluster>> calculateClusters(double radius);

    ~NodeCloud();

private:

    shared_ptr<vector<shared_ptr<Node>>> _nodes;

    unsigned _dimensions;

    unique_ptr<unordered_map<shared_ptr<Node>, list<shared_ptr<Node>>>> _nodeToNeighbours;

    unique_ptr<unordered_map<shared_ptr<Node>, bool>> _visitedNodes;

    unique_ptr<vector<map<double, list<shared_ptr<Node>>>>> _coordinateComponentToNodeMaps;
    
    void _searchNeighboursRecursively(const shared_ptr<Node> &node, double radius,
                                      const shared_ptr<NodeCluster> &cluster);

    void _assessNeighbour(const shared_ptr<Node> &thisNode, const shared_ptr<Node> &candidateNode, double radius, list<shared_ptr<Node>> &filteredNodes) const;
};


#endif //ALTAIRINTERVIEW_NODECLOUD_H
