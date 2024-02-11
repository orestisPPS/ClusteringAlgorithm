//
// Created by hal9000 on 2/11/24.
//

#ifndef ALTAIRINTERVIEW_RANDOMDOMAINND_H
#define ALTAIRINTERVIEW_RANDOMDOMAINND_H


#include "Test.h"

class RandomDomainND : public Test {
public:
    explicit RandomDomainND(unsigned dimensions = 2) {
        if (dimensions < 2) {
            throw invalid_argument("RandomDomainND: Dimensions must be at least 2");
        }
        _name = "Random Domain 2D Test";
        _dimensions = dimensions;
    }

    void run() override{
        _consoleTestStart();
        // Define the length of each dimension of the domain, in this case a unit square
        auto domainLengths = vector<double>(_dimensions, 1);
        // Define the number of nodes to be generated
        auto numberOfNodes = 100;
        // Create a NodeCloud with random nodes
        auto nodeCloud = NodeCloudFactory::createNodeCloud(domainLengths, numberOfNodes, 1);
        _passed = true;
        if (!_checkNodeVectorSize(nodeCloud, numberOfNodes))
            _passed = false;
        if (!_checkSameCoordinateLength(nodeCloud))
            _passed = false;
        if (!_checkNodesInDomain(nodeCloud, domainLengths))
            _passed = false;
        if (!_checkEdgeClusterSize(nodeCloud, 1, 1))
            _passed = false;
        if (!_checkEdgeClusterSize(nodeCloud, 0, numberOfNodes))
            _passed = false;
    }
    
private:
    
    unsigned _dimensions;
    
    static bool _checkNodeVectorSize(const unique_ptr<NodeCloud>& nodeCloud, unsigned expectedSize) {
        auto _accepted = true;
        if (nodeCloud->getNodes()->empty()) {
            cout << "No nodes detected in the NodeCloud" << endl;
            _accepted = false;
        }
        if (nodeCloud->getNodes()->size() != expectedSize) {
            _accepted = false;
        }
        cout <<"Expected # of nodes: " << expectedSize << " Calculated # of nodes: " << nodeCloud->getNodes()->size() << endl;
        return _accepted;
    }
    
    static bool _checkSameCoordinateLength(const unique_ptr<NodeCloud>& nodeCloud) {
        auto nodes = nodeCloud->getNodes();
        auto commonCoordinateLength = (*nodes)[0]->getCoordinatesVector().size();
        for (auto& node : *nodes) {
            if (node->getCoordinatesVector().size() != commonCoordinateLength){
                cout << "Different coordinate vector lengths detected in the NodeCloud" << endl;
                return false;
            }
        }
        cout << "All nodal coordinate vectors have the same coordinate length" << endl;
        return true;
    }
    
    static bool _checkNodesInDomain(const unique_ptr<NodeCloud>& nodeCloud, const vector<double>& domainLengths) {
        auto nodes = nodeCloud->getNodes();
        for (auto& node : *nodes) {
            auto &coordinates = node->getCoordinatesVector();
            for (unsigned i = 0; i < coordinates.size(); i++) {
                if ((coordinates)[i] < 0 || (coordinates)[i] > domainLengths[i]) {
                    cout << "Node outside of domain detected in the NodeCloud" << endl;
                    cout << "Node coordinates: " << (coordinates)[0] << " " << (coordinates)[1] << endl;
                    return false;
                }
            }
        }
        cout << "All nodes are within the domain" << endl;
        return true;
    }

    static bool _checkEdgeClusterSize(const unique_ptr<NodeCloud>& nodeCloud, double radius, unsigned expectedSize) {
        auto clusters = nodeCloud->calculateClusters(radius);
        bool accepted = true;
        if (clusters.size() != expectedSize) {
            accepted = false;
        }
        cout << "Length Scale : " << radius << " Calculated Cluster size : " << clusters.size() <<
             " Expected Cluster size : " << expectedSize << endl;
        return accepted;
    }
};


#endif //ALTAIRINTERVIEW_RANDOMDOMAINND_H
