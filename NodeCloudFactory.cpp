//
// Created by hal9000 on 1/30/24.
//

#include "NodeCloudFactory.h"

NodeCloud NodeCloudFactory::createNodeCloud(const list<vector<double>> &coordinatesList) {
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

    auto nodesVector = vector<Node*>(coordinatesList.size());
    unsigned id = 0;
    for (auto &coordinates: coordinatesList) {
        nodesVector[id] = new Node(coordinates);
        id++;
    }
    return NodeCloud(std::move(nodesVector));
}

NodeCloud
NodeCloudFactory::createNodeCloud(const vector<double> &dimensionsLengths, unsigned numberOfNodes,
                                  unsigned availableThreads) {
    _checkRandomDomainInput(dimensionsLengths, numberOfNodes, availableThreads);
    //Create a vector of uniform_real_distribution objects for each dimension length [0, length]
    auto distributionsVector = vector<uniform_real_distribution<double>>(dimensionsLengths.size());
    for (unsigned i = 0; i < dimensionsLengths.size(); i++)
        distributionsVector[i] = uniform_real_distribution<double>(0, dimensionsLengths[i]);
    
    auto nodesVector = vector<Node*>(numberOfNodes);
    auto nodeInitializationJob = [&](unsigned start, unsigned end) {
        //Random device to seed the Mersenne Twister generator
        random_device randomDevice;
        mt19937 generator(randomDevice());
        for (unsigned i = start; i < end; i++) {
            auto nodalCoordinates = make_unique<vector<double>>(dimensionsLengths.size());
            for (unsigned j = 0; j < dimensionsLengths.size(); j++)
                (*nodalCoordinates)[j] = distributionsVector[j](generator);
            nodesVector[i] = new Node(std::move(*nodalCoordinates));
        }
    };
    ThreadingOperations<Node*>::executeParallelJob(nodeInitializationJob, nodesVector.size(), availableThreads);
    return NodeCloud(std::move(nodesVector));
}

void
NodeCloudFactory::_checkRandomDomainInput(const vector<double> &dimensionsLengths, unsigned int numberOfNodes,
                                          unsigned int availableThreads) {
    if (dimensionsLengths.empty() || numberOfNodes == 0)
        throw runtime_error("Input parameters are invalid");
    for (auto &length: dimensionsLengths)
        if (length <= 0)
            throw runtime_error("Domain length should be greater than 0");
    if (availableThreads == 0)
        throw runtime_error("Number of threads should be greater than 0");
    if (availableThreads > thread::hardware_concurrency())
        throw runtime_error("Number of threads should be less than or equal to the number of hardware threads");
}
