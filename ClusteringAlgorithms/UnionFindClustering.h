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
             ClusteringAlgorithm<dimensions, numberOfNodes>(nodes){}

    
    ~UnionFindClustering() = default;

    UnionFind<numberOfNodes> _unionFind;
    
    unordered_map<Node<dimensions>*, unsigned > _nodeToId;
    
    mutex _unionFindMutex;
};
#endif //ALTAIRINTERVIEW_UNIONFINDCLUSTERING_H
