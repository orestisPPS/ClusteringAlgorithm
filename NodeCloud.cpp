//
// Created by hal9000 on 1/30/24.
//

#include "NodeCloud.h"

NodeCloud::NodeCloud(shared_ptr<vector<shared_ptr<Node>>> nodesVector) : _nodesVector(std::move(nodesVector)) {}

NodeCloud::~NodeCloud() {
    if (_nodesVector != nullptr) {
        _nodesVector.reset();
    }
}

