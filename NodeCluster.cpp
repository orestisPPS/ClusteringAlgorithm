//
// Created by hal9000 on 2/10/24.
//

#include "NodeCluster.h"

NodeCluster::NodeCluster(unsigned int id) : _id(id), _nodes(make_unique<list<shared_ptr<Node>>>()){
}

list<shared_ptr<Node>> & NodeCluster::getNodes() const{
    return *_nodes;
}

unsigned int NodeCluster::getId() const {
    return _id;
}

void NodeCluster::printCluster() const {
    cout << "Cluster: " << _id << endl;
    unsigned index = 0;
    for (auto &node: *_nodes) {
        cout << "[" << index << "] "<< "Node: " << node->getId() << endl;
        index++;
    }
    cout << endl;
}



