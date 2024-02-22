//
// Created by hal9000 on 2/19/24.
//

#ifndef ALTAIRINTERVIEW_UNIONFIND_H
#define ALTAIRINTERVIEW_UNIONFIND_H

#include <unordered_map>
#include <stdexcept>
using namespace std;

template<unsigned numberOfNodes>
class UnionFind {
    public:

    explicit UnionFind(){
        for (unsigned i = 0; i < numberOfNodes; i++){
            _parents[i] = i;
            _ranks[i] = 0;
        }
    }
    
    unsigned find(const unsigned & setI){
        if (setI >= numberOfNodes)
            throw out_of_range("Index out of range");
        // Path compression : make each node in the path point to the root
        if (_parents[setI] != setI)
            _parents[setI] = find(_parents[setI]);
        return _parents[setI];
    }

    void unionSets(const unsigned &setI, const unsigned &setJ){
        //Find the roots
        unsigned rootI = find(setI);
        unsigned rootJ = find(setJ);
        //Union by rank : attach the smaller rank tree to the root of the larger rank tree
        if (rootI == rootJ)
            return;
        if (_ranks[rootI] < _ranks[rootJ])
            _parents[rootI] = rootJ;
        else if (_ranks[rootI] > _ranks[rootJ])
            _parents[rootJ] = rootI;
        // If the ranks are equal, attach the second tree to the first and increase the rank of the first
        else {
            _parents[rootJ] = rootI;
            _ranks[rootI]++;
        }
    }
    private:
    array<unsigned , numberOfNodes> _parents;
    array<unsigned, numberOfNodes> _ranks;
        
};


#endif //ALTAIRINTERVIEW_UNIONFIND_H
