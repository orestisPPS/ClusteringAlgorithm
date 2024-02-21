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
    
    ~UnionFindBunchClustering() = default;
    
    list<Cluster<Node<dimensions> *>> findClusters(double radius, unsigned availableThreads) override {


        auto nodeToClusterMap = unordered_map<Node<dimensions>*, list<Node<dimensions>*>>(numberOfNodes);
        for (auto &node: this->_nodes)
            nodeToClusterMap[node] = list<Node<dimensions>*>();

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
            if (nodeToClusterMap.find(root) == nodeToClusterMap.end()) {
                auto newCluster = list<Node<dimensions>*>();
                nodeToClusterMap.insert({root, newCluster});
            }
            nodeToClusterMap[root].push_back(node);
        }
        

        auto clusterId = 0;
        auto clusters = list<Cluster<Node<dimensions>*>>();
        for (auto &pair : nodeToClusterMap) {
            auto cluster = Cluster<Node<dimensions>*>(clusterId);
            cluster.getList() = std::move(pair.second);
            clusters.push_back(std::move(cluster));
            clusterId++;
        }
        for (auto &node : this->_nodes){
            this->_nodeToNeighboursMap[node].clear();
        }
        return clusters;
    }
    
protected:
    void _neighbourJob(Node<dimensions> *thisNode, Node<dimensions> *candidateNode) override {
        this->_nodeToNeighboursMap[thisNode].push_back(candidateNode);
    }
    
};
#endif //ALTAIRINTERVIEW_UNIONFINDBUNCHCLUSTERING_H
