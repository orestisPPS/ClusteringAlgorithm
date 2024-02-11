//
// Created by hal9000 on 1/30/24.
//

#include "NodeCloudFactory.h"

shared_ptr<NodeCloud> NodeCloudFactory::createNodeCloud(const list<vector<double>>& coordinatesList) {
    auto nodesVector = make_shared<vector<shared_ptr<Node>>>(coordinatesList.size());
    auto commonCoordinatesSize = coordinatesList.front().size();
    // Iterate through each set of coordinates in the list to ensure they all meet certain criteria
    for (auto thisCoordIt = coordinatesList.begin(); thisCoordIt != coordinatesList.end(); ++thisCoordIt) {
        const auto& coordinates = *thisCoordIt;

        if (coordinates.size() != commonCoordinatesSize) {
            throw std::runtime_error("Coordinates have different sizes");
        }
        for (auto otherCoordIt = next(thisCoordIt); otherCoordIt != coordinatesList.end(); ++otherCoordIt) {
            const auto& coordinates2 = *otherCoordIt;
            if (coordinates == coordinates2)
                throw std::runtime_error("Coordinates are not unique");
        }
    }

    
    unsigned id = 0;
    for (auto &coordinates: coordinatesList) {
        auto node = make_shared<Node>(id);
        node->setCoordinatesVector(std::move(make_shared<vector<double>>(coordinates)));
        (*nodesVector)[id] = std::move(node);
        id++;
    }
    return make_shared<NodeCloud>(std::move(nodesVector));
}

shared_ptr<NodeCloud>
NodeCloudFactory::createNodeCloud(const vector<double> &directionToDomainLength, unsigned numberOfNodes,
                                  unsigned availableThreads) {
    _checkRandomDomainInput(directionToDomainLength, numberOfNodes, availableThreads);
    //Create a vector of uniform_real_distribution objects for each dimension length [0, length]
    auto distributionsVector = vector<uniform_real_distribution<double>>(directionToDomainLength.size());
    for (unsigned i = 0; i < directionToDomainLength.size(); i++)
        distributionsVector[i] = uniform_real_distribution<double>(0, directionToDomainLength[i]);
    auto nodesVector = make_shared<vector<shared_ptr<Node>>>(numberOfNodes);
    
    auto nodeInitializationJob = [&](unsigned start, unsigned end) {
        //Random device to seed the Mersenne Twister generator
        random_device randomDevice;
        mt19937 generator(randomDevice());
        for (unsigned i = start; i < end; i++) {
            auto node = make_shared<Node>(i);
            auto nodalCoordinates = make_shared<vector<double>>(directionToDomainLength.size());
            for (unsigned j = 0; j < directionToDomainLength.size(); j++)
                (*nodalCoordinates)[j] = distributionsVector[j](generator);
            node->setCoordinatesVector(std::move(nodalCoordinates));
            //TODO - Investigate why std::move over assignent
            (*nodesVector)[i] = node; //std::move(node);
        }
    };
    HardwareAcceleration<shared_ptr<Node>>::executeParallelJob(nodeInitializationJob, nodesVector->size(), availableThreads);
    return make_shared<NodeCloud>(std::move(nodesVector));
}

void
NodeCloudFactory::_checkRandomDomainInput(const vector<double> &directionToDomainLength, unsigned int numberOfNodes,
                                          unsigned int availableThreads) {
    if (directionToDomainLength.empty() || numberOfNodes == 0)
        throw runtime_error("Input parameters are invalid");
    for (auto &length: directionToDomainLength)
        if (length <= 0)
            throw runtime_error("Domain length should be greater than 0");
    if (availableThreads == 0)
        throw runtime_error("Number of threads should be greater than 0");
    if (availableThreads > thread::hardware_concurrency())
        throw runtime_error("Number of threads should be less than or equal to the number of hardware threads");
}
