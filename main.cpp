#include <iostream>
#include <chrono>
#include "Tests/ProofOfConceptTest.h"
#include "Tests/AltairExample.h"
#include "Tests/RandomDomain2DTest.h"
#include "Tests/MultithreadingTest.h"
using namespace std::chrono;

int main() {
    
    auto start = high_resolution_clock::now();
    
    ProofOfConceptTest().run();
    AltairExample().run();
    RandomDomain2DTest().run();
    MultithreadingTest().run();

    //cout << "radius : " << radius << " Cluster size : " << cloudInterview->calculateClusters(radius).size() << endl;

    //cloud->hardwareAccelerationConfig.enableCuda(true);
    //auto cloud = NodeCloudFactory::createNodeCloud(directionToDomainLength,4E0);


    double radius = 1;   
    //auto clusters = cloud->calculateClusters(radius);
    //cout << "radius : " << radius << " Cluster size : " << clusters.size() << endl;
//    
//    cout << "That's all folks!" << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << duration.count() << "ms" << endl;
    return 0;
    
    //r1 = 0.7, r2 = 1.8, r3 = 4.8, r4 = 6.5, r5 = 10.6 
    
}
