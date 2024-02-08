//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUDFACTORY_H
#define ALTAIRINTERVIEW_NODECLOUDFACTORY_H
#include <list>
#include <random>
#include "NodeCloud.h"
#include "HardwareAcceleration/ThreadingOperations.h"

class NodeCloudFactory {
public:
    
    NodeCloudFactory() = default;

    static shared_ptr<NodeCloud> createNodeCloud(const list<vector<double>>& coordinatesList);

    static shared_ptr<NodeCloud> createNodeCloud(const vector<double> &directionToDomainLength, unsigned numberOfNodes);
};


#endif //ALTAIRINTERVIEW_NODECLOUDFACTORY_H
