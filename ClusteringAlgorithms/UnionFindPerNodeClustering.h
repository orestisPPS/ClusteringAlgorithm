//
// Created by hal9000 on 2/21/24.
//

#ifndef ALTAIRINTERVIEW_UNIONFINDPERNODECLUSTERING_H
#define ALTAIRINTERVIEW_UNIONFINDPERNODECLUSTERING_H

#include "UnionFindClustering.h"

template<unsigned dimensions, unsigned numberOfNodes>
class UnionFindPerNodeClustering : public UnionFindClustering<dimensions, numberOfNodes> {
public:
    explicit UnionFindPerNodeClustering(const array<Node<dimensions>*, numberOfNodes> &nodes) :
             UnionFindClustering<dimensions, numberOfNodes>(nodes) {
        
        this->_nodeToNeighboursMap = unordered_map<Node<dimensions>*, list<Node<dimensions>*>>();
        this->_nodeToId = unordered_map<Node<dimensions>*, unsigned>();
        for (unsigned dimension = 0; dimension < dimensions; dimension++)
            this->_coordinateComponentToNodeMaps[dimension] = map<double, list<Node<dimensions>*>>();
        
        for (unsigned i = 0; i < numberOfNodes; i++) {
            this->_nodeToNeighboursMap[this->_nodes[i]] = list<Node<dimensions>*>();
            this->_nodeToId[this->_nodes[i]] = i;
            auto coords = this->_nodes[i]->getCoordinatesVector();
            for (unsigned dimension = 0; dimension < dimensions; dimension++)
                this->_coordinateComponentToNodeMaps[dimension][coords[dimension]].push_back(this->_nodes[i]);
        }
    }
    
    ~UnionFindPerNodeClustering() = default;
    
    list<Cluster<Node<dimensions> *>> findClusters(double radius, unsigned availableThreads) override {

        auto threadJob = [&](unsigned start, unsigned end){
            for (unsigned i = start; i < end; i++){
                this->_findNeighboursWithMapBounds(this->_nodes[i], radius);
            }
        };
        ThreadingOperations<void>::executeParallelJob(threadJob, numberOfNodes, availableThreads);
        
        auto rootToClusterMap = unordered_map<Node<dimensions>*, Cluster<Node<dimensions>*>>(numberOfNodes);

        for (auto &node : this->_nodes) {
            auto root = this->_nodes[this->_unionFind.find(this->_nodeToId[node])];
            // Check if the root belongs to a cluster. If not, create a new cluster
            if (rootToClusterMap.find(root) == rootToClusterMap.end()) {
                auto newCluster = Cluster<Node<dimensions>*>();
                rootToClusterMap[root] = std::move(newCluster);
            }
            rootToClusterMap[root].items.push_back(node);
        }
        auto clusters = list<Cluster<Node<dimensions>*>>();
        for (auto &pair : rootToClusterMap) {
            clusters.push_back(std::move(pair.second));
        }
        return clusters;

    }

protected:
    
    void _neighbourJob(Node<dimensions> *thisNode, Node<dimensions> *candidateNode) override {
        //this->_nodeToNeighboursMap[thisNode].push_back(candidateNode);
        this->_unionFind.unionSets(this->_nodeToId[thisNode], this->_nodeToId[candidateNode]);

    }
    
};
#endif //ALTAIRINTERVIEW_UNIONFINDPERNODECLUSTERING_H
