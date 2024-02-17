////
//// Created by hal9000 on 2/10/24.
////
//
//#include "NodeCluster.h"
//
//NodeCluster::NodeCluster(unsigned int id) : _id(id), _nodes(new list<Node*>()){}
//
//
//NodeCluster::~NodeCluster() {
//    delete _nodes;
//    _nodes = nullptr;
//}
//
//NodeCluster::NodeCluster(NodeCluster &&other) noexcept {
//    _id = other._id;
//    _nodes = other._nodes;
//    other._nodes = nullptr;
//}
//
//
//list<Node*> & NodeCluster::getNodes(){
//    return *_nodes;
//}
//
//unsigned int NodeCluster::getId() const {
//    return _id;
//}
//
//
//
//
