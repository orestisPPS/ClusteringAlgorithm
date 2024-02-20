//
// Created by hal9000 on 2/11/24.
//
#ifndef ALTAIRINTERVIEW_MULTITHREADINGTEST_H
#define ALTAIRINTERVIEW_MULTITHREADINGTEST_H

#include <chrono>
#include "Test.h"

template<unsigned dimensions, unsigned numberOfNodes>
class MultithreadingTest : public Test {
public:
    MultithreadingTest() {
        _name = "Multithreading";
    }

    /**
    * @brief Tests the time improvement of the initialization and clustering processes for 3E3 Nodes
    * for a 1 and multiple threads.
    * 
    * @note Passes if the time improvement percentage ((t_single - t_multi) / t_single * 100) is greater than 50% and fails otherwise.
    * @note This method uses all available threads of the machine (std::thread::hardware_concurrency()).
    */
    void run() override{
        _consoleTestStart();
        // Define the length of each dimension of the domain, in this case a unit square
        // Define the number of nodes to be generated
        unsigned availableThreads = thread::hardware_concurrency();

        // Measure the time of initialization and clustering for a single and multiple threads
        auto singleThreadTime = _measureTime(1);
        auto singleThreadInitializationTime = singleThreadTime.first;
        auto singleThreadClustering = singleThreadTime.second;

        auto multiThreadTime = _measureTime(12);
        auto multiThreadInitializationTime = multiThreadTime.first;
        auto multiThreadClustering = multiThreadTime.second;

        _printOutput("Node Initialization", availableThreads, singleThreadInitializationTime, multiThreadInitializationTime);
        _printOutput("Clustering", availableThreads, singleThreadClustering, multiThreadClustering);
        _printOutput("Total", availableThreads, singleThreadInitializationTime + singleThreadClustering, multiThreadInitializationTime + multiThreadClustering);

        auto timeImprovement = (singleThreadInitializationTime + singleThreadClustering - multiThreadInitializationTime - multiThreadClustering) /
                              (singleThreadInitializationTime + singleThreadClustering) * 100;
        if (timeImprovement > 50) {
            _passed = true;
        }
        _consoleTestEnd();
    }

    private:
        /**
         * @brief Measures the time of initialization and clustering for a given number of nodes and available threads.
         * @param availableThreads Available threads
         * @param numberOfNodes Number of nodes in the NodeCloud.
         * @return A pair of the initialization and clustering time.
         */
        static pair<chrono::duration<double>, chrono::duration<double>> _measureTime(unsigned availableThreads) {
            auto start = chrono::high_resolution_clock::now();
            auto nodeCloud = NodeCloud<dimensions, numberOfNodes>({1,1}, availableThreads);
            auto end = chrono::high_resolution_clock::now();
            //auto initializationTime = chrono::duration_cast<chrono::seconds>(end - start);
            auto initializationTime = chrono::duration_cast<chrono::duration<double>>(end - start);

            cout << "NodeCloud with " << numberOfNodes << " nodes and " << availableThreads << " threads created" << endl;

            start = chrono::high_resolution_clock::now();
            auto clusters = nodeCloud.calculateClusters(1, availableThreads);
            end = chrono::high_resolution_clock::now();
            //auto clusterTime = chrono::duration_cast<chrono::seconds>(end - start);
            auto clusterTime = chrono::duration_cast<chrono::duration<double>>(end - start);

            cout << "NodeCloud with " << numberOfNodes << " nodes and " << availableThreads << "clustered" << endl;
            return {initializationTime, clusterTime};
        }

        /**
         * @brief Prints the number of nodes, threads, the execution time, the acceleration and time improvement percentage
         *        of the process for 1 and multiple threads.
         * @param processName Name of the process being measured.
         * @param numberOfNodes Number of nodes in the NodeCloud.
         * @param availableThreads Available threads
         * @param singleThreadTime Time of the process for a single thread.
         * @param multiThreadTime Time of the process for multiple threads.
         */
        static void _printOutput(string  processName, unsigned availableThreads, chrono::duration<double> singleThreadTime, chrono::duration<double> multiThreadTime) {
            cout << "Process: " << processName << endl;
            cout <<"--------------------------------" << endl;
             cout << "Number of nodes: " << numberOfNodes << endl;
            cout << "Number of threads: " << availableThreads << endl;
            cout << "Execution time [ms] for 1 / " << availableThreads << " threads: " << singleThreadTime.count() << " / " << multiThreadTime.count() << endl;
            cout << "Acceleration (t_single / t_multi) " << singleThreadTime.count() / multiThreadTime.count() << endl;
            cout << "Time Improvement Percentage: " << (singleThreadTime.count() - multiThreadTime.count()) / singleThreadTime.count() * 100 << "%" << endl;
            cout << endl;    
    }
};
#endif //ALTAIRINTERVIEW_MULTITHREADINGTEST_H
