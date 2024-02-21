//
// Created by hal9000 on 2/21/24.
//

#ifndef ALTAIRINTERVIEW_DEPTHFIRSTSEARCHCLUSTERING_H
#define ALTAIRINTERVIEW_DEPTHFIRSTSEARCHCLUSTERING_H
#include "ClusteringAlgorithm.h"

template<unsigned dimensions, unsigned numberOfNodes>
class DepthFirstSearchClustering : public ClusteringAlgorithm<dimensions, numberOfNodes> {
public:
    explicit DepthFirstSearchClustering(const array<Node<dimensions>*, numberOfNodes> &nodes) : ClusteringAlgorithm<dimensions, numberOfNodes>(nodes){
        for (unsigned i = 0; i < numberOfNodes; i++) {
            for (unsigned dimension = 0; dimension < dimensions; dimension++)
                this->_coordinateComponentToNodeMaps[dimension][this->_nodes[i]->getCoordinatesVector()[dimension]].push_back(this->_nodes[i]);
            _visitedStatus[this->_nodes[i]] = false;
            this->_nodeToNeighboursMap[this->_nodes[i]] = list<Node<dimensions>*>();
        }
    }
    
    ~DepthFirstSearchClustering() = default;
    
    list<Cluster<Node<dimensions> *>> findClusters(double radius, unsigned availableThreads) override {
        auto threadJob = [&](unsigned start, unsigned end){
            for (unsigned i = start; i < end; i++)
                this->_findNeighboursWithMapBounds(this->_nodes[i], radius);
        };
        ThreadingOperations<void>::executeParallelJob(threadJob, numberOfNodes, availableThreads);
        
        auto clusterId = 0;
        auto clusters = list<Cluster<Node<dimensions> *>>();
        for (auto &node : this->_nodes){
            if (!_visitedStatus[node]){
                auto cluster = Cluster<Node<dimensions> *>(clusterId);
                _depthFirstSearch(node, cluster);
                clusters.push_back(std::move(cluster));
                clusterId++;
            }
        }
        for (auto &node : this->_nodes){
            _visitedStatus[node] = false;
            this->_nodeToNeighboursMap[node].clear();
        }
        return clusters;
    }
    
private:
    
    unordered_map<Node<dimensions>*, bool> _visitedStatus;
    
    void _depthFirstSearch(Node<dimensions> *node, Cluster<Node<dimensions> *> &cluster){
        _visitedStatus[node] = true;
        cluster.getList().push_back(node);
        for (auto &neighbour : this->_nodeToNeighboursMap[node]){
            if (!_visitedStatus[neighbour])
                _depthFirstSearch(neighbour, cluster);
        }
    }
    
    void _neighbourJob(Node<dimensions> *thisNode, Node<dimensions> *candidateNode) override {
        this->_nodeToNeighboursMap[thisNode].push_back(candidateNode);
    }
    

};

#endif //ALTAIRINTERVIEW_DEPTHFIRSTSEARCHCLUSTERING_H
