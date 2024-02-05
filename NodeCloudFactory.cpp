//
// Created by hal9000 on 1/30/24.
//

#include "NodeCloudFactory.h"


NodeCloudFactory::NodeCloudFactory() : _generator(_randomDevice()) {
}

shared_ptr<NodeCloud> NodeCloudFactory::createNodeCloud(const list<vector<double>>& coordinatesList) {
    auto nodesVector = _initializeNodesVectorFromCoordinatesList(coordinatesList);
    return make_shared<NodeCloud>(std::move(nodesVector));
}

shared_ptr<NodeCloud> NodeCloudFactory::createNodeCloud(const map<unsigned short, double>& directionToDomainLength, unsigned numberOfNodes) {
    _checkInputMap(directionToDomainLength);
    auto nodesList = _initializeRandomCoordinateNodesList(directionToDomainLength, numberOfNodes);
    //can be removed since the coords are put in a map later
    _sortNodesList(nodesList, directionToDomainLength.size());
    auto nodesVector = _covertListToVectorAndAssignID(nodesList);
    return make_shared<NodeCloud>(std::move(nodesVector));
}


shared_ptr<list<shared_ptr<Node>>>
NodeCloudFactory::_initializeRandomCoordinateNodesList(const map<unsigned short, double>& directionToDomainLength, unsigned int numberOfNodes) {
    // Create a random device and use it to seed the Mersenne Twister generator
    auto distributionMap = make_shared<map<unsigned short, uniform_real_distribution<double>>>();
    for (auto &entry: directionToDomainLength) {
        (*distributionMap)[entry.first] = uniform_real_distribution<double>(0, entry.second);
    }

    auto nodesList = make_shared<list<shared_ptr<Node>>>();

    for (unsigned i = 0; i < numberOfNodes; i++) {
        auto node = make_shared<Node>();
        auto nodalCoordinates = make_shared<vector<double>>(directionToDomainLength.size());
        for (auto &entry: *distributionMap)
            (*nodalCoordinates)[entry.first - 1] = entry.second(_generator);
        node->setCoordinatesVector(std::move(nodalCoordinates));
        nodesList->push_back(std::move(node));
    }
    return nodesList;
}

shared_ptr<vector<shared_ptr<Node>>>
NodeCloudFactory::_initializeNodesVectorFromCoordinatesList(const list<vector<double>> &coordinatesList) {
    auto nodesVector = make_shared<vector<shared_ptr<Node>>>(coordinatesList.size());
    unsigned id = 0;
    for (auto &coordinates: coordinatesList) {
        auto node = make_shared<Node>();
        node->setCoordinatesVector(std::move(make_shared<vector<double>>(coordinates)));
        node->setId(id);
        (*nodesVector)[id] = std::move(node);
        id++;
    }
    return nodesVector;
}

void NodeCloudFactory::_checkInputMap(const map<unsigned short, double> &directionToDomainLength) {
    if (directionToDomainLength.size() > 3 || directionToDomainLength.size() < 2)
        throw runtime_error("Input map should contain 2 or 3 entries for directions 1 and 2 or 3");
    for (auto& entry : directionToDomainLength) {
        if (entry.second <= 0)
            throw runtime_error("Input map should contain positive values for domain lengths");
        if (entry.first != 1 && entry.first != 2 && entry.first != 3)
            throw runtime_error("Input map should contain directions 1, 2, or 3");
    }
}

void NodeCloudFactory::_sortNodesList(const shared_ptr<list<shared_ptr<Node>>>& nodesList, unsigned maximumDirections) {
    
    nodesList->sort([maximumDirections](const shared_ptr<Node>& node1, const shared_ptr<Node>& node2) {
        const auto& coords1 = *node1->getCoordinatesVector();
        const auto& coords2 = *node2->getCoordinatesVector();
        for (unsigned i = 0; i < maximumDirections; ++i) {
            if (coords1[i] < coords2[i])
                return true;
            else if (coords1[i] > coords2[i])
                return false;
        }
        return false;
    });
}

shared_ptr<vector<shared_ptr<Node>>>
NodeCloudFactory::_covertListToVectorAndAssignID(const shared_ptr<list<shared_ptr<Node>>> &nodesList) {
    auto nodesVector = make_shared<vector<shared_ptr<Node>>>(nodesList->size());
    for (unsigned iNode = 0; iNode < nodesVector->size(); ++iNode) {
        (*nodesVector)[iNode] = std::move(nodesList->front());
        nodesList->pop_front();
        (*nodesVector)[iNode]->setId(iNode);
    }
    return nodesVector;
}

