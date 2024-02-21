//
// Created by hal9000 on 2/21/24.
//

#ifndef ALTAIRINTERVIEW_IMMEDIATECLUSTERING_H
#define ALTAIRINTERVIEW_IMMEDIATECLUSTERING_H

#include "ClusteringAlgorithm.h"

template<unsigned dimensions, unsigned numberOfNodes>
class ImmediateClustering : public ClusteringAlgorithm<dimensions, numberOfNodes> {
public:
    explicit ImmediateClustering(const array<Node<dimensions> *, numberOfNodes> &nodes)
            : ClusteringAlgorithm<dimensions, numberOfNodes>(nodes) {
        for (unsigned i = 0; i < numberOfNodes; i++) {
            for (unsigned dimension = 0; dimension < dimensions; dimension++)
                this->_coordinateComponentToNodeMaps[dimension][this->_nodes[i]->getCoordinatesVector()[dimension]].push_back(
                        this->_nodes[i]);
            _nodeToClusterNodesMap[this->_nodes[i]] = list<Node<dimensions> *>();
            _nodeToClusterIdMap[this->_nodes[i]] = -1;
        }
    }

    list<NodeCluster<dimensions>> findClusters(double radius, unsigned availableThreads) override {
        auto clusterId = 0;
        auto clusters = list<Node<dimensions> *>();
        
        for (auto &node : this->_nodes) {
            if (_nodeToClusterIdMap[node] == -1) {
                auto cluster = NodeCluster<dimensions>(clusterId);
                _nodeToClusterIdMap[node] = clusterId;
                _nodeToClusterNodesMap[node].push_back(node);
            }
            auto candidateNodeCounter = unordered_map<Node<dimensions> *, unsigned>();
            auto thisCoordinates = node->getCoordinatesVector();
            auto radiusSquared = radius * radius;
            for (unsigned i = 0; i < dimensions; i++) {
                //Find the range of nodal coordinate components i that could be within the radius of node at dimension i
                auto lowerBound = this->_coordinateComponentToNodeMaps[i].lower_bound(thisCoordinates[i] - radius);
                auto upperBound = this->_coordinateComponentToNodeMaps[i].upper_bound(thisCoordinates[i] + radius);
                bool first = true;
                for (auto it = lowerBound; it != upperBound; it++) {
                    for (auto &candidateNode: it->second) {

                        if (candidateNodeCounter.find(candidateNode) == candidateNodeCounter.end())
                            candidateNodeCounter[candidateNode] = 1;
                        else {
                            auto &timesAppeared = candidateNodeCounter[candidateNode];
                            timesAppeared++;
                            if (timesAppeared == dimensions && candidateNode != node && node->sumOfSquaresFrom(*candidateNode) <= radiusSquared) {
                                neighbourJob(node, candidateNode);
                            }
                        }
                    }
                }
            }
        }
        return clusters;
    }

private:

    unordered_map<Node<dimensions> *, list<Node<dimensions> *>> _nodeToClusterNodesMap;
    unordered_map<Node<dimensions> *, int> _nodeToClusterIdMap;


};

#endif //ALTAIRINTERVIEW_IMMEDIATECLUSTERING_H
