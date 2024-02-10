cmake_minimum_required(VERSION 3.27)
project(altairInterview)

set(CMAKE_CXX_STANDARD 17)

# Attempt to enable CUDA language. If it fails, continue without CUDA.
include(CheckLanguage)
check_language(CUDA)

if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    add_definitions(-DWITH_CUDA)
else()
    message(WARNING "CUDA not found: Building without CUDA support")
endif()

add_executable(altairInterview main.cpp
        Node.cpp
        Node.h
        NodeCloud.cpp
        NodeCloud.h
        NodeCloudFactory.cpp
        NodeCloudFactory.h
        HardwareAcceleration/HardwareAcceleration.h
        Tests/Test.cpp
        Tests/Test.h
        Tests/ProofOfConceptTest.h
        HardwareAcceleration/CudaAcceleration.cuh
        Tests/AltairTask.h
        NodeCluster.cpp
        NodeCluster.h
)
