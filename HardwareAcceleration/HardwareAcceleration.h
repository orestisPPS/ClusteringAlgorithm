//
// Created by hal9000 on 2/7/24.
//

#ifndef ALTAIRINTERVIEW_HARDWAREACCELERATION_H
#define ALTAIRINTERVIEW_HARDWAREACCELERATION_H

#include <stdexcept>
#include <memory>
#include <thread>
#include <vector>
using namespace std;

template<typename T>
class HardwareAcceleration {

public:

/**
    * \brief Executes the provided task in parallel across multiple threads. Each thread operates on a distinct segment
    * of the data.
    * \tparam ThreadJob A callable object type (function, lambda, functor).
    * \param arraySize The size of the data being processed.
    * \param objectSize The size of the object being processed.
     * \param availableThreads The number of threads available for processing.
    * \param cacheLineSize An optional parameter to adjust for system's cache line size (default is 64 bytes).

    */
    template<typename ThreadJob>
    static void executeParallelJob(ThreadJob task, size_t arraySize, size_t objectSize,
                                   unsigned availableThreads, unsigned cacheLineSize = 64) {
        //Determine the number of doubles that fit in a cache line
        unsigned int numThreads = std::min(availableThreads, static_cast<unsigned>(arraySize));
        
        //Determine the block size.
        unsigned blockSize = (arraySize / numThreads) ;

        vector<thread> threads;
        for (unsigned int i = 0; i < numThreads; ++i) {
            unsigned start = i * blockSize;
            unsigned end = start + blockSize;
            if (start >= arraySize)
                break;
            end = std::min(end, static_cast<unsigned>(arraySize));
            if (i == numThreads - 1) {
                end = arraySize;
            }
            threads.push_back(thread(task, start, end));
        }

        for (auto &thread: threads) {
            thread.join();
        }
    }
};
#endif //ALTAIRINTERVIEW_HARDWAREACCELERATION_H
