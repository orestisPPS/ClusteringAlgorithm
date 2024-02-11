#include <iostream>
#include <chrono>
#include "Tests/ProofOfConceptTest.h"
#include "Tests/AltairTask.h"
using namespace std::chrono;

int main() {
    
    auto start = high_resolution_clock::now();
    
    vector<double> directionToDomainLength = {1, 1};
    auto cloud = NodeCloudFactory::createNodeCloud(directionToDomainLength, 1E4);
    //cout << "radius : " << radius << " Cluster size : " << cloudInterview->calculateClusters(radius).size() << endl;

    //cloud->hardwareAccelerationConfig.enableCuda(true);
    //auto cloud = NodeCloudFactory::createNodeCloud(directionToDomainLength,4E0);

    //auto cloud2 = factory.createNodeCloud({{4, 4}, {3, 3}, {2, 2}, {1, 1}});
    //auto cloudInterview = NodeCloudFactory::createNodeCloud({{3.2, 5.11}, {11.4, 5}, {0, 5.1}, {3.21,0}, {12.8, 4.9}});

    //auto cloud3 = NodeCloudFactory::createNodeCloud({{0, 0}, {0.01, 1}, {0.02, 2}, {0.03, 3}, {0.04, 4}});
    double radius = 1;   
    auto clusters = cloud->calculateClusters(radius);
    cout << "radius : " << radius << " Cluster size : " << clusters.size() << endl;
    
    cout << "That's all folks!" << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << duration.count() << "ms" << endl;
    return 0;
    
    //r1 = 0.7, r2 = 1.8, r3 = 4.8, r4 = 6.5, r5 = 10.6 
    
}
