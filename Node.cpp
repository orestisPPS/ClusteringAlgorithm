//
// Created by hal9000 on 1/30/24.
//

#include "Node.h"

Node::Node(unsigned id) : _id(id), _coordinates(make_unique<vector<double>>()){
}

unsigned Node::getId() const{
    return _id;
}

const vector<double> & Node::getCoordinatesVector() const{
    return *_coordinates;
}

void Node::setCoordinatesVector(unique_ptr<vector<double>> coordinates) {
    if (coordinates == nullptr)
        throw runtime_error("Input coordinates are null");
    if (coordinates->empty())
        throw runtime_error("Input coordinates are empty");
    _coordinates = move(coordinates);
}


