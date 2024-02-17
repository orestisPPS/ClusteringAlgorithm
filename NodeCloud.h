//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUD_H
#define ALTAIRINTERVIEW_NODECLOUD_H

#include <random>
#include <list>
#include <cmath>
#include "Node.h"
#include "ThreadingOperations.h"

template<unsigned short dimensions, unsigned numberOfNodes>
class NodeCloud {
public:
    
    explicit NodeCloud(array<double, dimensions> dimensionsLengths, unsigned availableThreads = 1) {
        static_assert(dimensionsLengths.size() == dimensions, "Invalid number of dimension lengths");

        // Allocate contiguous memory for the nodes
        _blockOfNodes = new char[sizeof(Node<dimensions>) * numberOfNodes];

        // Create distributions for each dimension
        array<uniform_real_distribution<double>, dimensions> distributionsVector;
        for (unsigned i = 0; i < dimensions; i++) {
            distributionsVector[i] = uniform_real_distribution<double>(0, dimensionsLengths[i]);
        }

        // Node initialization job for parallel execution
        auto nodeInitializationJob = [&](unsigned start, unsigned end) {
            random_device randomDevice;
            mt19937 generator(randomDevice());
            for (unsigned i = start; i < end; i++) {
                array<double, dimensions> nodalCoordinates;
                for (unsigned j = 0; j < dimensions; j++) {
                    nodalCoordinates[j] = distributionsVector[j](generator);
                }
                _nodes[i] = new (_blockOfNodes + i * sizeof(Node<dimensions>)) Node<dimensions>(nodalCoordinates);
            }
        };
        // Assuming ThreadingOperations has a method to execute the job in parallel
        ThreadingOperations<Node<dimensions>*>::executeParallelJob(nodeInitializationJob, numberOfNodes, availableThreads);
        bool ool= false;
    }
    ~NodeCloud() {
        for (unsigned i = 0; i < numberOfNodes; i++) {
            // Manually call the destructor for each Node
            _nodes[i]->~Node();
        }
        // Deallocate the block of nodes
        delete[] _blockOfNodes;
    }
    
private:
    char* _blockOfNodes; // Contiguous memory for Node objects
    
    array<Node<dimensions>*, numberOfNodes> _nodes; // Array of pointers to Node objects

};




#endif //ALTAIRINTERVIEW_NODECLOUD_H
