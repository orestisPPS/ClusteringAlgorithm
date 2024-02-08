#include <iostream>
#include "NodeCloudFactory.h"
int main() {
    
    vector<double> directionToDomainLength = {1, 1};
    auto cloud = NodeCloudFactory::createNodeCloud(directionToDomainLength,4E0);
    //cloud->hardwareAccelerationConfig.enableCuda(true);
    //auto cloud = NodeCloudFactory::createNodeCloud(directionToDomainLength,4E0);

    //auto cloud2 = factory.createNodeCloud({{4, 4}, {3, 3}, {2, 2}, {1, 1}});
    //auto cloud3 = NodeCloudFactory::createNodeCloud({{0, 0}, {0.01, 1}, {0.02, 2}, {0.03, 3}, {0.04, 4}});
    double radius = 0.3;
    cout << "radius : " << radius << " Cluster size : " << cloud->calculateClusters(radius) << endl;
    cout << "That's all folks!" << endl;
    
    return 0;
    
    
}
