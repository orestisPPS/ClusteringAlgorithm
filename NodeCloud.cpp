//
// Created by hal9000 on 1/30/24.
//

#include <algorithm>
#include "NodeCloud.h"
#include "ThreadingOperations.h"

NodeCloud::NodeCloud(shared_ptr<vector<shared_ptr<Node>>> nodesVector) {
    if (nodesVector->empty())
        throw runtime_error("NodeCloud cannot be created with an empty node vector");
    _nodes = std::move(nodesVector);
    _dimensions = _nodes->at(0)->getCoordinatesVector()->size();
    _nodeToNeighbours = make_unique<unordered_map<shared_ptr<Node>, list<shared_ptr<Node>>>>(_nodes->size());
    _coordinateComponentToNodeMaps = make_unique<vector<map<double, shared_ptr<Node>>>>(_dimensions);
    _visitedNodes = make_unique<unordered_map<shared_ptr<Node>, bool>>();
    for (auto &node: *_nodes){
        _visitedNodes->insert({node, false});
        _nodeToNeighbours->insert({node, list<shared_ptr<Node>>()});
        auto coordinates = node->getCoordinatesVector();
        for (unsigned i = 0; i < _dimensions; i++)
            (*_coordinateComponentToNodeMaps)[i][(*coordinates)[i]] = node;
    }
}

NodeCloud::~NodeCloud() {
    if (_nodes != nullptr)
        _nodes.reset();
    if (_nodeToNeighbours != nullptr)
        _nodeToNeighbours.reset();
    if (_coordinateComponentToNodeMaps != nullptr)
        _coordinateComponentToNodeMaps.reset();
}

list<shared_ptr<Node>> NodeCloud::findNeighboursOfNode(const shared_ptr<Node>& node, double radius) {
    auto neighbourCounter = make_unique<unordered_map<shared_ptr<Node>, unsigned>>();
    auto thisCoordinates = node->getCoordinatesVector();
    auto acceptedNeighbors = list<shared_ptr<Node>>();

    for (unsigned i = 0; i < _dimensions; i++) {
        auto lowerBound = (*_coordinateComponentToNodeMaps)[i].lower_bound(thisCoordinates->at(i) - radius);
        auto upperBound = (*_coordinateComponentToNodeMaps)[i].upper_bound(thisCoordinates->at(i) + radius);

        for (auto it = lowerBound; it != upperBound; it++) {
            if (neighbourCounter->find(it->second) == neighbourCounter->end())
                (*neighbourCounter)[it->second] = 1;
            else{
                (*neighbourCounter)[it->second]++;
                if ((*neighbourCounter)[it->second] == _dimensions)
                    acceptedNeighbors.push_back(it->second);
            }
        }
    }
    if (neighbourCounter->find(node) != neighbourCounter->end())
        neighbourCounter->erase(node);
    return std::move(acceptedNeighbors);
}

unsigned NodeCloud::calculateClusters(double radius) {
    
    auto neighbourFindThreadJob = [this, radius](unsigned start, unsigned end) {
        for (auto i = start; i < end; i++) {
            auto thisNode = (*_nodes)[i];
            (*_nodeToNeighbours)[thisNode] = std::move(findNeighboursOfNode(thisNode, radius));
        }
    };
    ThreadingOperations<shared_ptr<Node>>::executeParallelJob(neighbourFindThreadJob, _nodes->size(), sizeof((*_nodes)[0]),12);
    
    auto clusterCount = 0;
    for (auto &thisNode : *_nodes) {
        if (!(*_visitedNodes)[thisNode]) {
            auto cluster = make_shared<list<shared_ptr<Node>>>();
            _searchNeighboursRecursively(thisNode, radius, *_visitedNodes, cluster);
            clusterCount++;
        }
    }
    return clusterCount;
}

void NodeCloud::_searchNeighboursRecursively(const shared_ptr<Node> &node, double radius,
                                             unordered_map<shared_ptr<Node>, bool> &visitedNodes,
                                             const shared_ptr<list<shared_ptr<Node>>>& cluster) {
    visitedNodes[node] = true;
    cluster->push_back(node);
    auto &thisNodeNeighbours = (*_nodeToNeighbours)[node];
    for (auto &neighbour: thisNodeNeighbours)
        if (!visitedNodes[neighbour])
            _searchNeighboursRecursively(neighbour, radius, visitedNodes, cluster);
}

