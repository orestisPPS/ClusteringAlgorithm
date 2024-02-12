//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUDFACTORY_H
#define ALTAIRINTERVIEW_NODECLOUDFACTORY_H

#include <list>
#include <vector>
#include <memory>
#include <random>
#include "NodeCloud.h"
#include "Node.h"

/**
    * @class NodeCloudFactory
    * Offers a set of static methods to generate NodeCloud objects, which are collections
    * of Node objects representing points in n-dimensional space. Enables the creation of
    * NodeClouds from predefined sets of coordinates or through random generation within specified domains.
 */
class NodeCloudFactory {
public:
    
    /**
     * @brief Creates a NodeCloud from a list of coordinate vectors.
     *
     * Constructs a NodeCloud by iterating over a list of coordinate vectors, where each vector
     * represents the coordinates of a Node in n-dimensional space. 
     *
     * @param coordinatesList A list of vectors, each representing the n-dimensional coordinates of a Node.
     * @return unique_ptr<NodeCloud> A unique pointer to the created NodeCloud, populated with Nodes
     *         corresponding to the provided list of coordinates.
     * @throws std::runtime_error If any two nodes have identical coordinates or if the coordinate
     *         vectors are of differing lengths, indicating inconsistent dimensions.
     */
    static unique_ptr<NodeCloud> createNodeCloud(const std::list<std::vector<double>>& coordinatesList);

    /**
     * @brief Creates a NodeCloud with nodes randomly distributed within a specified multidimensional domain.
     *
     * Generates a specified number of nodes, each with coordinates randomly distributed within the bounds of a
     * given multidimensional domain by utilizing a uniform distribution for random number generation in each
     * dimension
     *
     * Multithreading can be employed with each thread operating independently using its own random number generator

     *
     * @param dimensionsLengths A vector specifying the length of the domain in each dimension, defining
     *                          the bounds of the domain
     * @param numberOfNodes The total number of nodes to generate.
     * @param availableThreads (optional) The number of threads to use for node generation, with a default of 1.
     * @return unique_ptr<NodeCloud> A unique pointer to the created NodeCloud, containing the randomly generated nodes.
     * @throws std::runtime_error If any parameter is invalid, such as non-positive domain lengths,
     *         zero number of nodes, or an inappropriate number of threads.
     */
    static unique_ptr<NodeCloud> createNodeCloud(const vector<double> &dimensionsLengths, unsigned numberOfNodes, unsigned availableThreads = 1);


private:

    /**
    * @brief Validates input parameters for random NodeCloud generation.
    *
    * @param dimensionsLengths The lengths of each dimension in the domain.
    * @param numberOfNodes The number of nodes to generate.
    * @param availableThreads The number of threads available.
    * @throws std::runtime_error If any parameter is invalid, such as non-positive domain lengths,
    *         zero number of nodes, or an inappropriate number of threads.
    */
    static void _checkRandomDomainInput(const std::vector<double>& dimensionsLengths, unsigned int numberOfNodes,
                                        unsigned int availableThreads);

};

#endif // ALTAIRINTERVIEW_NODECLOUDFACTORY_H