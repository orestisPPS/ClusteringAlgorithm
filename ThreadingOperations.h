//
// Created by hal9000 on 2/7/24.
//

#ifndef ALTAIRINTERVIEW_THREADINGOPERATIONS_H
#define ALTAIRINTERVIEW_THREADINGOPERATIONS_H

#include <stdexcept>
#include <memory>
#include <thread>
#include <vector>
using namespace std;

template<typename T>
class ThreadingOperations {

public:

/**
    * \brief Executes the provided task in parallel across multiple threads. Each thread operates on a distinct segment
    * of the data.
    * \tparam T The data type of the result (e.g., double, float).
    * \tparam ThreadJob A callable object type (function, lambda, functor).
    *
    * \param size The size of the data being processed.
    * \param task The callable object that describes the work each thread should execute and return a local result.
     * \param availableThreads The number of threads available for processing.
    * \param cacheLineSize An optional parameter to adjust for system's cache line size (default is 64 bytes).
    * 
    * \return The combined result after the reduction step.
    */
    template<typename ThreadJob>
    static void executeParallelJob(ThreadJob task, size_t arraySize, size_t objectSize, unsigned availableThreads, unsigned cacheLineSize = 64) {
        //Determine the number of doubles that fit in a cache line
        unsigned objectsPerCacheLine = cacheLineSize / objectSize;
        unsigned int numThreads = std::min(availableThreads, static_cast<unsigned>(arraySize));
        //Determine the block size.
        unsigned blockSize = (arraySize + numThreads - 1) / numThreads;
        blockSize = (blockSize + objectsPerCacheLine - 1) / objectsPerCacheLine * objectsPerCacheLine;
        if (blockSize * numThreads > arraySize) {
            blockSize = arraySize / numThreads;
        }
        vector<thread> threads;
        for (unsigned int i = 0; i < numThreads; ++i) {
            unsigned start = i * blockSize;
            unsigned end = start + blockSize;
            if (start >= arraySize)
                break;
            end = std::min(end, static_cast<unsigned>(arraySize));
            threads.push_back(thread(task, start, end));
        }

        for (auto &thread: threads) {
            thread.join();
        }
    }
};
#endif //ALTAIRINTERVIEW_THREADINGOPERATIONS_H
