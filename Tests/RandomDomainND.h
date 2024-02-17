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
        _dimensions = dimensions;
        _name = "Random Domain " + to_string(_dimensions) + "D";
    }

    /**
     * @brief Tests the creation of a NodeCloud with random nodes in a domain of a given number of dimensions with length 1.
     * The test checks the following:
     * 1. The number of nodes is not 0 or different from the expected number of nodes.
     * 2. The length of all nodal coordinate vectors is the same.
     * 3. All nodes are within the domain boundaries.
     * 4. The number of clusters is as expected for a given radius. For sufficiently large clouds, the number of clusters
     *    should be 1 for a unit radius and 0 for a radius of 0.
     * @note Passes if all checks are successful and fails otherwise.
     */
    void run() override{
        _consoleTestStart();
        // Define the length of each dimension of the domain, in this case a unit square
        auto domainLengths = vector<double>(_dimensions, 1);
        // Define the number of nodes to be generated
        auto numberOfNodes = 500;
        // Create a NodeCloud with random nodes
        auto nodeCloud = NodeCloudFactory::createNodeCloud(domainLengths, numberOfNodes, 1);
        _passed = true;
        if (!_checkNodeVectorSize(nodeCloud, numberOfNodes))
            _passed = false;
        if (!_checkSameCoordinateLength(nodeCloud))
            _passed = false;
        if (!_checkNodesInDomain(nodeCloud, domainLengths))
            _passed = false;
        if (!_checkEdgeCaseClusterSize(nodeCloud, 1, 1))
            _passed = false;
        if (!_checkEdgeCaseClusterSize(nodeCloud, 0, numberOfNodes))
            _passed = false;
        _consoleTestEnd();
    }
        
private:
    
    unsigned _dimensions;

    /**
     * @brief Checks if the number of nodes is not 0 or different from the expected number of nodes.
     * @param nodeCloud NodeCloud to be checked.
     * @param expectedSize Expected number of nodes.
     * @return True if the number of nodes in the NodeCloud is as expected, false otherwise.
     */
    static bool _checkNodeVectorSize(const NodeCloud& nodeCloud, unsigned expectedSize) {
        auto _accepted = true;
        if (nodeCloud.getNodes().empty()) {
            cout << "No nodes detected in the NodeCloud" << endl;
            _accepted = false;
        }
        if (nodeCloud.getNodes().size() != expectedSize) {
            _accepted = false;
        }
        cout <<"Expected # of nodes: " << expectedSize << " Calculated # of nodes: " << nodeCloud.getNodes().size() << endl;
        return _accepted;
    }
    
    /**
     * @brief Checks if the length of all nodal coordinate vectors is the same.
     * @param nodeCloud NodeCloud to be checked.
     * @return True if all nodes have the same coordinate length, false otherwise.
     */
    static bool _checkSameCoordinateLength(const NodeCloud& nodeCloud) {
        auto nodes = nodeCloud.getNodes();
        auto commonCoordinateLength = nodes[0]->getCoordinatesVector().size();
        for (auto& node : nodes) {
            if (node->getCoordinatesVector().size() != commonCoordinateLength){
                cout << "Different coordinate vector lengths detected in the NodeCloud" << endl;
                return false;
            }
        }
        cout << "All nodal coordinate vectors have the same coordinate length" << endl;
        return true;
    }
    
    /**
     * @brief Checks if all nodes are within the domain boundaries.
     * @param nodeCloud NodeCloud to be checked.
     * @param domainLengths Lengths of the domain.
     * @return True if all nodes are within the domain boundaries, false otherwise.
     */
    static bool _checkNodesInDomain(const NodeCloud& nodeCloud, const vector<double>& domainLengths) {
        auto nodes = nodeCloud.getNodes();
        for (auto& node : nodes) {
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
    
    /**
     * @brief Checks if the number of clusters is as expected for a given radius.
     * @param nodeCloud NodeCloud to be checked.
     * @param radius Radius within which nodes are considered to be in the same cluster.
     * @param expectedSize Expected number of clusters.
     * @return True if the number of clusters is as expected, false otherwise.
     */
    static bool _checkEdgeCaseClusterSize(NodeCloud &nodeCloud, double radius, unsigned expectedSize) {
        auto clusters = nodeCloud.calculateClusters(radius);
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
