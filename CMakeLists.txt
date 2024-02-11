cmake_minimum_required(VERSION 3.27)
project(altairInterview)

set(CMAKE_CXX_STANDARD 17)

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
        Tests/AltairExample.h
        NodeCluster.cpp
        NodeCluster.h
        Tests/RandomDomain2DTest.h
        Tests/MultithreadingTest.h
)
