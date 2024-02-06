//
// Created by hal9000 on 1/30/24.
//

#include "NodeCloudFactory.h"

shared_ptr<NodeCloud> NodeCloudFactory::createNodeCloud(const list<vector<double>>& coordinatesList) {
    auto nodesVector = make_shared<vector<shared_ptr<Node>>>(coordinatesList.size());
    unsigned id = 0;
    for (auto &coordinates: coordinatesList) {
        auto node = make_shared<Node>();
        node->setCoordinatesVector(std::move(make_shared<vector<double>>(coordinates)));
        node->setId(id);
        (*nodesVector)[id] = std::move(node);
        id++;
    }
    return make_shared<NodeCloud>(std::move(nodesVector));
}

shared_ptr<NodeCloud> NodeCloudFactory::createNodeCloud(const vector<double> &directionToDomainLength, unsigned numberOfNodes) {
    if (directionToDomainLength.empty() || numberOfNodes == 0)
        throw runtime_error("Input parameters are invalid");
    for (auto &length: directionToDomainLength)
        if (length <= 0)
            throw runtime_error("Domain length should be greater than 0");
    //Random device to seed the Mersenne Twister generator
    random_device randomDevice;
    mt19937 generator(randomDevice());
    auto distributionsVector = vector<uniform_real_distribution<double>>(directionToDomainLength.size());
    for (unsigned i = 0; i < directionToDomainLength.size(); i++)
        distributionsVector[i] = uniform_real_distribution<double>(0, directionToDomainLength[i]);

    auto nodesVector = make_shared<vector<shared_ptr<Node>>>(numberOfNodes);
    for (unsigned i = 0; i < numberOfNodes; i++) {
        auto node = make_shared<Node>();
        auto nodalCoordinates = make_shared<vector<double>>(directionToDomainLength.size());
        for (unsigned j = 0; j < directionToDomainLength.size(); j++)
            (*nodalCoordinates)[j] = distributionsVector[j](generator);
        node->setCoordinatesVector(std::move(nodalCoordinates));
        node->setId(i);
        (*nodesVector)[i] = std::move(node);
    }
    return make_shared<NodeCloud>(std::move(nodesVector));
}