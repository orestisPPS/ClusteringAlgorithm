//
// Created by hal9000 on 2/11/24.
//
#ifndef ALTAIRINTERVIEW_MULTITHREADINGTEST_H
#define ALTAIRINTERVIEW_MULTITHREADINGTEST_H

#include <chrono>
#include "Test.h"

class MultithreadingTest : public Test {
public:
    MultithreadingTest() {
        _name = "Multithreading";
    }

    void run() override{
        _consoleTestStart();
        // Define the length of each dimension of the domain, in this case a unit square
        auto domainLengths = vector<double>{1, 1};
        // Define the number of nodes to be generated
        unsigned numberOfNodes = 1E3;
        //unsigned availableThreads = thread::hardware_concurrency();
        unsigned availableThreads = 8;

        // Measure the time of initialization and clustering for a single and multiple threads
        auto singleThreadTime = _measureTime(1, numberOfNodes);
        auto multiThreadTime = _measureTime(availableThreads, numberOfNodes);
        auto singleThreadInitializationTime = singleThreadTime.first;
        auto singleThreadClustering = singleThreadTime.second;
        auto multiThreadInitializationTime = multiThreadTime.first;
        auto multiThreadClustering = multiThreadTime.second;
        
        _printOutput("Node Initialization", numberOfNodes, availableThreads, singleThreadInitializationTime, multiThreadInitializationTime);
        _printOutput("Clustering", numberOfNodes, availableThreads, singleThreadClustering, multiThreadClustering);
        _printOutput("Total", numberOfNodes, availableThreads, singleThreadInitializationTime + singleThreadClustering, multiThreadInitializationTime + multiThreadClustering);
        
        auto timeImprovement = (singleThreadInitializationTime + singleThreadClustering - multiThreadInitializationTime - multiThreadClustering) /
                              (singleThreadInitializationTime + singleThreadClustering) * 100;
        if (timeImprovement > 65) {
            _passed = true;
        }
        _consoleTestEnd();
    }
    
    private:
        static pair<chrono::duration<double>, chrono::duration<double>> _measureTime(unsigned availableThreads, unsigned numberOfNodes) {
            auto start = chrono::high_resolution_clock::now();
            auto nodeCloud = NodeCloudFactory::createNodeCloud(vector<double>{1, 1}, numberOfNodes, availableThreads);
            auto end = chrono::high_resolution_clock::now();
            auto initializationTime = chrono::duration_cast<chrono::milliseconds>(end - start);
            
            start = chrono::high_resolution_clock::now();
            nodeCloud->calculateClusters(1, availableThreads);
            end = chrono::high_resolution_clock::now();
            auto clusterTime = chrono::duration_cast<chrono::milliseconds>(end - start);
            
            return {initializationTime, clusterTime};
        }
        
        static void _printOutput(string  processName, unsigned numberOfNodes, unsigned availableThreads, chrono::duration<double> singleThreadTime, chrono::duration<double> multiThreadTime) {
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
