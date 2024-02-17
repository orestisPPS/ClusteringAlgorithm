

#ifndef NODECLUSTER_H
#define NODECLUSTER_H

#include <memory>
#include <list>
#include <iostream>
#include "Node.h"
/**
 * @brief Defines the NodeCluster class, representing a collection of Node objects.
 * A NodeCluster is identified by a unique ID and collects multiple Node object pointers into a single entity.
 */
class NodeCluster {
public:
    /**
     * @brief Constructs a new NodeCluster with a given ID.
     * @param id The unique identifier for the cluster.
     */
    explicit NodeCluster(unsigned int id);

    /**
     * @brief Gets the list of nodes in the cluster.
     * @return A reference to the list of Node* objects.
     */
    list<Node*>& getNodes();

    /**
     * @brief Gets the unique ID of the cluster.
     * @return The cluster's unique ID.
     */
    unsigned int getId() const;

    /**
     * @brief Prints the details of the cluster to standard output.
     * 
     * Iterates over all nodes in the cluster, printing the cluster id and each node's ID. 
     */
    void printCluster() const;

private:
    unique_ptr<list<Node*>> _nodes; ///< List of nodes of the cluster.
    unsigned int _id; ///< Unique identifier for the cluster.
};

#endif // NODECLUSTER_H
