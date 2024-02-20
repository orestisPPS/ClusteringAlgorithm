//
// Created by hal9000 on 2/20/24.
//

#ifndef ALTAIRINTERVIEW_CLUSTERINGALGORITHM_H
#define ALTAIRINTERVIEW_CLUSTERINGALGORITHM_H

#include "../NodeCloud.h"

enum ClusteringAlgorithmType {
    UnionFindClustering,
    DepthFirstSearchClustering
};

template<unsigned dimensions, unsigned numberOfNodes>
class ClusteringAlgorithm {
public:
    ~ClusteringAlgorithm() = default;

    ClusteringAlgorithmType type;



protected:
    explicit ClusteringAlgorithm(const array<Node<dimensions>*, numberOfNodes> &nodes) : _nodes(nodes) , type() {}
    
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
                            node->squaredSumFrom(*candidateNode) <= radius * radius) {
                            NeighbourJob();
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
