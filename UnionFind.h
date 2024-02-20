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
        if (_parents[setI] != setI)
            _parents[setI] = find(_parents[setI]);
        return _parents[setI];
    }

    void unionSets(const unsigned &setI, const unsigned &setJ){
        
        unsigned rootI = find(setI);
        unsigned rootJ = find(setJ);
        if (rootI == rootJ)
            return;
        if (_ranks[rootI] < _ranks[rootJ])
            _parents[rootI] = rootJ;
        else if (_ranks[rootI] > _ranks[rootJ])
            _parents[rootJ] = rootI;
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
