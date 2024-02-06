#include <iostream>
#include "NodeCloudFactory.h"
int main() {
    
    NodeCloudFactory factory = NodeCloudFactory();
    vector<double> directionToDomainLength = {1, 1};
    auto cloud = factory.createNodeCloud(directionToDomainLength,1E5);
    //auto cloud2 = factory.createNodeCloud({{4, 4}, {3, 3}, {2, 2}, {1, 1}});
    //auto cloud3 = factory.createNodeCloud({{0, 0}, {0.01, 1}, {0.02, 2}, {0.03, 3}, {0.04, 4}});

    cloud->calculateClusters(0.2);
    cout << "That's all folks!" << endl;
    
    return 0;
    
    
}
