//
// Created by hal9000 on 2/21/24.
//

#ifndef ALTAIRINTERVIEW_UNIONFINDCLUSTERING_H
#define ALTAIRINTERVIEW_UNIONFINDCLUSTERING_H
#include "ClusteringAlgorithm.h"

template<unsigned dimensions, unsigned numberOfNodes>
class UnionFindClustering : public ClusteringAlgorithm<dimensions, numberOfNodes> {
public:
    
protected:
    explicit UnionFindClustering(const array<Node<dimensions>*, numberOfNodes> &nodes) :
             ClusteringAlgorithm<dimensions, numberOfNodes>(nodes) {
        _nodeToId = unordered_map<Node<dimensions>*, unsigned>();
        _rootToClusterMap = unordered_map<Node<dimensions>*, Cluster<Node<dimensions>*> >();
        for (unsigned dimension = 0; dimension < dimensions; dimension++)
            this->_coordinateComponentToNodeMaps[dimension] = map<double, list<Node<dimensions>*>>();
        for (unsigned i = 0; i < numberOfNodes; i++) {
            _nodeToId[this->_nodes[i]] = i;
            auto coords = this->_nodes[i]->getCoordinatesVector();
            for (unsigned dimension = 0; dimension < dimensions; dimension++)
                this->_coordinateComponentToNodeMaps[dimension][coords[dimension]].push_back(this->_nodes[i]);
        }
    }


    
    ~UnionFindClustering() = default;

    UnionFind<numberOfNodes> _unionFind;
    
    unordered_map<Node<dimensions>*, unsigned > _nodeToId;
    
    unordered_map<Node<dimensions>*, Cluster<Node<dimensions>*> > _rootToClusterMap;
    
    mutex _unionFindMutex;
};
#endif //ALTAIRINTERVIEW_UNIONFINDCLUSTERING_H
