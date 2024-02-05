#include <iostream>
#include "NodeCloudFactory.h"
int main() {
    
    NodeCloudFactory factory = NodeCloudFactory();
    map<unsigned short, double> directionToDomainLength = {{1, 1}, {2, 1}};
    //auto cloud = factory.createNodeCloud(directionToDomainLength,10);
    //auto cloud2 = factory.createNodeCloud({{4, 4}, {3, 3}, {2, 2}, {1, 1}});
    auto cloud3 = factory.createNodeCloud({{0, 0}, {0.01, 1}, {0.02, 2}, {0.03, 3}, {0.04, 4}});

    cloud3->calculateClusters(0.1);
    cout << "That's all folks!" << endl;
    
    return 0;
    
    
}
