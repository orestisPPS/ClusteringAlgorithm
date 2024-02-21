//
// Created by hal9000 on 2/20/24.
//

#ifndef ALTAIRINTERVIEW_CLUSTERINGALGORITHM_H
#define ALTAIRINTERVIEW_CLUSTERINGALGORITHM_H

#include "../NodeCluster.h"
#include "../ThreadingOperations.h"
#include "../UnionFind.h"
#include <map>

#include <unordered_map>

enum ClusteringAlgorithmType {
    UNION_FIND_PER_NODE,
    UNION_FIND_BUNCH,
    DEPTH_FIRST_SEARCH
};

template<unsigned dimensions, unsigned numberOfNodes>
class ClusteringAlgorithm {
public:
    
    virtual list<NodeCluster<dimensions>> findClusters(double radius, unsigned availableThreads) = 0;
    

protected:
    explicit ClusteringAlgorithm(const array<Node<dimensions>*, numberOfNodes> &nodes) : _nodes(nodes){}

    ~ClusteringAlgorithm() = default;
    
    const array<Node<dimensions>*, numberOfNodes>& _nodes;

    array<map<double, list<Node<dimensions>*>>, dimensions> _coordinateComponentToNodeMaps;

    template<typename NeighbourJob>
    void _findNeighboursWithMapBounds(Node<dimensions> *node, double radius, NeighbourJob neighbourJob) {

        auto candidateNodeCounter = unordered_map<Node<dimensions>*, unsigned>();
        auto thisCoordinates = node->getCoordinatesVector();
        for (unsigned i = 0; i < dimensions; i++) {
            //Find the range of nodal coordinate components i that could be within the radius of node at dimension i
            auto lowerBound = _coordinateComponentToNodeMaps[i].lower_bound(thisCoordinates[i] - radius);
            auto upperBound = _coordinateComponentToNodeMaps[i].upper_bound(thisCoordinates[i] + radius);
            bool first = true;
            for (auto it = lowerBound; it != upperBound; it++) {
                for (auto &candidateNode: it->second) {
                    if (candidateNodeCounter.find(candidateNode) != candidateNodeCounter.end()){
                        auto &counter = candidateNodeCounter[candidateNode];
                        if (counter == i)
                            counter++;
                        if (counter == dimensions && candidateNode != node &&
                                node->sumOfSquaresFrom(*candidateNode) <= radius * radius) {
                            auto candidatePtr = candidateNode;
                            neighbourJob(node, candidatePtr);
                        }
                    }
                    if (candidateNodeCounter.find(candidateNode) == candidateNodeCounter.end() && i == 0)
                        candidateNodeCounter.insert({candidateNode, 1});
                }
            }
        }
    }
    
};


#endif //ALTAIRINTERVIEW_CLUSTERINGALGORITHM_H
