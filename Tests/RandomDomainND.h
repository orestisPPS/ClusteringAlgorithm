//
// Created by hal9000 on 2/11/24.
//

#ifndef ALTAIRINTERVIEW_RANDOMDOMAINND_H
#define ALTAIRINTERVIEW_RANDOMDOMAINND_H


#include "Test.h"

template<unsigned int dimensions, unsigned int numberOfNodes>
class RandomDomainND : public Test {
    
public:
    explicit RandomDomainND() {
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

        // Define the length of each dimension of the domain, in this case a unit square

        auto domainLengths = array<double, dimensions>();
        for (unsigned i = 0; i < dimensions; i++)
            domainLengths[i] = 1;
        auto algortihms = vector<ClusteringAlgorithmType> {
                UNION_FIND_PER_NODE,
                UNION_FIND_BUNCH,
                DEPTH_FIRST_SEARCH
        };
        auto algorithmNames = vector<string>{
                "Union Find Per Node",
                "Union Find Bunch",
                "Depth First Search"
        };
        for (unsigned i = 0; i < algortihms.size(); i++) {
            _name = "Random Domain " + to_string(_dimensions) + "D " + algorithmNames[i];
            _consoleTestStart();
            // Create a NodeCloud with random nodes
            auto nodeCloud = NodeCloud<dimensions, numberOfNodes>(domainLengths, 1, 0);
            _passed = true;
            if (!_checkNodeVectorSize(nodeCloud, numberOfNodes))
                _passed = false;
            if (!_checkNodesInDomain(nodeCloud, domainLengths))
                _passed = false;
            if (!_checkEdgeCaseClusterSize(nodeCloud, 1, 1, algortihms[i]))
                _passed = false;
            if (!_checkEdgeCaseClusterSize(nodeCloud, 0, numberOfNodes, algortihms[i]))
                _passed = false;
            _consoleTestEnd();
        }
    }

private:

    unsigned _dimensions;

    /**
     * @brief Checks if the number of nodes is not 0 or different from the expected number of nodes.
     * @param nodeCloud NodeCloud to be checked.
     * @param expectedSize Expected number of nodes.
     * @return True if the number of nodes in the NodeCloud is as expected, false otherwise.
     */
    static bool _checkNodeVectorSize(const NodeCloud<dimensions, numberOfNodes>& nodeCloud, unsigned expectedSize) {
        auto _accepted = true;
        if (nodeCloud.getNodes().empty()) {
            cout << "No nodes detected in the NodeCloud" << endl;
            _accepted = false;
        }
        if (nodeCloud.getNodes().size() != expectedSize) {
            _accepted = false;
        }
        cout <<"Number of nodes [Expected / Calculated] : [" << expectedSize << " / " << nodeCloud.getNodes().size() << "]" << endl;
        return _accepted;
    }
    
    /**
     * @brief Checks if all nodes are within the domain boundaries.
     * @param nodeCloud NodeCloud to be checked.
     * @param domainLengths Lengths of the domain.
     * @return True if all nodes are within the domain boundaries, false otherwise.
     */
    static bool _checkNodesInDomain(const NodeCloud<dimensions, numberOfNodes>& nodeCloud, const array<double, dimensions>& domainLengths) {
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
    static bool _checkEdgeCaseClusterSize(NodeCloud<dimensions, numberOfNodes> &nodeCloud, double radius, unsigned expectedSize, ClusteringAlgorithmType algorithm) {
        auto clusters = nodeCloud.findClusters(radius, algorithm);
        bool accepted = true;
        if (clusters.size() != expectedSize) {
            accepted = false;
        }
        cout << "Length Scale : " << radius << " : Clusters [Expected / Calculated] : [" << expectedSize << " / " << clusters.size() << "]" << endl;
        return accepted;
    }
};


#endif //ALTAIRINTERVIEW_RANDOMDOMAINND_H
