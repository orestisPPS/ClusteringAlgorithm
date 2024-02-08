//
// Created by hal9000 on 2/8/24.
//

#ifndef ALTAIRINTERVIEW_HARDWAREACCELERATIONCONFIG_H
#define ALTAIRINTERVIEW_HARDWAREACCELERATIONCONFIG_H

#include <thread>
#include <stdexcept>
class HardwareAccelerationConfig {


public:
    HardwareAccelerationConfig();
    
    void setThreadsPerNode(unsigned short value);
    void setNodesPerThread(unsigned short value);
    void enableCuda(bool enabled);
    unsigned short getThreadsPerNode();
    unsigned short getNodesPerThread();
    bool isCudaEnabled();
    
private:
    unsigned short _threadsPerNode;
    unsigned short _nodesPerThread;
    bool _cudaEnabled;
};


#endif //ALTAIRINTERVIEW_HARDWAREACCELERATIONCONFIG_H
