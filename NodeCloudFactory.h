//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUDFACTORY_H
#define ALTAIRINTERVIEW_NODECLOUDFACTORY_H
#include <list>
#include <random>
#include "NodeCloud.h"
#include "HardwareAcceleration/HardwareAcceleration.h"

/**
 * @class NodeCloudFactory
 * @brief Factory class for creating instances of NodeCloud.
 * 
 * Provides static factory methods to create NodeCloud objects from different types of input data. 
 */
class NodeCloudFactory {
public:
    /**
     * @brief Constructs a NodeCloudFactory object.
     */
    NodeCloudFactory() = default;

    /**
     * @brief Creates a NodeCloud from a list of coordinate vectors.
     * 
     * Each vector in the list represents a set of coordinates for a Node.
     * 
     * @param coordinatesList A list of vector<double> representing the coordinates for each Node.
     * @return shared_ptr<NodeCloud> A shared pointer to the created NodeCloud.
     * @throws runtime_error If the coordinate vectors are not of the same size or if duplicate coordinate vectors are found.
     */
    static shared_ptr<NodeCloud> createNodeCloud(const list<vector<double>>& coordinatesList);

    /**
     * @brief Creates a NodeCloud with a specified number of Nodes, each positioned randomly within given domain lengths.
     * 
     * This method generates random coordinates for each Node based on the provided domain lengths, utilizing hardware acceleration for parallel processing when desired.
     * 
     * @param directionToDomainLength A vector<double> representing the length of each dimension's domain for the random coordinates.
     * @param numberOfNodes The number of Nodes to be created within the NodeCloud.
     * @return shared_ptr<NodeCloud> A shared pointer to the created NodeCloud.
     * @throws runtime_error If the input parameters are invalid, such as an empty domain lengths vector or a non-positive number of Nodes.
     */
    static shared_ptr<NodeCloud> createNodeCloud(const vector<double> &directionToDomainLength, unsigned numberOfNodes,
                                                 unsigned availableThreads);
    
private:
    
    static void _checkRandomDomainInput(const vector<double> &directionToDomainLength, unsigned numberOfNodes,
                                 unsigned availableThreads);
};
#endif //ALTAIRINTERVIEW_NODECLOUDFACTORY_H
