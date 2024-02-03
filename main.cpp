#include <iostream>
#include "NodeCloudFactory.h"
int main() {
    
    NodeCloudFactory factory = NodeCloudFactory();
    map<unsigned short, double> directionToDomainLength = {{1, 1}, {2, 1}};
    auto cloud = factory.createNodeCloud(directionToDomainLength,10);
    
    cout << "That's all folks!" << endl;
    
    return 0;
    
    
}
