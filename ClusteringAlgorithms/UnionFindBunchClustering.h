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
    }
    
    list<NodeCluster<dimensions>> findClusters(double radius, unsigned availableThreads) override {
        auto nodeToNeighboursMap = unordered_map<Node<dimensions>*, list<Node<dimensions>*>>(numberOfNodes);
        for (auto &node: this->_nodes)
            nodeToNeighboursMap[node] = list<Node<dimensions>*>();
        
        auto _neighbourJob = [&](Node<dimensions> *thisNode, Node<dimensions> *candidateNode){
            nodeToNeighboursMap[thisNode].push_back(candidateNode);
        };
        auto threadJob = [&](unsigned start, unsigned end){
            for (unsigned i = start; i < end; i++)
                this->_findNeighboursWithMapBounds(this->_nodes[i], radius, _neighbourJob);
        };
        ThreadingOperations<void>::executeParallelJob(threadJob, numberOfNodes, availableThreads);
        
        for (auto &pair : nodeToNeighboursMap) {
            auto thisNode = pair.first;
            for (auto &candidateNode : pair.second) {
                this->_unionFind.unionSets(this->_nodeToId[thisNode], this->_nodeToId[candidateNode]);
            }
        }
        for (auto &node : this->_nodes) {
            auto root = this->_nodes[this->_unionFind.find(this->_nodeToId[node])];
            if (this->_nodeToClusterMap.find(root) == this->_nodeToClusterMap.end()) {
                auto newCluster = list<Node<dimensions>*>();
                this->_nodeToClusterMap.insert({root, newCluster});
            }
            this->_nodeToClusterMap[root].push_back(node);      
        }
        nodeToNeighboursMap.clear();

        auto clusterId = 0;
        auto clusters = list<NodeCluster<dimensions>>();
        for (auto &pair : this->_nodeToClusterMap) {
            auto cluster = NodeCluster<dimensions>(clusterId);
            cluster.getNodes() = std::move(pair.second);
            clusters.push_back(std::move(cluster));
            clusterId++;
        }
        return clusters;
    }
    
};
#endif //ALTAIRINTERVIEW_UNIONFINDBUNCHCLUSTERING_H
