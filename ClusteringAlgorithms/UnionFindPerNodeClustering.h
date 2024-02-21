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
    }
    
    list<NodeCluster<dimensions>> findClusters(double radius, unsigned availableThreads) override {
        
        
        auto _neighbourJob = [&](Node<dimensions> *thisNode, Node<dimensions> *candidateNode){
            this->_unionFind.unionSets(this->_nodeToId[thisNode], this->_nodeToId[candidateNode]);
        };
        if (availableThreads != 1){
            auto threadJob = [&](unsigned start, unsigned end){
                for (unsigned i = start; i < end; i++){
                    //apply mutex to the union find
                    std::lock_guard<std::mutex> lock(this->_unionFindMutex);
                    this->_findNeighboursWithMapBounds(this->_nodes[i], radius, _neighbourJob);
                }
            };
            ThreadingOperations<void>::executeParallelJob(threadJob, numberOfNodes, availableThreads);
        }
        if (availableThreads == 1){
            for (unsigned i = 0; i < numberOfNodes; i++){
                this->_findNeighboursWithMapBounds(this->_nodes[i], radius, _neighbourJob);
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
#endif //ALTAIRINTERVIEW_UNIONFINDPERNODECLUSTERING_H
