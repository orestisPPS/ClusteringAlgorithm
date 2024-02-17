//
// Created by hal9000 on 2/10/24.
//

#include "NodeCluster.h"

NodeCluster::NodeCluster(unsigned int id) : _id(id), _nodes(make_unique<list<Node*>>()){
}

list<Node*> & NodeCluster::getNodes(){
    return *_nodes;
}

unsigned int NodeCluster::getId() const {
    return _id;
}




