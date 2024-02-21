//
// Created by hal9000 on 2/7/24.
//

#ifndef ALTAIRINTERVIEW_THREADINGOPERATIONS_H
#define ALTAIRINTERVIEW_THREADINGOPERATIONS_H

#include <stdexcept>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

template<typename T>
class ThreadingOperations {

public:

    /**
     * \brief Executes a given task in parallel, distributing the workload across multiple threads.
     *
     * Template-based function that divides a contiguous data array into nearly equal segments, each processed by a separate thread.
     * It ensures all segments are processed by adjusting the last segment's end index to cover the entire array if needed.
     *
     * \tparam ThreadJob A callable object type that takes two parameters: the start and end indices of the segment to process.
     *                   The callable object is expected to handle elements from 'start' up to but not including 'end'.
     * \param arraySize The total number of elements in the data array to be processed.
     * \param availableThreads The number of threads to be utilized for processing the data. The actual number of threads
     *                         used will be the lesser of this parameter and the total size of the data array.
     * Example usage:
     * \code{.cpp}
     * executeParallelJob([](unsigned start, unsigned end) {
     *     for(unsigned i = start; i < end; ++i) {
     *         // Process element at index i
     *     }
     * }, 1000, std::thread::hardware_concurrency());
     * \endcode
     */
    template<typename ThreadJob>
    static void executeParallelJob(ThreadJob task, size_t arraySize, unsigned availableThreads) {
        
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
    
    constexpr static unsigned getAvailableThreads(){
        return std::thread::hardware_concurrency();
    }
};
#endif //ALTAIRINTERVIEW_THREADINGOPERATIONS_H
