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

template<unsigned dimensions, unsigned numberOfNodes>
class NodeFactory {
    
    public:
    
    static void
    createRandomNodes(array<double, dimensions> dimensionsLengths, array<Node<dimensions>*, numberOfNodes>& nodes,
                        char* heapPosition, int sortByDimension, unsigned numberOfThreads) {

        static_assert(sortByDimension < dimensions , "sortByDimension must be less than the number of dimensions");

        array<uniform_real_distribution<double>, dimensions> distributionsVector;
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

        if (sortByDimension >= 0) {
            auto compare = [sortByDimension](const array<double, dimensions> &a, const array<double, dimensions> &b) {
                return a[sortByDimension] < b[sortByDimension];
            };
            sort(nodalCoordinatesArray.begin(), nodalCoordinatesArray.end(), compare);
        }
        
        auto nodeInitializationJob = [&](unsigned start, unsigned end) {
            for (unsigned i = start; i < end; i++)
                nodes[i] = new(heapPosition + i * sizeof(Node<dimensions>)) Node<dimensions>(std::move(nodalCoordinatesArray[i]));
        };
        ThreadingOperations<double>::executeParallelJob(nodeInitializationJob, numberOfNodes, numberOfThreads);
        return std::move(nodes);
    }

    static void
    createNodesFromInputCoordinates(list<array<double, dimensions>> coordinates, array<Node<dimensions>*, numberOfNodes>& nodes,
                                    char* heapPosition, int sortByDimension, unsigned numberOfThreads) {
        if (sortByDimension  >= 0) {
            auto compare = [sortByDimension](const array<double, dimensions> &a, const array<double, dimensions> &b) {
                return a[sortByDimension] < b[sortByDimension];
            };
            coordinates.sort(compare); // Corrected sort call for std::list
        }

        auto i = 0;
        for (auto &coordinate : coordinates) {
            nodes[i] = new(heapPosition + i * sizeof(Node<dimensions>)) Node<dimensions>(std::move(coordinate));
            i++;
        }
    }
    
private:
    NodeFactory() = default;
    ~NodeFactory() = default;
};

#endif //ALTAIRINTERVIEW_NODEFACTORY_H
