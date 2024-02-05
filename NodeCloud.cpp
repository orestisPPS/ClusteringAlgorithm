//
// Created by hal9000 on 1/30/24.
//

#include <algorithm>
#include "NodeCloud.h"

NodeCloud::NodeCloud(shared_ptr<vector<shared_ptr<Node>>> nodesVector) {
    if (nodesVector->empty())
        throw runtime_error("NodeCloud cannot be created with an empty node vector");
    _nodes = std::move(nodesVector);
    _dimensions = _nodes->at(0)->getCoordinatesVector()->size();
    _nodeToNeighbours = make_unique<unordered_map<shared_ptr<Node>, list<shared_ptr<Node>>>>(_nodes->size());
    _coordinateComponentToNodeMaps = std::move(_initializeCoordinateComponentToNodeMaps());
}

NodeCloud::~NodeCloud() {
    if (_nodes != nullptr)
        _nodes.reset();
    if (_nodeToNeighbours != nullptr)
        _nodeToNeighbours.reset();
}

list<shared_ptr<Node>> NodeCloud::findNeighboursOfNode(const shared_ptr<Node>& node, double radius) {
    auto neighbourCounter = make_unique<unordered_map<shared_ptr<Node>, unsigned>>();
    auto thisCoordinates = node->getCoordinatesVector();

    for (unsigned i = 0; i < _dimensions; i++) {
        auto lowerBound = (*_coordinateComponentToNodeMaps)[i].lower_bound(thisCoordinates->at(i) - radius);
        auto upperBound = (*_coordinateComponentToNodeMaps)[i].upper_bound(thisCoordinates->at(i) + radius);

        for (auto it = lowerBound; it != upperBound; it++) {
            if (neighbourCounter->find(it->second) == neighbourCounter->end())
                (*neighbourCounter)[it->second] = 1;
            else
                (*neighbourCounter)[it->second]++;
        }
    }
    if (neighbourCounter->find(node) != neighbourCounter->end())
        neighbourCounter->erase(node);

    // Find common neighbors across all dimensions
    auto acceptedNeighbors = list<shared_ptr<Node>>();

    for (auto &entry: *neighbourCounter) {
        if (entry.second == _dimensions)
            acceptedNeighbors.push_back(entry.first);
    }
    
    // if (inputThr
    return std::move(acceptedNeighbors);
    
}

unsigned NodeCloud::calculateClusters(double radius) {

    auto visitedNodes = make_unique<unordered_map<shared_ptr<Node>, bool>>();

    for (auto &thisNode : *_nodes) {
        (*_nodeToNeighbours)[thisNode] = std::move(findNeighboursOfNode(thisNode, radius));
        visitedNodes->insert({thisNode, false});
    }

    auto clusterCount = 0;

    for (auto &thisNode : *_nodes) {
        if (!(*visitedNodes)[thisNode]) {
            auto cluster = make_shared<list<shared_ptr<Node>>>();
            _searchNeighboursRecursively(thisNode, radius, *visitedNodes, cluster);
            clusterCount++;
        }
    }
    return clusterCount;
}

void NodeCloud::_searchNeighboursRecursively(const shared_ptr<Node> &node, double radius,
                                             unordered_map<shared_ptr<Node>, bool> &visitedNodes,
                                             shared_ptr<list<shared_ptr<Node>>> cluster) {
    visitedNodes[node] = true;
    cluster->push_back(node);
    auto &thisNodeNeighbours = (*_nodeToNeighbours)[node];
    for (auto &neighbour: thisNodeNeighbours)
        if (!visitedNodes[neighbour])
            _searchNeighboursRecursively(neighbour, radius, visitedNodes, cluster);
    
}


unique_ptr<vector<map<double, shared_ptr<Node>>>> NodeCloud::_initializeCoordinateComponentToNodeMaps() {
    auto vectorOfMaps = make_unique<vector<map<double, shared_ptr<Node>>>>(_dimensions);
    for (auto &node: *_nodes) {
        auto coordinates = node->getCoordinatesVector();
        for (unsigned i = 0; i < _dimensions; i++)
            (*vectorOfMaps)[i][(*coordinates)[i]] = node;
    }
    return vectorOfMaps;
}


//unsigned NodeCloud::calculateClusters(double radius) {
//    // Create a structure to hold sorted coordinates
//    unique_ptr<vector<map<double, shared_ptr<Node>>>> sortedCoordinatesToNodeID =
//            make_unique<vector<map<double, shared_ptr<Node>>>>(_dimensions);
//
//
//    // Data structure to track which nodes are in which cluster
//    map<shared_ptr<Node>, unsigned> nodeToClusterMap;
//    unsigned clusterCount = 0;
//
//    for (auto &thisNode : *_nodesVector) {
//        auto thisCoordinates = thisNode->getCoordinatesVector();
//        bool isNewCluster = true;
//
//        // Check for neighbors in each dimension
//        for (unsigned i = 0; i < _dimensions; i++) {
//            for (auto &otherCoordinateComponent: (*sortedCoordinatesToNodeID)[i]) {
//                if (fabs(otherCoordinateComponent.first - (*thisCoordinates)[i]) <= radius) {
//                    auto otherNode = otherCoordinateComponent.second;
//                    // Calculate the Euclidean distance between the nodes
//                    if (distanceBetweenNodes(thisNode, otherNode) <= radius) {
//                        // Check if this node or the neighbor is already part of a cluster
//                        if (nodeToClusterMap.find(thisNode) != nodeToClusterMap.end() ||
//                            nodeToClusterMap.find(otherNode) != nodeToClusterMap.end()) {
//                            isNewCluster = false;
//                            unsigned clusterId = nodeToClusterMap[thisNode];
//                            nodeToClusterMap[otherNode] = clusterId;
//                        }
//                    }
//                } else if (otherCoordinateComponent.first > (*thisCoordinates)[i] + radius) {
//                    break;
//                }
//            }
//        }
//
//        // If this node is not part of any cluster, create a new one
//        if (isNewCluster) {
//            nodeToClusterMap[thisNode] = clusterCount++;
//        }
//    }
//
//    return clusterCount;
//}

//
//        unsigned NodeCloud::dimensions() {
//            return _dimensions;
//        }
