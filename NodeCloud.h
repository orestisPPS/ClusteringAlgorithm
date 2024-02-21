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
#include "Cluster.h"
#include "ClusteringAlgorithms/UnionFindBunchClustering.h"
#include "ClusteringAlgorithms/UnionFindPerNodeClustering.h"
#include "ClusteringAlgorithms/DepthFirstSearchClustering.h"

template<unsigned short dimensions, unsigned numberOfNodes>
class NodeCloud {
public:
    
    explicit NodeCloud(array<double, dimensions> dimensionsLengths,unsigned availableThreads, int sortByDimension = -1) {

        _checkNumberOfThreads(availableThreads);
        _availableThreads = availableThreads;
        // Allocate contiguous memory for the nodes
        _blockOfNodes = new char[sizeof(Node<dimensions>) * numberOfNodes];
        NodeFactory<dimensions, numberOfNodes>::createRandomNodes(dimensionsLengths, _nodes, _blockOfNodes, sortByDimension, availableThreads);
    }
    
    explicit NodeCloud(const list<array<double, dimensions>>& coordinates, unsigned availableThreads, int sortByDimension = -1) {
        _checkNumberOfThreads(availableThreads);
        _availableThreads = availableThreads;
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
    
    list<Cluster<Node<dimensions> *>> findClusters(double radius, ClusteringAlgorithmType algorithm) {
        switch (algorithm) {
            case ClusteringAlgorithmType::UNION_FIND_PER_NODE:
                return std::move(UnionFindPerNodeClustering<dimensions, numberOfNodes>(_nodes).findClusters(radius, _availableThreads));
            case UNION_FIND_BUNCH:
                return std::move(UnionFindBunchClustering<dimensions, numberOfNodes>(_nodes).findClusters(radius, _availableThreads));
            case DEPTH_FIRST_SEARCH:
                return std::move(DepthFirstSearchClustering<dimensions, numberOfNodes>(_nodes).findClusters(radius, _availableThreads));
            default:
                throw std::invalid_argument("Invalid algorithm type");
        }
    }
    
private:
    
    char* _blockOfNodes; // Contiguous heap memory for Node objects

    array<Node<dimensions>*, numberOfNodes> _nodes; // Array of pointers to Node objects
    
    unsigned _availableThreads;
    
    static void _checkNumberOfThreads(unsigned availableThreads) {
        if (availableThreads <= 0 || availableThreads > ThreadingOperations<void>::getAvailableThreads())
            throw std::invalid_argument("Threads should be between 1 and " + to_string(ThreadingOperations<void>::getAvailableThreads()));
    }
    
};




#endif //ALTAIRINTERVIEW_NODECLOUD_H
