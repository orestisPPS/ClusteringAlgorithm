//
// Created by hal9000 on 2/21/24.
//

#ifndef ALTAIRINTERVIEW_NODEFACTORY_H
#define ALTAIRINTERVIEW_NODEFACTORY_H

#include <array>
#include <random>
#include <algorithm>
#include <list>
#include "ThreadingOperations.h"
#include "Node.h"

using namespace std;

template<unsigned dimensions>
class NodeFactory {
    
    public:

    template<unsigned numberOfNodes> 
    static array<Node<dimensions>*, dimensions> 
    createRandomVectors(array<double, dimensions> dimensionsLengths, char* heapPosition, unsigned numberOfThreads = 1) {
        
        array<uniform_real_distribution<double>, dimensions> distributionsVector;
        array<Node<dimensions>*, numberOfNodes> nodes;
        for (unsigned i = 0; i < dimensions; i++)
            distributionsVector[i] = uniform_real_distribution<double>(0, dimensionsLengths[i]);

        auto coordinateInitializationJob = [&](unsigned start, unsigned end) {
            random_device randomDevice;
            mt19937 generator(randomDevice());
            auto coords = array<double, dimensions>();
            for (unsigned i = start; i < end; i++){
                for (unsigned direction = 0; direction < dimensions; direction++){
                    coords[direction] = distributionsVector[direction](generator);
                }
                nodes[i] = new(heapPosition + i * sizeof(Node<dimensions>)) Node<dimensions>(coords);
            }
        };
        ThreadingOperations<double>::executeParallelJob(coordinateInitializationJob, numberOfNodes, numberOfThreads);
        return nodes;
    }

    template<unsigned numberOfNodes>
    static array<Node<dimensions>*, dimensions>
    createRandomVectors(array<double, dimensions> dimensionsLengths, char* heapPosition, int sortByDimension, unsigned numberOfThreads = 1) {

        static_assert(sortByDimension < dimensions , "sortByDimension must be less than the number of dimensions");

        array<uniform_real_distribution<double>, dimensions> distributionsVector;
        array<Node<dimensions>*, numberOfNodes> nodes;
        array<array<double, dimensions>, numberOfNodes> nodalCoordinatesArray;
        for (unsigned i = 0; i < dimensions; i++)
            distributionsVector[i] = uniform_real_distribution<double>(0, dimensionsLengths[i]);

        auto coordinateInitializationJob = [&](unsigned start, unsigned end) {
            random_device randomDevice;
            mt19937 generator(randomDevice());
            auto coords = array<double, dimensions>();
            for (unsigned i = start; i < end; i++){
                for (unsigned direction = 0; direction < dimensions; direction++)
                    nodalCoordinatesArray[i][direction] = distributionsVector[direction](generator);
            }
        };
        ThreadingOperations<double>::executeParallelJob(coordinateInitializationJob, numberOfNodes, numberOfThreads);

        auto compare = [sortByDimension](const array<double, dimensions> &a, const array<double, dimensions> &b) {
            return a[sortByDimension] < b[sortByDimension];
        };
        sort(nodes.begin(), nodes.end(), compare);
        
        auto coordinateInitializationJob2 = [&](unsigned start, unsigned end) {
            for (unsigned i = start; i < end; i++)
                nodes[i] = new(heapPosition + i * sizeof(Node<dimensions>)) Node<dimensions>(nodalCoordinatesArray[i]);
        };
        ThreadingOperations<double>::executeParallelJob(coordinateInitializationJob2, numberOfNodes, numberOfThreads);
        return nodes;
    }

    static array<double, dimensions> createVectorFromInput(const list<array<double, dimensions>> &inputList,
                                                           int sortByDimension = -1, unsigned numberOfThreads = 1) {
        
        array<array<double, dimensions>, inputList.size()> nodalCoordinatesArray;
        copy(inputList.begin(), inputList.end(), nodalCoordinatesArray.begin());
        
        if (sortByDimension >= 0) {
            auto compare = [sortByDimension](const array<double, dimensions> &a, const array<double, dimensions> &b) {
                return a[sortByDimension] < b[sortByDimension];
            };
            sort(nodalCoordinatesArray.begin(), nodalCoordinatesArray.end(), compare);
        }
        return nodalCoordinatesArray;
    }
    
private:
    NodeFactory() = default;
    ~NodeFactory() = default;
};

#endif //ALTAIRINTERVIEW_NODEFACTORY_H
