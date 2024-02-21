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
    explicit UnionFindClustering(const array<Node<dimensions>*, numberOfNodes> &nodes) : ClusteringAlgorithm<dimensions, numberOfNodes>(nodes){
    _unionFind = UnionFind<numberOfNodes>();
        _nodeToId = unordered_map<Node<dimensions>*, unsigned >(numberOfNodes);
        for (unsigned i = 0; i < numberOfNodes; i++) {
            _nodeToId[this->_nodes[i]] = i;
            for (unsigned dimension = 0; dimension < dimensions; dimension++)
                this->_coordinateComponentToNodeMaps[dimension][this->_nodes[i]->getCoordinatesVector()[dimension]].push_back(this->_nodes[i]);
        }
    }
    UnionFind<numberOfNodes> _unionFind;
    unordered_map<Node<dimensions>*, unsigned > _nodeToId;
    unordered_map<Node<dimensions>*, list<Node<dimensions>*>> _nodeToClusterMap;
    mutex _unionFindMutex;
};
#endif //ALTAIRINTERVIEW_UNIONFINDCLUSTERING_H
