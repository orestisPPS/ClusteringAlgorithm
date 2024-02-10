//
// Created by hal9000 on 1/30/24.
//

#include "NodeCloud.h"


NodeCloud::NodeCloud(shared_ptr<vector<shared_ptr<Node>>> nodesVector) {
    if (nodesVector->empty())
        throw runtime_error("NodeCloud cannot be created with an empty node vector");
    _nodes = std::move(nodesVector);
    _dimensions = (*_nodes)[0]->getCoordinatesVector()->size();
    _nodeToNeighbours = make_unique<unordered_map<shared_ptr<Node>, list<shared_ptr<Node>>>>(_nodes->size());
    _coordinateComponentToNodeMaps = make_unique<vector<map<double, list<shared_ptr<Node>>>>>(_dimensions);
    _visitedNodes = make_unique<unordered_map<shared_ptr<Node>, bool>>();
    for (auto &node: *_nodes){
        _visitedNodes->insert({node, false});
        _nodeToNeighbours->insert({node, list<shared_ptr<Node>>()});
        auto coordinates = node->getCoordinatesVector();
        for (unsigned i = 0; i < _dimensions; i++)
            (*_coordinateComponentToNodeMaps)[i][(*coordinates)[i]].push_back(node);
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
    for (unsigned i = 0; i < _dimensions; i++) {
        //Find the range of nodal coordinate components i that could be within the radius of node at dimension i
        auto lowerBound = (*_coordinateComponentToNodeMaps)[i].lower_bound(thisCoordinates[i] - radius);
        auto upperBound = (*_coordinateComponentToNodeMaps)[i].upper_bound(thisCoordinates[i] + radius);
        //Iterate through the range of coordinate component keys and count the number of times each candidate nodes appear
        for (auto it = lowerBound; it != upperBound; it++) {
            for (auto &candidateNode: it->second) {
                //Appears for the first time
                if (candidateNodeCounter->find(candidateNode) == candidateNodeCounter->end())
                    (*candidateNodeCounter)[candidateNode] = 1;
                    //Appears for > 1 times
                else {
                    auto &timesAppeared = (*candidateNodeCounter)[candidateNode];
                    timesAppeared++;
                    if (timesAppeared == _dimensions && candidateNode != node) {
                        if (!cudaEnabled)
                            _assessNeighbour(node, candidateNode, radius, *filteredNodes);
                        else
                            filteredNodes->push_back(candidateNode);
                    }
                }
            }
        }
    }
    if (cudaEnabled)
        _calculateDistancesCuda(node, radius, filteredNodes);
    return std::move(*filteredNodes);
}

list<shared_ptr<list<shared_ptr<Node>>>> NodeCloud::calculateClusters(double radius) {

    auto clusters = list<shared_ptr<list<shared_ptr<Node>>>>();
    auto neighbourFindThreadJob = [this, radius](unsigned start, unsigned end) {
        for (auto i = start; i < end; i++) {
            auto thisNode = (*_nodes)[i];
            (*_nodeToNeighbours)[thisNode] = std::move(findNeighboursOfNode(thisNode, radius));
        }
    };
    HardwareAcceleration<shared_ptr<Node>>::executeParallelJob(neighbourFindThreadJob, _nodes->size(), sizeof((*_nodes)[0]), 1);

    for (auto &thisNode : *_nodes) {
        if (!(*_visitedNodes)[thisNode]) {
            auto cluster = make_shared<list<shared_ptr<Node>>>();
            clusters.push_back(cluster);
            _searchNeighboursRecursively(thisNode, radius, *_visitedNodes, cluster);
        }
    }
    return std::move(clusters);
}

void NodeCloud::_assessNeighbour(const shared_ptr<Node> &thisNode, const shared_ptr<Node> &candidateNode, double radius,
                                 list<shared_ptr<Node>> &filteredNodes) const {
    auto &thisCoordinates = *thisNode->getCoordinatesVector();
    auto &candidateCoordinates = *candidateNode->getCoordinatesVector();
    double distance = 0;
    
    for (unsigned j = 0; j < _dimensions; j++)
        distance += (thisCoordinates[j] - candidateCoordinates[j]) * (thisCoordinates[j] - candidateCoordinates[j]);
    if( sqrt(distance) <= radius)
        filteredNodes.push_back(candidateNode);
}

void NodeCloud::_searchNeighboursRecursively(const shared_ptr<Node> &node, double radius,
                                             unordered_map<shared_ptr<Node>, bool> &visitedNodes,
                                             const shared_ptr<list<shared_ptr<Node>>>& cluster) {
    visitedNodes[node] = true;
    cluster->push_back(node);
    for (auto &neighbour: (*_nodeToNeighbours)[node])
        if (!visitedNodes[neighbour])
            _searchNeighboursRecursively(neighbour, radius, visitedNodes, cluster);
}

void NodeCloud::_calculateDistancesCuda(const shared_ptr<Node> &node, double radius,
                                        shared_ptr<list<shared_ptr<Node>> > &consideredNeighbors) const {
    auto coordinates = make_unique<vector<const double*>>(consideredNeighbors->size() * _dimensions);
    auto accepted = make_unique<vector<bool>>(consideredNeighbors->size(), false);
    auto nodeIndex = 0;
    for (auto &neighbour: *consideredNeighbors) {
        auto &neighbourCoordinates = neighbour->getCoordinatesVector();
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







