

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
 template <typename T>
class Cluster {
public:
    /**
     * @brief Constructs a new NodeCluster with a given ID.
     * @param id The unique identifier for the cluster.
     */
    explicit Cluster(unsigned int id) : _id(id), _items(list<T>()){}

    ~Cluster() = default;

    //Move constructor
    Cluster(Cluster&& other) noexcept {
        _id = other._id;
        _items = other._items;
    }

    Cluster& operator=(Cluster&& other) noexcept {
        if (this != &other) {
            _id = other._id;
            _items = other._items;
        }
        return *this;
    }

    list<T>& getList(){
        return _items;
    }
    
    unsigned int getId() const {
        return _id;
    }


private:
    list<T> _items; ///< List of nodes of the cluster.

    unsigned int _id; ///< Unique identifier for the cluster.
};

#endif // NODECLUSTER_H
