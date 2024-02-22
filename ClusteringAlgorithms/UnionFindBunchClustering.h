//
// Created by hal9000 on 2/21/24.
//

#ifndef ALTAIRINTERVIEW_UNIONFINDBUNCHCLUSTERING_H
#define ALTAIRINTERVIEW_UNIONFINDBUNCHCLUSTERING_H

#include "UnionFindClustering.h"

template<unsigned dimensions, unsigned numberOfNodes>
class UnionFindBunchClustering : public UnionFindClustering<dimensions, numberOfNodes> {
public:
    explicit UnionFindBunchClustering(const array<Node<dimensions>*, numberOfNodes> &nodes) :
             UnionFindClustering<dimensions, numberOfNodes>(nodes) {
        this->_nodeToNeighboursMap = unordered_map<Node<dimensions>*, list<Node<dimensions>*>>();
        for (unsigned i = 0; i < numberOfNodes; i++) {
            this->_nodeToNeighboursMap[this->_nodes[i]] = list<Node<dimensions>*>();
        }
    }
    
    ~UnionFindBunchClustering() = default;
    
    list<Cluster<Node<dimensions> *>> findClusters(double radius, unsigned availableThreads) override {
        
        auto threadJob = [&](unsigned start, unsigned end){
            for (unsigned i = start; i < end; i++)
                this->_findNeighboursWithMapBounds(this->_nodes[i], radius);
        };
        ThreadingOperations<void>::executeParallelJob(threadJob, numberOfNodes, availableThreads);

        for (auto &pair : this->_nodeToNeighboursMap) {
            auto thisNode = pair.first;
            for (auto &candidateNode : pair.second) {
                this->_unionFind.unionSets(this->_nodeToId[thisNode], this->_nodeToId[candidateNode]);
            }
        }

        for (auto &node : this->_nodes) {
            auto root = this->_nodes[this->_unionFind.find(this->_nodeToId[node])];
            // Check if the root belongs to a cluster. If not, create a new cluster
            if (this->_rootToClusterMap.find(root) == this->_rootToClusterMap.end()) {
                auto newCluster = Cluster<Node<dimensions>*>();
                this->_rootToClusterMap[root] = std::move(newCluster);
            }
            this->_rootToClusterMap[root].items.push_back(node);
        }
        auto clusters = list<Cluster<Node<dimensions>*>>();
        for (auto &pair : this->_rootToClusterMap) {
            clusters.push_back(std::move(pair.second));
        }
        this->_rootToClusterMap.clear();
        for (auto &node : this->_nodes){
            this->_nodeToNeighboursMap[node].clear();
        }
        this->_rootToClusterMap.clear();
        return clusters;
    }
    
protected:
    void _neighbourJob(Node<dimensions> *thisNode, Node<dimensions> *candidateNode) override {
        this->_nodeToNeighboursMap[thisNode].push_back(candidateNode);
    }
    
};
#endif //ALTAIRINTERVIEW_UNIONFINDBUNCHCLUSTERING_H
