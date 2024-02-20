//
// Created by hal9000 on 2/19/24.
//

#ifndef ALTAIRINTERVIEW_UNIONFIND_H
#define ALTAIRINTERVIEW_UNIONFIND_H

#include <unordered_map>
#include <stdexcept>
using namespace std;


template<unsigned numberOfNodes, typename T>
class UnionFind {
    public:

    explicit UnionFind(const array<T, numberOfNodes> & nodes){
        static_assert(numberOfNodes > 0, "Invalid number of nodes");
        // Initialize each element as its own parent and with rank 0
        _parentsAndRanks= unordered_map<T, pair<T, unsigned>>(numberOfNodes);
        for (unsigned i = 0; i < numberOfNodes; i++){
            _parentsAndRanks[nodes[i]] = make_pair(nodes[i], 0);
        }
    }
    
    T find(const T & node){
        if (_parentsAndRanks.find(node) == _parentsAndRanks.end())
            throw out_of_range("Node not found");
        if (_parents(node) != node)
            _parents(node) = find(_parents(node));
        return _parents(node);
    }

    void unionSets(const T &setI, const T &setJ){
        if (_parentsAndRanks.find(setI) == _parentsAndRanks.end() || _parentsAndRanks.find(setJ) == _parentsAndRanks.end())
            throw out_of_range("Node not found");
        T rootI = find(setI);
        T rootJ = find(setJ);
        if (rootI == rootJ)
            return;
        if (_ranks(rootI) < _ranks(rootJ))
            _parents(rootI) = rootJ;
        else if (_ranks(rootI) > _ranks(rootJ))
            _parents(rootJ) = rootI;
        else {
            _parents(rootJ) = rootI;
            _ranks(rootI)++;
        }
    }
    
    private:
    
    T & _parents(const T & node){
        return get<0>(_parentsAndRanks[node]);
    }
    
    unsigned & _ranks(const T & node){
        return get<1>(_parentsAndRanks[node]);
    }
    
    unordered_map<T, pair<T, unsigned>> _parentsAndRanks;
        
};


#endif //ALTAIRINTERVIEW_UNIONFIND_H
