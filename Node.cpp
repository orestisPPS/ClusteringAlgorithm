//
// Created by hal9000 on 1/30/24.
//

#include "Node.h"

Node::Node(unsigned id) : _id(id) {}

unsigned Node::getId() const{
    return _id;
}

const shared_ptr<vector<double>>& Node::getCoordinatesVector() const{
    if (_coordinates == nullptr)
        throw runtime_error("Coordinates are not assigned");
    return _coordinates;
}


void Node::setCoordinatesVector(shared_ptr<vector<double>> coordinates) {
    if (coordinates == nullptr)
        throw runtime_error("Input coordinates are null");
    if (coordinates->empty())
        throw runtime_error("Input coordinates are empty");
    _coordinates = std::move(coordinates);
}


