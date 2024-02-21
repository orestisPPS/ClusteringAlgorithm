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
    
    ~UnionFindPerNodeClustering() = default;
    
    list<Cluster<Node<dimensions> *>> findClusters(double radius, unsigned availableThreads) override {

        auto threadJob = [&](unsigned start, unsigned end){
            for (unsigned i = start; i < end; i++){
                //apply mutex to the union find
                this->_findNeighboursWithMapBounds(this->_nodes[i], radius);
                if (availableThreads > 1){
                    std::lock_guard<std::mutex> lock(this->_unionFindMutex);
                    for (auto &candidateNode : this->_nodeToNeighboursMap[this->_nodes[i]]) {
                        this->_unionFind.unionSets(this->_nodeToId[this->_nodes[i]], this->_nodeToId[candidateNode]);
                    }
                }
                else{
                    for (auto &candidateNode : this->_nodeToNeighboursMap[this->_nodes[i]]) {
                        this->_unionFind.unionSets(this->_nodeToId[this->_nodes[i]], this->_nodeToId[candidateNode]);
                    }
                }
            }
        };
        ThreadingOperations<void>::executeParallelJob(threadJob, numberOfNodes, availableThreads);
        
        for (auto &node : this->_nodes) {
            auto root = this->_nodes[this->_unionFind.find(this->_nodeToId[node])];
            if (this->_nodeToNeighboursMap.find(root) == this->_nodeToNeighboursMap.end()) {
                auto newCluster = list<Node<dimensions>*>();
                this->_nodeToNeighboursMap.insert({root, newCluster});
            }
            this->_nodeToNeighboursMap[root].push_back(node);      
        }

        auto clusterId = 0;
        auto clusters = list<Cluster<Node<dimensions> *>>();
        for (auto &pair : this->_nodeToNeighboursMap) {
            auto cluster = Cluster<Node<dimensions> *>(clusterId);
            cluster.getList() = std::move(pair.second);
            clusters.push_back(std::move(cluster));
            clusterId++;
        }
        return clusters;
    }

protected:
    void _neighbourJob(Node<dimensions> *thisNode, Node<dimensions> *candidateNode) override {
        this->_nodeToNeighboursMap[thisNode].push_back(candidateNode);
        //            this->_unionFind.unionSets(this->_nodeToId[thisNode], this->_nodeToId[candidateNode]);

    }
    
};
#endif //ALTAIRINTERVIEW_UNIONFINDPERNODECLUSTERING_H
