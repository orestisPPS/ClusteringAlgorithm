cmake_minimum_required(VERSION 3.24)
project(altairInterview)

set(CMAKE_CXX_STANDARD 17)

add_executable(altairInterview main.cpp
        Node.h
        NodeCloud.h
        ThreadingOperations.h
        Tests/Test.cpp
        Tests/Test.h
        Tests/ProofOfConceptTest.h
        Tests/AltairExample.h
        Cluster.h
        Tests/RandomDomainND.h
        Tests/MultithreadingTest.h
        UnionFind.h
        ClusteringAlgorithms/ClusteringAlgorithm.h
        NodeFactory.h
        ClusteringAlgorithms/UnionFindBunchClustering.h
        ClusteringAlgorithms/UnionFindClustering.h
        ClusteringAlgorithms/UnionFindPerNodeClustering.h
        ClusteringAlgorithms/DepthFirstSearchClustering.h
        ClusteringAlgorithms/ImmediateClustering.h
        Tests/AlgorithmsTimeComparisonTest.h
)
