//
// Created by hal9000 on 2/11/24.
//
#ifndef ALTAIRINTERVIEW_MULTITHREADINGTEST_H
#define ALTAIRINTERVIEW_MULTITHREADINGTEST_H

#include <chrono>
#include "Test.h"

template<unsigned dimensions, unsigned numberOfNodes, int sortByDimension = -1>
class MultithreadingTest : public Test {
public:
    explicit MultithreadingTest(double radius = 1) {
        _radius = radius;
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
        auto lengths = array<double, dimensions>();
        unsigned availableThreads = thread::hardware_concurrency();

        for (unsigned i = 0; i < dimensions; i++)
            lengths[i] = 1;
        auto singleThreadInitializationStart = chrono::high_resolution_clock::now();
        auto nodeCloudSingleThread = NodeCloud<dimensions, numberOfNodes>(lengths, 1, sortByDimension);
        auto singleThreadInitializationEnd = chrono::high_resolution_clock::now();
        auto singleThreadInitializationTime = chrono::duration_cast<chrono::duration<double>>(singleThreadInitializationEnd - singleThreadInitializationStart);
        
        auto multiThreadInitializationStart = chrono::high_resolution_clock::now();
        auto nodeCloudMultiThread = NodeCloud<dimensions, numberOfNodes>(lengths, availableThreads, sortByDimension);
        auto multiThreadInitializationEnd = chrono::high_resolution_clock::now();
        auto multiThreadInitializationTime = chrono::duration_cast<chrono::duration<double>>(multiThreadInitializationEnd - multiThreadInitializationStart);

        _printOutput("Node Initialization", availableThreads, singleThreadInitializationTime, multiThreadInitializationTime);
        // Define the number of nodes to be generated
        auto algorithms = vector<ClusteringAlgorithmType> {
                UNION_FIND_PER_NODE,
                UNION_FIND_BUNCH,
                DEPTH_FIRST_SEARCH
        };
        auto algorithmNames = vector<string>{
                "Union Find Per Node",
                "Union Find Bunch",
                "Depth First Search"
        };
        for (unsigned i = 0; i < algorithms.size(); i++) {
            auto singleThreadClusteringStart = chrono::high_resolution_clock::now();
            auto clusters = nodeCloudSingleThread.findClusters(_radius, algorithms[i]);
            auto singleThreadClusteringEnd = chrono::high_resolution_clock::now();
            auto singleThreadClustering = chrono::duration_cast<chrono::duration<double>>(singleThreadClusteringEnd - singleThreadClusteringStart);

            auto multiThreadClusteringStart = chrono::high_resolution_clock::now();
            clusters = nodeCloudMultiThread.findClusters(_radius, algorithms[i]);
            auto multiThreadClusteringEnd = chrono::high_resolution_clock::now();
            auto multiThreadClustering = chrono::duration_cast<chrono::duration<double>>(multiThreadClusteringEnd - multiThreadClusteringStart);
            
            _printOutput("Clustering " + algorithmNames[i], availableThreads, singleThreadClustering, multiThreadClustering);
        }
        //_consoleTestEnd();
    }

    private:
    
        double _radius;    
    
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
