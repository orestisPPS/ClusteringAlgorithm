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
                this->_findNeighboursWithMapBounds(this->_nodes[i], radius);
            }
        };
        ThreadingOperations<void>::executeParallelJob(threadJob, numberOfNodes, availableThreads);
        

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
        return clusters;

    }

protected:
    
    void _neighbourJob(Node<dimensions> *thisNode, Node<dimensions> *candidateNode) override {
        //this->_nodeToNeighboursMap[thisNode].push_back(candidateNode);
        this->_unionFind.unionSets(this->_nodeToId[thisNode], this->_nodeToId[candidateNode]);

    }
    
};
#endif //ALTAIRINTERVIEW_UNIONFINDPERNODECLUSTERING_H
