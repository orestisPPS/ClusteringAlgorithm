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
            _nodeToNeighboursMap[this->_nodes[i]] = list<Node<dimensions>*>();
        }
    }
    
    list<NodeCluster<dimensions>> findClusters(double radius, unsigned availableThreads) override {

        auto _neighbourJob = [&](Node<dimensions> *thisNode, Node<dimensions> *candidateNode){
            _nodeToNeighboursMap[thisNode].push_back(candidateNode);
        };
        auto threadJob = [&](unsigned start, unsigned end){
            for (unsigned i = start; i < end; i++)
                this->_findNeighboursWithMapBounds(this->_nodes[i], radius, _neighbourJob);
        };
        ThreadingOperations<void>::executeParallelJob(threadJob, numberOfNodes, availableThreads);
        
        auto clusterId = 0;
        auto clusters = list<NodeCluster<dimensions>>();
        for (auto &node : this->_nodes){
            if (!_visitedStatus[node]){
                auto cluster = NodeCluster<dimensions>(clusterId);
                _depthFirstSearch(node, radius, cluster, _nodeToNeighboursMap, _visitedStatus);
                clusters.push_back(std::move(cluster));
                clusterId++;
            }
        }
        for (auto &node : this->_nodes){
            _visitedStatus[node] = false;
            _nodeToNeighboursMap[node].clear();
        }
        return clusters;
    }
    
private:
    
    unordered_map<Node<dimensions>*, list<Node<dimensions>*>> _nodeToNeighboursMap;
    unordered_map<Node<dimensions>*, bool> _visitedStatus;
    
    void _depthFirstSearch(Node<dimensions> *node, double radius, NodeCluster<dimensions> &cluster,
                          unordered_map<Node<dimensions>*, list<Node<dimensions>*>> &nodeToNeighboursMap, 
                          unordered_map<Node<dimensions>*, bool> &visitedStatus){
        visitedStatus[node] = true;
        cluster.getNodes().push_back(node);
        for (auto &neighbour : nodeToNeighboursMap[node]){
            if (!visitedStatus[neighbour])
                _depthFirstSearch(neighbour, radius, cluster, nodeToNeighboursMap, visitedStatus);
        }
    }

};

#endif //ALTAIRINTERVIEW_DEPTHFIRSTSEARCHCLUSTERING_H
