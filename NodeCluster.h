

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
 template <unsigned dimensions>
class NodeCluster {
public:
    /**
     * @brief Constructs a new NodeCluster with a given ID.
     * @param id The unique identifier for the cluster.
     */
    explicit NodeCluster(unsigned int id) : _id(id), _nodes(new list<Node<dimensions>*>()){}

    ~NodeCluster(){
        delete _nodes;
        _nodes = nullptr;
    }

    //Move constructor
    NodeCluster(NodeCluster&& other) noexcept {
        _id = other._id;
        _nodes = other._nodes;
        other._nodes = nullptr;
    }

    NodeCluster& operator=(NodeCluster&& other) noexcept {
        if (this != &other) {
            delete _nodes;
            _id = other._id;
            _nodes = other._nodes;
            other._nodes = nullptr;
        }
        return *this;
    }


    /**
     * @brief Gets the list of nodes in the cluster.
     * @return A reference to the list of Node* objects.
     */
    list<Node<dimensions>*>& getNodes(){
        return *_nodes;
    }

    /**
     * @brief Gets the unique ID of the cluster.
     * @return The cluster's unique ID.
     */
    unsigned int getId() const {
        return _id;
    }


private:
    //list<Node*>* _nodes; ///< List of nodes of the cluster.
    list<Node<dimensions>*>* _nodes; ///< List of nodes of the cluster.

    unsigned int _id; ///< Unique identifier for the cluster.
};

#endif // NODECLUSTER_H
