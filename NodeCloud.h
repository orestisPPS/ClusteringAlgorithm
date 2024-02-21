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
#include "NodeCluster.h"
#include "UnionFind.h"


template <unsigned dimensions>
struct NodeStatus {
    NodeStatus() : nodeId(-1), neighbours(list<Node<dimensions>*>()) {}
    
    unsigned nodeId;
    list<Node<dimensions>*> neighbours;
    
    void reset() {
        nodeId = -1;
        neighbours.clear();
    }
};


template<unsigned short dimensions, unsigned numberOfNodes>
class NodeCloud {
public:
    
    explicit NodeCloud(array<double, dimensions> dimensionsLengths, unsigned availableThreads, int sortByDimension = -1) {
        // Allocate contiguous memory for the nodes
        _blockOfNodes = new char[sizeof(Node<dimensions>) * numberOfNodes];
        if (sortByDimension == -1)
            _nodes = std::move(NodeFactory<dimensions>::createRandomVectors<numberOfNodes>(dimensionsLengths, _blockOfNodes, availableThreads));
        else
            _nodes = std::move(NodeFactory<dimensions>::createRandomVectors<numberOfNodes>(dimensionsLengths, _blockOfNodes, sortByDimension, availableThreads));
    }
    
    explicit NodeCloud(list<array<double, dimensions>> coordinates, unsigned availableThreads) {
        // Allocate contiguous memory for the nodes
        _blockOfNodes = new char[sizeof(Node<dimensions>) * coordinates.size()];
        _nodes = std::move(NodeFactory<dimensions>::createVectorsFromList(coordinates, _blockOfNodes, availableThreads));
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
    
    list<NodeCluster<dimensions>> calculateClustersUnionFind(double radius, bool bunchUnionFind, unsigned availableThreads = 1) {
        
        auto unionFind = UnionFind<numberOfNodes>();
        
        unordered_map<Node<dimensions>*, list<Node<dimensions>*>> neighbourMap;
        
        if (bunchUnionFind)
            neighbourMap = unordered_map<Node<dimensions>*, list<Node<dimensions>*>>(_nodes.size());
        
        unordered_map<Node<dimensions>*, unsigned> nodeToId = unordered_map<Node<dimensions>*, unsigned>();
        for (unsigned i = 0; i < _nodes.size(); i++){
            nodeToId.insert({_nodes[i], i});
            if (bunchUnionFind)
                neighbourMap.insert({_nodes[i], list<Node<dimensions>*>()});
        }
        
        if (bunchUnionFind){

            auto neighbourFindJob = [&](Node<dimensions>* thisNode, Node<dimensions>* candidateNode){
                neighbourMap[thisNode].push_back(candidateNode);
            };
            auto neighbourFindThreadJob = [&](unsigned start, unsigned end) {
                for (auto i = start; i < end; i++)
                    _findNeighbours(_nodes[i], radius, neighbourFindJob);
            };
            ThreadingOperations<Node<dimensions>*>::executeParallelJob(neighbourFindThreadJob, _nodes.size(), availableThreads);
            
            for (auto &pair : neighbourMap) {
                auto thisNode = pair.first;
                for (auto &candidateNode : pair.second) {
                    unionFind.unionSets(nodeToId[thisNode], nodeToId[candidateNode]);
                }
            }
        }
        else{
            auto neighbourFindJob = [&](Node<dimensions>* thisNode, Node<dimensions>* candidateNode){
                unionFind.unionSets(nodeToId[thisNode], nodeToId[candidateNode]);
            };

            auto neighbourFindThreadJob = [&](unsigned start, unsigned end) {
                auto neighbours = list<Node<dimensions>*>();
                for (auto i = start; i < end; i++)
                    _findNeighboursAndUnionSets(_nodes[i], radius, unionFind);
            };
            ThreadingOperations<Node<dimensions>*>::executeParallelJob(neighbourFindThreadJob, _nodes.size(), availableThreads);
        }
        
        auto clusterMap = unordered_map<Node<dimensions>*, list<Node<dimensions>*>>();
        for (auto &node : _nodes) {
            auto root = _nodes[unionFind.find(_nodeToId[node])];
            if (clusterMap.find(root) == clusterMap.end()) {
                auto newCluster = list<Node<dimensions>*>();
                clusterMap.insert({root, newCluster});
            }
            // Add the node to the cluster associated with its representative
            clusterMap[root].push_back(node);
        }
        
        auto clusters = list<NodeCluster<dimensions>>();
        auto clusterId = 0;
        for (auto &pair : clusterMap) {
            clusters.push_back(NodeCluster<dimensions>(clusterId));
            clusters.front().getNodes() = std::move(pair.second);
            clusterId++;
        }
        return clusters;
    }

    list<NodeCluster<dimensions>> calculateClusters(double radius, unsigned availableThreads) {

        auto clusters = list<NodeCluster<dimensions>>();

        auto unionFind = UnionFind<numberOfNodes>();

        auto neighbourFindThreadJob = [&](unsigned start, unsigned end) {
            auto neighbours = list<Node<dimensions>*>();
            for (auto i = start; i < end; i++)
                _findNeighbours(_nodes[i], radius, unionFind);
        };
        ThreadingOperations<Node<dimensions>*>::executeParallelJob(neighbourFindThreadJob, _nodes.size(), availableThreads);
        
        auto clusterMap = unordered_map<Node<dimensions>*, list<Node<dimensions>*>>();
        for (auto &node : _nodes) {
            auto root = _nodes[unionFind.find(_nodeToId[node])];
            if (clusterMap.find(root) == clusterMap.end()) {
                auto newCluster = list<Node<dimensions>*>();
                clusterMap.insert({root, newCluster});
            }
            // Add the node to the cluster associated with its representative
            clusterMap[root].push_back(node);
        }

        auto clusterId = 0;
        for (auto &pair : clusterMap) {
            clusters.push_back(NodeCluster<dimensions>(clusterId));
            clusters.front().getNodes() = std::move(pair.second);
            clusterId++;
        }
        return clusters;
    }

    
private:
    char* _blockOfNodes; // Contiguous heap memory for Node objects

    array<Node<dimensions>*, numberOfNodes> _nodes; // Array of pointers to Node objects
    
};




#endif //ALTAIRINTERVIEW_NODECLOUD_H
