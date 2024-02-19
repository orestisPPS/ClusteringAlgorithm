//
// Created by hal9000 on 2/19/24.
//

#ifndef ALTAIRINTERVIEW_UNIONFIND_H
#define ALTAIRINTERVIEW_UNIONFIND_H

#include <array>
#include <stdexcept>
using namespace std;


template<unsigned numberOfNodes>
class UnionFind {
    public:

    explicit UnionFind(){
        static_assert(numberOfNodes > 0, "Invalid number of nodes");
        // Initialize each element as its own parent and with rank 0
        for (unsigned i = 0; i < numberOfNodes; i++) {
            parents[i] = i;
            ranks[i] = 0;
        }
    }
    
    unsigned find(unsigned i){
        if (i >= numberOfNodes)
            throw out_of_range("Index out of bounds");
        if (i != parents[i])
            parents[i] = find(parents[i]);
        return parents[i];
    }

    void unionSets(unsigned setI, unsigned setJ){
        if (setI >= numberOfNodes || setJ >= numberOfNodes)
            throw out_of_range("Index out of bounds");
        unsigned rootI = find(setI);
        unsigned rootJ = find(setJ);
        if (rootI == rootJ)
            return;
        if (ranks[rootI] < ranks[rootJ])
            parents[rootI] = rootJ;
        else if (ranks[rootI] > ranks[rootJ])
            parents[rootJ] = rootI;
        else {
            parents[rootJ] = rootI;
            ranks[rootI]++;
        }
    }
    
    private:
        array<unsigned, numberOfNodes> parents;
        array<unsigned, numberOfNodes> ranks;
};


#endif //ALTAIRINTERVIEW_UNIONFIND_H
