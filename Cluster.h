

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

    explicit Cluster() : items(list<T>()){}

    ~Cluster() = default;

    //Move constructor
    Cluster(Cluster&& other) noexcept {
        items = other.items;
    }

    Cluster& operator=(Cluster&& other) noexcept {
        if (this != &other) {
            items = other.items;
        }
        return *this;
    }

    list<T> items;

private:
   
    
};

#endif // NODECLUSTER_H
