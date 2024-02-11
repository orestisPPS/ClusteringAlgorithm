//
// Created by hal9000 on 2/11/24.
//

#ifndef ALTAIRINTERVIEW_RANDOMDOMAIN2DTEST_H
#define ALTAIRINTERVIEW_RANDOMDOMAIN2DTEST_H


#include "Test.h"

class RandomDomain2DTest : public Test {
public:
    RandomDomain2DTest() {
        _name = "Random Domain 2D Test";
    }

    void run() override{
        _consoleTestStart();
        // Define the length of each dimension of the domain, in this case a unit square
        auto domainLengths = vector<double>{1, 1};
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
        
        _consoleTestEnd();
    }
    
private:
    
    static bool _checkNodeVectorSize(const shared_ptr<NodeCloud>& nodeCloud, unsigned expectedSize) {
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
    
    static bool _checkSameCoordinateLength(const shared_ptr<NodeCloud>& nodeCloud) {
        auto nodes = nodeCloud->getNodes();
        auto commonCoordinateLength = (*nodes)[0]->getCoordinatesVector()->size();
        for (auto& node : *nodes) {
            if (node->getCoordinatesVector()->size() != commonCoordinateLength){
                cout << "Different coordinate vector lengths detected in the NodeCloud" << endl;
                return false;
            }
        }
        cout << "All nodal coordinate vectors have the same coordinate length" << endl;
        return true;
    }
    
    static bool _checkNodesInDomain(const shared_ptr<NodeCloud>& nodeCloud, const vector<double>& domainLengths) {
        auto nodes = nodeCloud->getNodes();
        for (auto& node : *nodes) {
            auto coordinates = node->getCoordinatesVector();
            for (unsigned i = 0; i < coordinates->size(); i++) {
                if ((*coordinates)[i] < 0 || (*coordinates)[i] > domainLengths[i]) {
                    cout << "Node outside of domain detected in the NodeCloud" << endl;
                    cout << "Node coordinates: " << (*coordinates)[0] << " " << (*coordinates)[1] << endl;
                    return false;
                }
            }
        }
        cout << "All nodes are within the domain" << endl;
        return true;
    }
};


#endif //ALTAIRINTERVIEW_RANDOMDOMAIN2DTEST_H
