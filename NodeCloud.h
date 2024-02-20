//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUD_H
#define ALTAIRINTERVIEW_NODECLOUD_H

#include <random>
#include <algorithm>
#include <list>
#include <unordered_map>
#include <map>
#include <cmath>
#include <mutex>
#include "ThreadingOperations.h"
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
    
    explicit NodeCloud(array<double, dimensions> dimensionsLengths, unsigned availableThreads = 1) {
        static_assert(dimensionsLengths.size() == dimensions, "Invalid number of dimension lengths");

        // Allocate contiguous memory for the nodes
        _blockOfNodes = new char[sizeof(Node<dimensions>) * numberOfNodes];
        

        // Create distributions for each dimension
        array<uniform_real_distribution<double>, dimensions> distributionsVector;
        for (unsigned i = 0; i < dimensions; i++) {
            distributionsVector[i] = uniform_real_distribution<double>(0, dimensionsLengths[i]);
            _coordinateComponentToNodeMaps[i] = map<double, list<Node<dimensions>*>>();
        }
        array<array<double, dimensions>, numberOfNodes> nodalCoordinatesArray;
        
        // Node initialization job for parallel execution
        auto coordinateInitializationJob = [&](unsigned start, unsigned end) {
            random_device randomDevice;
            mt19937 generator(randomDevice());
            auto coords = array<double, dimensions>();
            for (unsigned i = start; i < end; i++){
                for (unsigned direction = 0; direction < dimensions; direction++){
                    nodalCoordinatesArray[i][direction] = distributionsVector[direction](generator);
                    coords[direction] = distributionsVector[direction](generator);
                }
                _nodes[i] = new(_blockOfNodes + i * sizeof(Node<dimensions>)) Node<dimensions>(std::move(coords));
            }
        };
        ThreadingOperations<Node<dimensions>*>::executeParallelJob(coordinateInitializationJob, numberOfNodes, availableThreads);
        
        //Sort the coordinates based on the sum of the squares of their components
//        sort(nodalCoordinatesArray.begin(), nodalCoordinatesArray.end(),
//             [](const array<double, dimensions>& a, const array<double, dimensions>& b) {
//                 return a[0] < b[0];
//            });
//
//        //Create the nodes in the heap based on their proximity to the origin
//        auto nodeCreationJob = [&](unsigned start, unsigned end) {
//            for (unsigned i = start; i < end; i++)
//                _nodes[i] = new(_blockOfNodes + i * sizeof(Node<dimensions>)) Node<dimensions>(std::move(nodalCoordinatesArray[i]));
//        };
//        ThreadingOperations<Node<dimensions>*>::executeParallelJob(nodeCreationJob, numberOfNodes, availableThreads);
        
        auto coordinateToNodeMapJob = [&](unsigned direction, unsigned direction2) {
            for (unsigned i = 0; i < numberOfNodes; i++){
                //_coordinateComponentToNodeMaps[direction][_nodes[i]->getCoordinatesVector()[direction]].push_back(_nodes[i]);
                _coordinateComponentToNodeMaps[direction][nodalCoordinatesArray[i][direction]].push_back(_nodes[i]);
                _nodeToId.insert({_nodes[i], i});
            }
        };
        ThreadingOperations<Node<dimensions>*>::executeParallelJob(coordinateToNodeMapJob, dimensions, availableThreads == 1 ? 1 : dimensions);
        
        //cout << "NodeCloud created" << endl;
    }
    
    explicit NodeCloud(array<array<double,dimensions> , numberOfNodes> nodalCoordinatesArray) {
        // Allocate contiguous memory for the nodes
        _blockOfNodes = new char[sizeof(Node<dimensions>) * numberOfNodes];
        //Sort the coordinates based on the sum of the squares of their components
        sort(nodalCoordinatesArray.begin(), nodalCoordinatesArray.end(),
             [](const array<double, dimensions>& a, const array<double, dimensions>& b) {
                 double sumA = 0;
                 double sumB = 0;
                 for (unsigned i = 0; i < dimensions; i++) {
                     sumA += a[i] * a[i];
                     sumB += b[i] * b[i];
                 }
                 return sumA < sumB;
             });
        for (unsigned i = 0; i < nodalCoordinatesArray.size(); i++){
            _nodes[i] = new(_blockOfNodes + i * sizeof(Node<dimensions>)) Node<dimensions>(nodalCoordinatesArray[i]);
            for (unsigned j = 0; j < dimensions; j++)
                _coordinateComponentToNodeMaps[j][nodalCoordinatesArray[i][j]].push_back(_nodes[i]);
        }
    }
    
    ~NodeCloud() {
        // Deallocate the node objects
        for (unsigned i = 0; i < numberOfNodes; i++) {
            _nodes[i]->~Node();
        }
        // Deallocate the block of nodes
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

    
    unordered_map<Node<dimensions>*, unsigned> _nodeToId;
    
    mutex _mutex;
    


};




#endif //ALTAIRINTERVIEW_NODECLOUD_H
