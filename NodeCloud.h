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
#include "Node.h"
#include "ThreadingOperations.h"
#include "NodeCluster.h"

template <unsigned dimensions>
struct NodeStatus {
    NodeStatus() : visited(false), neighbours(list<Node<dimensions>*>()) , clusterId(-1) {}

    bool visited = false;

    list<Node<dimensions>*> neighbours;

    unsigned clusterId = -1;

    void reset() {
        visited = false;
        neighbours.clear();
        clusterId = -1;
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

                if (direction == 0)
                    _nodeToClusteringStatus[_nodes[i]] = NodeStatus<dimensions>();
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
            _nodeToClusteringStatus[_nodes[i]] = NodeStatus<dimensions>();
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
    
    void findNeighboursOfNode(Node<dimensions>* node, double radius) {
        
        auto candidateNodeCounter = unordered_map<Node<dimensions>*, unsigned>();
        auto thisCoordinates = node->getCoordinatesVector();
        auto& filteredNodes = _nodeToClusteringStatus[node].neighbours;
        for (unsigned i = 0; i < dimensions; i++) {
            //Find the range of nodal coordinate components i that could be within the radius of node at dimension i
            auto lowerBound = _coordinateComponentToNodeMaps[i].lower_bound(thisCoordinates[i] - radius);
            auto upperBound = _coordinateComponentToNodeMaps[i].upper_bound(thisCoordinates[i] + radius);
            bool first = true;
            //Iterate through the range of coordinate component keys and count the number of times each candidate nodes appear
            for (auto it = lowerBound; it != upperBound; it++) {
                for (auto &candidateNode: it->second) {
                    //Appears for the first time
                    if (candidateNodeCounter.find(candidateNode) == candidateNodeCounter.end())
                        candidateNodeCounter[candidateNode] = 1;
                        //Appears for > 1 times
                    else {
                        auto &timesAppeared = candidateNodeCounter[candidateNode];
                        timesAppeared++;
                        if (timesAppeared == dimensions && candidateNode != node && _assessNeighbour(node, candidateNode, radius, filteredNodes)) {
                            filteredNodes.push_back(candidateNode);
                        }
                    }
                }
            }
        }
    }

    list<NodeCluster<dimensions>> calculateClusters(double radius, unsigned availableThreads) {
    
        auto clusters = list<NodeCluster<dimensions>>();
    
        auto neighbourFindThreadJob = [&](unsigned start, unsigned end) {
            for (auto i = start; i < end; i++)
                findNeighboursOfNode(_nodes[i], radius);
        };
        ThreadingOperations<Node<dimensions>*>::executeParallelJob(neighbourFindThreadJob, _nodes.size(), availableThreads);
        cout << "Neighbours found" << endl;
        unsigned clusterId = 0;
        for (auto &thisNode : _nodes) {
            if (!(_nodeToClusteringStatus)[thisNode].visited) {
                auto cluster = NodeCluster<dimensions>(clusterId);
                clusterId++;
                _searchNeighboursRecursively(thisNode, cluster);
                clusters.push_back(std::move(cluster));
            }
        }
        cout << "Clusters found" << endl;
    
        auto clearUnorderedMap = [&](unsigned start, unsigned end){
            for (auto i = start; i < end; i++) {
                _nodeToClusteringStatus[_nodes[i]].reset();
            }
        };
        ThreadingOperations<Node<dimensions>*>::executeParallelJob(clearUnorderedMap, _nodes.size(), availableThreads);
        return clusters;
    }

    
private:
    char* _blockOfNodes; // Contiguous memory for Node objects

    array<Node<dimensions>*, numberOfNodes> _nodes; // Array of pointers to Node objects

    array<map<double, list<Node<dimensions>*>>, dimensions> _coordinateComponentToNodeMaps;

    unordered_map<Node<dimensions>*, NodeStatus<dimensions>> _nodeToClusteringStatus;
    

    bool _assessNeighbour(Node<dimensions> *thisNode, Node<dimensions> *candidateNode, double radius,
                          list<Node<dimensions>*> &filteredNodes) const {
        auto &thisCoordinates = thisNode->getCoordinatesVector();
        auto &candidateCoordinates = candidateNode->getCoordinatesVector();
        double distance = 0;

        for (unsigned j = 0; j < dimensions; j++)
            distance += (thisCoordinates[j] - candidateCoordinates[j]) * (thisCoordinates[j] - candidateCoordinates[j]);
        if(sqrt(distance) <= radius)
            //if( distance <= radius * radius)
            return true;
        return false;
    }

    void _searchNeighboursRecursively(Node<dimensions> *node, NodeCluster<dimensions> &cluster) {
        auto& status = _nodeToClusteringStatus[node];
        if (!status.visited) {
            status.visited = true;
            cluster.getNodes().push_back(node);
            for (auto& neighbour : status.neighbours) {
                _searchNeighboursRecursively(neighbour, cluster);
            }
        }
    }
};




#endif //ALTAIRINTERVIEW_NODECLOUD_H
