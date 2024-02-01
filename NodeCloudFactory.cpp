//
// Created by hal9000 on 1/30/24.
//

#include "NodeCloudFactory.h"


NodeCloudFactory::NodeCloudFactory() : _generator(_randomDevice()) {
}

shared_ptr<NodeCloud2D> NodeCloudFactory::createNodeCloud(list<vector<double>> coordinatesList) {
    return shared_ptr<NodeCloud2D>();
}

shared_ptr<NodeCloud2D> NodeCloudFactory::createNodeCloud2D(unsigned domainLength1, unsigned domainLength2, unsigned int numberOfNodes) {
    auto nodesVector = _initializeRandom2DNodesVector(domainLength1, domainLength2, numberOfNodes);
}


shared_ptr<vector<shared_ptr<Node>>>
NodeCloudFactory::_initializeRandom2DNodesVector(unsigned int domainLength1, unsigned int domainLength2,
                                                 unsigned int numberOfNodes) {
    auto nodesVector = make_shared<vector<shared_ptr<Node>>>(numberOfNodes);

    // Create a random device and use it to seed the Mersenne Twister generator
    std::uniform_real_distribution<double> distribution1(0, domainLength1);
    std::uniform_real_distribution<double> distribution2(0, domainLength2);

    for (unsigned i = 0; i < numberOfNodes; i++) {
        auto node = make_shared<Node>();
        auto nodalCoordinates = make_shared<vector<double>>(2);
        (*nodalCoordinates)[0] = distribution1(_generator);
        (*nodalCoordinates)[1] = distribution2(_generator);
        node->setCoordinatesVector(std::move(nodalCoordinates));
        (*nodesVector)[i] = std::move(node);
    }

    bool duplicateFound;
    do {
        duplicateFound = false;
        for (size_t i = 0; i < nodesVector->size() - 1; ++i) {
            const auto& coords1 = *(*nodesVector)[i]->getCoordinatesVector();
            const auto& coords2 = *(*nodesVector)[i + 1]->getCoordinatesVector();
            if (coords1 == coords2) {
                duplicateFound = true;
                auto newCoordinates = make_shared<vector<double>>(2);
                (*newCoordinates)[0] = distribution1(_generator);
                (*newCoordinates)[1] = distribution2(_generator);
                (*nodesVector)[i + 1]->setCoordinatesVector(std::move(newCoordinates));
            }
        }
    } while (duplicateFound);
    
}

void NodeCloudFactory::_sortNodesVector(shared_ptr<vector<shared_ptr<Node>>> nodesVector) {

}



