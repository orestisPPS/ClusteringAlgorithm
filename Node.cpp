//
// Created by hal9000 on 1/30/24.
//

#include "Node.h"

Node::Node(vector<double> coordinates) : _coordinates(std::move(coordinates)){}


const vector<double> & Node::getCoordinatesVector() const{
    return _coordinates;
}


