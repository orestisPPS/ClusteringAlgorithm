//
// Created by hal9000 on 1/30/24.
//

#include "NodeCloud.h"


NodeCloud::NodeCloud(vector<Node *> nodesVector) {
    if (nodesVector.empty())
        throw runtime_error("NodeCloud cannot be created with an empty node vector");
    _nodes = std::move(nodesVector);
    _dimensions = _nodes[0]->getCoordinatesVector().size();
    _nodeToNeighbours = make_unique<unordered_map<Node*, list<Node*>>>(_nodes.size());
    _coordinateComponentToNodeMaps = make_unique<vector<map<double, list<Node*>>>>(_dimensions);
    _visitedNodes = make_unique<unordered_map<Node*, bool>>();
    for (auto &node: _nodes){
        _visitedNodes->insert({node, false});
        _nodeToNeighbours->insert({node, list<Node*>()});
        auto coordinates = node->getCoordinatesVector();
        for (unsigned i = 0; i < _dimensions; i++)
            (*_coordinateComponentToNodeMaps)[i][coordinates[i]].push_back(node);
    }
}

NodeCloud::~NodeCloud() {
    for (auto &node: _nodes)
        delete node;
}

const vector<Node*> &NodeCloud::getNodes() const {
    return _nodes;
}

list<Node*> NodeCloud::findNeighboursOfNode(Node* node, double radius) {
    auto candidateNodeCounter = make_unique<unordered_map<Node*, unsigned>>();
    auto thisCoordinates = node->getCoordinatesVector();
    auto filteredNodes = make_shared<list<Node*>>();
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
                    if (timesAppeared == _dimensions && candidateNode != node && _assessNeighbour(node, candidateNode, radius, *filteredNodes)) {
                        filteredNodes->push_back(candidateNode);
                    }
                }
            }
        }
    }
    return std::move(*filteredNodes);
}

list<shared_ptr<NodeCluster>> NodeCloud::calculateClusters(double radius, unsigned availableThreads) {

    auto clusters = list<shared_ptr<NodeCluster>>();

    auto neighbourFindThreadJob = [this, radius](unsigned start, unsigned end) {
        for (auto i = start; i < end; i++) {
            auto thisNode = _nodes[i];
            (*_nodeToNeighbours)[thisNode] = std::move(findNeighboursOfNode(thisNode, radius));
        }
    };
    ThreadingOperations<Node*>::executeParallelJob(neighbourFindThreadJob, _nodes.size(), availableThreads);

    unsigned clusterId = 0;
    for (auto &thisNode : _nodes) {
        if (!(*_visitedNodes)[thisNode]) {
            auto cluster = make_shared<NodeCluster>(clusterId);
            clusterId++;
            clusters.push_back(cluster);
            _searchNeighboursRecursively(thisNode, cluster);
        }
    }
    
    auto clearUnorderedMap = [&](unsigned start, unsigned end){
        for (auto i = start; i < end; i++) {
            (*_visitedNodes)[_nodes[i]] = false;
            (*_nodeToNeighbours)[_nodes[i]].clear();
        }
    };
    ThreadingOperations<Node*>::executeParallelJob(clearUnorderedMap, _nodes.size(), availableThreads);
    return std::move(clusters);
}

bool NodeCloud::_assessNeighbour(Node *thisNode, Node *candidateNode, double radius,
                                 list<Node*> &filteredNodes) const {
    auto &thisCoordinates = thisNode->getCoordinatesVector();
    auto &candidateCoordinates = candidateNode->getCoordinatesVector();
    double distance = 0;

    for (unsigned j = 0; j < _dimensions; j++)
        distance += (thisCoordinates[j] - candidateCoordinates[j]) * (thisCoordinates[j] - candidateCoordinates[j]);
    if(sqrt(distance) <= radius)
    //if( distance <= radius * radius)
        return true;
    return false;
}

void NodeCloud::_searchNeighboursRecursively(Node *node, const shared_ptr<NodeCluster> &cluster) {
    (*_visitedNodes)[node] = true;
    cluster->getNodes().push_back(node);
    for (auto &neighbour: (*_nodeToNeighbours)[node])
        if (!(*_visitedNodes)[neighbour])
            _searchNeighboursRecursively(neighbour, cluster);
}







