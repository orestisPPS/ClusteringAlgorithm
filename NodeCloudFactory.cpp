////
//// Created by hal9000 on 1/30/24.
////
//
//#include "NodeCloudFactory.h"
//
//NodeCloud NodeCloudFactory::createNodeCloud(const list<vector<double>> &coordinatesList) {
//    auto commonCoordinatesSize = coordinatesList.front().size();
//    // Iterate through each set of coordinates in the list to ensure they all meet certain criteria
//    for (auto thisCoordIt = coordinatesList.begin(); thisCoordIt != coordinatesList.end(); ++thisCoordIt) {
//        const auto& coordinates = *thisCoordIt;
//
//        if (coordinates.size() != commonCoordinatesSize) {
//            throw std::runtime_error("Coordinates have different sizes");
//        }
//        for (auto otherCoordIt = next(thisCoordIt); otherCoordIt != coordinatesList.end(); ++otherCoordIt) {
//            const auto& coordinates2 = *otherCoordIt;
//            if (coordinates == coordinates2)
//                throw std::runtime_error("Coordinates are not unique");
//        }
//    }
//
//    auto nodesVector = vector<Node*>(coordinatesList.size());
//    unsigned id = 0;
//    for (auto &coordinates: coordinatesList) {
//        nodesVector[id] = new Node(coordinates);
//        id++;
//    }
//    return NodeCloud(std::move(nodesVector));
//}
//
//NodeCloud
//NodeCloudFactory::createNodeCloud(const vector<double> &dimensionsLengths, unsigned numberOfNodes,
//                                  unsigned availableThreads) {
//    _checkRandomDomainInput(dimensionsLengths, numberOfNodes, availableThreads);
//    
//    return NodeCloud(std::move(nodesVector));
//}
//
//void
//NodeCloudFactory::_checkRandomDomainInput(const vector<double> &dimensionsLengths, unsigned int numberOfNodes,
//                                          unsigned int availableThreads) {
//    if (dimensionsLengths.empty() || numberOfNodes == 0)
//        throw runtime_error("Input parameters are invalid");
//    for (auto &length: dimensionsLengths)
//        if (length <= 0)
//            throw runtime_error("Domain length should be greater than 0");
//    if (availableThreads == 0)
//        throw runtime_error("Number of threads should be greater than 0");
//    if (availableThreads > thread::hardware_concurrency())
//        throw runtime_error("Number of threads should be less than or equal to the number of hardware threads");
//}
