//
// Created by hal9000 on 1/30/24.
//

#include "Node.h"

Node::Node() {
    _coordinates = nullptr;
    _id = make_shared<unsigned>(-1);
}

Node::~Node() {
    if (_coordinates != nullptr) {
        _coordinates.reset();
    }
    if (_id != nullptr) {
        _id.reset();
    }
}

const shared_ptr<vector<double>> &Node::getCoordinatesVector() {
    if (_coordinates == nullptr)
        throw runtime_error("Coordinates are not assigned");
    return _coordinates;
}

const shared_ptr<unsigned> &Node::getId(){
    if (_id == nullptr)
        throw runtime_error("Id is not assigned");
    return _id;
}

void Node::setCoordinatesVector(shared_ptr<vector<double>> coordinates) {
    if (coordinates == nullptr)
        throw runtime_error("Input coordinates are null");
    if (coordinates->size() < 2 || coordinates->size() > 3)
        throw runtime_error("Input Coordinates should exist in a 2 or 3 dimensional space");
    _coordinates = std::move(coordinates);
}

void Node::setId(unsigned id) {
    if (_id != nullptr)
        throw runtime_error("Id is already assigned");
}

