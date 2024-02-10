//
// Created by hal9000 on 2/10/24.
//

#ifndef ALTAIRINTERVIEW_NODECLUSTER_H
#define ALTAIRINTERVIEW_NODECLUSTER_H
#include "Node.h"
#include <list>
#include <iostream>

/**
 * @class NodeCluster
 * @brief Represents a cluster of nodes and is identified by a unique ID.
 *
 * NodeCluster encapsulates a collection of Node objects, allowing for the grouping of nodes
 * under a common identifier. 
 */
class NodeCluster {
public:
    /**
     * @brief Constructs a NodeCluster with a specified ID.
     * 
     * Initializes the NodeCluster with an empty list of nodes. The ID provided is used to uniquely
     * identify the cluster.
     * 
     * @param id The unique identifier for the NodeCluster.
     */
    explicit NodeCluster(unsigned int id);

    /**
     * @brief Gets a constant reference to the unique pointer holding the list of nodes.
     * 
     * Provides access to the list of nodes contained within the cluster and allows for manipulation
     * of the nodes, including adding new nodes or modifying existing ones.
     * 
     * @return const unique_ptr<list<shared_ptr<Node>>>& A constant reference to the unique pointer
     *         holding the list of nodes.
     */
    const unique_ptr<list<shared_ptr<Node>>>& getNodes();

    /**
     * @brief Gets the ID of the cluster
     * 
     * @return unsigned The ID of the NodeCluster.
     */
    unsigned int getId() const;

    /**
     * @brief Prints the details of the NodeCluster to standard output.
     * 
     * Iterates through each node in the cluster and prints its ID along with its index within
     * the cluster.
     */
    void printCluster() const;

private:
    unsigned int _id; ///< The unique identifier for the NodeCluster.
    
    unique_ptr<list<shared_ptr<Node>>> _nodes; ///< Dynamically allocated list of nodes within the cluster.
};



#endif //ALTAIRINTERVIEW_NODECLUSTER_H
