//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUD_H
#define ALTAIRINTERVIEW_NODECLOUD_H


#include <unordered_map>
#include <map>
#include <cmath>
#include <mutex>
#include "NodeFactory.h"
#include "NodeCluster.h"
#include "ClusteringAlgorithms/UnionFindBunchClustering.h"
#include "ClusteringAlgorithms/UnionFindPerNodeClustering.h"

template<unsigned short dimensions, unsigned numberOfNodes>
class NodeCloud {
public:
    
    NodeCloud(array<double, dimensions> dimensionsLengths, unsigned availableThreads, int sortByDimension = -1) {
        // Allocate contiguous memory for the nodes
        _blockOfNodes = new char[sizeof(Node<dimensions>) * numberOfNodes];
        NodeFactory<dimensions, numberOfNodes>::createRandomNodes<numberOfNodes>(dimensionsLengths, _nodes, _blockOfNodes, sortByDimension, availableThreads);
    }
    
    NodeCloud(const list<array<double, dimensions>>& coordinates, unsigned availableThreads, int sortByDimension = -1) {
        // Allocate contiguous memory for the nodes
        _blockOfNodes = new char[sizeof(Node<dimensions>) * coordinates.size()];
        NodeFactory<dimensions, numberOfNodes>::createNodesFromInputCoordinates(coordinates, _nodes, _blockOfNodes, sortByDimension, availableThreads);
    }
    
    ~NodeCloud() {
        for (unsigned i = 0; i < numberOfNodes; i++) {
            _nodes[i]->~Node();
        }
        delete[] _blockOfNodes;
    }
    
    const array<Node<dimensions>*, numberOfNodes>& getNodes() const {
        return _nodes;
    }
    
    list<NodeCluster<dimensions>> findClusters(double radius, ClusteringAlgorithmType algorithm, unsigned availableThreads = 1) {
        switch (algorithm) {
            case ClusteringAlgorithmType::UNION_FIND_PER_NODE:
                return std::move(UnionFindPerNodeClustering<dimensions, numberOfNodes>(_nodes).findClusters(radius, availableThreads));
            case UNION_FIND_BUNCH:
                return std::move(UnionFindBunchClustering<dimensions, numberOfNodes>(_nodes).findClusters(radius, availableThreads));
            default:
                throw std::invalid_argument("Invalid clustering algorithm type");
        }
    }
    
private:
    char* _blockOfNodes; // Contiguous heap memory for Node objects

    array<Node<dimensions>*, numberOfNodes> _nodes; // Array of pointers to Node objects
    
};




#endif //ALTAIRINTERVIEW_NODECLOUD_H
