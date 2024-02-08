//
// Created by hal9000 on 2/8/24.
//

#include "HardwareAccelerationConfig.h"

HardwareAccelerationConfig::HardwareAccelerationConfig() {
    _threadsPerNode = std::thread::hardware_concurrency();
    _nodesPerThread = std::thread::hardware_concurrency();
    _cudaEnabled = false;
}


void HardwareAccelerationConfig::setThreadsPerNode(unsigned short value) {
    if (value < 1 || value > std::thread::hardware_concurrency())
        throw std::runtime_error("Threads per node should be between 1 and the" +
                                 std::to_string(std::thread::hardware_concurrency()));
    _threadsPerNode = value;
    
}

void HardwareAccelerationConfig::setNodesPerThread(unsigned short value) {
    if (value < 1 || value > std::thread::hardware_concurrency())
        throw std::runtime_error("Nodes per thread should be between 1 and the" +
                                 std::to_string(std::thread::hardware_concurrency()));
    _nodesPerThread = value;
}

void HardwareAccelerationConfig::enableCuda(bool enabled) {
    _cudaEnabled = enabled;
}

unsigned short HardwareAccelerationConfig::getThreadsPerNode(){
    return _threadsPerNode;
}

unsigned short HardwareAccelerationConfig::getNodesPerThread(){
    return _nodesPerThread;
}

bool HardwareAccelerationConfig::isCudaEnabled() {
    return _cudaEnabled;
}

