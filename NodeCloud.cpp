//
// Created by hal9000 on 1/30/24.
//

#include <algorithm>
#include "NodeCloud.h"
#include "HardwareAcceleration/ThreadingOperations.h"

NodeCloud::NodeCloud(shared_ptr<vector<shared_ptr<Node>>> nodesVector) {
    if (nodesVector->empty())
        throw runtime_error("NodeCloud cannot be created with an empty node vector");
    _nodes = std::move(nodesVector);
    _dimensions = _nodes->at(0)->getCoordinatesVector()->size();
    hardwareAccelerationConfig = HardwareAccelerationConfig();
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
    auto candidateNodeCounter = make_unique<unordered_map<shared_ptr<Node>, unsigned>>();
    auto &thisCoordinates = *node->getCoordinatesVector();
    auto filteredNodes = make_shared<list<shared_ptr<Node>>>();
    double distance, euclideanDistance;
    for (unsigned i = 0; i < _dimensions; i++) {
        auto lowerBound = (*_coordinateComponentToNodeMaps)[i].lower_bound(thisCoordinates[i] - radius);
        auto upperBound = (*_coordinateComponentToNodeMaps)[i].upper_bound(thisCoordinates[i] + radius);

        for (auto it = lowerBound; it != upperBound; it++) {
            auto &candidateNode = it->second;
            if (candidateNodeCounter->find(candidateNode) == candidateNodeCounter->end())
                (*candidateNodeCounter)[candidateNode] = 1;
            else{
                auto &timesAppeared = (*candidateNodeCounter)[candidateNode];
                timesAppeared++;
                if (timesAppeared == _dimensions && candidateNode != node && !hardwareAccelerationConfig.isCudaEnabled()) {
                    auto &neighbourCoordinates = *candidateNode->getCoordinatesVector();
                    distance = 0;
                    for (unsigned j = 0; j < _dimensions; j++)
                        distance += (thisCoordinates[j] - neighbourCoordinates[j]) * (thisCoordinates[j] - neighbourCoordinates[j]);
                    euclideanDistance = sqrt(distance);
                    if(euclideanDistance <= radius)
                        filteredNodes->push_back(candidateNode);
                }
//                else if (timesAppeared == _dimensions && hardwareAccelerationConfig.isCudaEnabled()) {
//                    filteredNodes->push_back(candidateNode);
//                }
            }
        }
    }
    if (hardwareAccelerationConfig.isCudaEnabled())
        _calculateDistancesCuda(node, radius, filteredNodes);
    return std::move(*filteredNodes);
}

void NodeCloud::_calculateDistancesCuda(const shared_ptr<Node> &node, double radius,
                                        shared_ptr<list<shared_ptr<Node>> > &consideredNeighbors) const {
    auto coordinates = make_unique<vector<double*>>(consideredNeighbors->size() * _dimensions);
    auto accepted = make_unique<vector<bool>>(consideredNeighbors->size(), false);
    auto nodeIndex = 0;
    for (auto &neighbour: *consideredNeighbors) {
        auto neighbourCoordinates = neighbour->getCoordinatesVector();
        for (unsigned i = 0; i < _dimensions; i++)
            (*coordinates)[nodeIndex * _dimensions + i] = &(*neighbourCoordinates)[i];
        nodeIndex++;
    }
    //Do cuda stuff here
    //...
    //...
    coordinates.reset();
    nodeIndex = 0;
    for (auto &neighbour: *consideredNeighbors) {
        if ((*accepted)[nodeIndex]) {
            consideredNeighbors->remove(neighbour);
            nodeIndex++;
        }
    }
}


list<shared_ptr<list<shared_ptr<Node>>>> NodeCloud::calculateClusters(double radius) {
    
    auto clusters = list<shared_ptr<list<shared_ptr<Node>>>>();
    auto neighbourFindThreadJob = [this, radius](unsigned start, unsigned end) {
        for (auto i = start; i < end; i++) {
            auto thisNode = (*_nodes)[i];
            (*_nodeToNeighbours)[thisNode] = std::move(findNeighboursOfNode(thisNode, radius));
        }
    };
    ThreadingOperations<shared_ptr<Node>>::executeParallelJob(neighbourFindThreadJob, _nodes->size(), sizeof((*_nodes)[0]),1);
    
    for (auto &thisNode : *_nodes) {
        if (!(*_visitedNodes)[thisNode]) {
            auto cluster = make_shared<list<shared_ptr<Node>>>();
            clusters.push_back(cluster);
            _searchNeighboursRecursively(thisNode, radius, *_visitedNodes, cluster);
        }
    }
    return std::move(clusters);
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

