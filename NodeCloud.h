//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUD_H
#define ALTAIRINTERVIEW_NODECLOUD_H

#include <map>
#include <unordered_map>
#include <list>
#include <cmath>
#include <algorithm>
#include "NodeCluster.h"
#include "ThreadingOperations.h"

class NodeCloud {
public:
    explicit NodeCloud(vector<Node *> nodesVector);

    /**
     * @brief Identifies and returns the neighbouring nodes of a given node within a specified radius.
     *
     * Utilizes _coordinateComponentToNodeMaps to narrow down potential neighbours,
     * minimizing direct distance calculations. Iterates over each dimension of the node's space, using
     * the coordinate component maps to find the nodes that fall within the search radius along that dimension
     * (find complexity per direction 2log n). If a node is found to be within the radius in one dimension, an index
     * is incremented in a candidateNodeCounter hashmap. If the index reaches the number of dimensions, the L2 norm
     * between the node and the candidate node is calculated and if it is within the radius, the candidate is added to the
     * list of filtered nodes.
     *
     * @param node shared_ptr to the node for which neighbours are being sought.
     * @param radius The search radius
     * @return A list of Node* objects, each pointing to a neighbour of the input node within the specified radius.
     */
    list<Node*> findNeighboursOfNode(Node* node, double radius);

    /**
    * @brief Calculates clusters of nodes within a specified radius and returns them.

    * The method leverages the `findNeighboursOfNode` to populate `_nodeToNeighbours` map with each node's neighbours.
    * Then, for each unvisited node, it starts a new cluster and recursively adds all its reachable neighbours to this cluster,
    * marking them as visited.
    *
    * @param radius The radius within which nodes are considered to be in the same cluster.
    * @param availableThreads The number of available threads (default is 1).
    * @return A list of shared_ptr<NodeCluster> objects, each representing a distinct cluster of nodes.
    */
    list<NodeCluster> calculateClusters(double radius, unsigned availableThreads = 1);
    
    /**
     * @brief Returns a constant reference to the vector of Node objects.
     * @return A constant reference to the vector of shared pointers to the nodes of the cloud.
     */
    const vector<Node*> & getNodes() const;

    ~NodeCloud();

private:

private:
    /**
     * @brief Unique pointer to a vector of shared pointers to Node objects.
     * Stores all the pointers to the nodes that represent the cloud.
     */
    vector<Node*> _nodes;

    /**
     * @brief The dimensionality of the n-dimensional space that the cloud occupies.
     */
    unsigned _dimensions;

    /**
     * @brief Maps each node to a list of its neighbouring nodes within a specified radius.
     *
     * An unordered map for fast access of a node's neighbouring nodes.
     * The key is a shared_ptr to a Node, while the value is a list of shared_ptr to Node, representing the neighbouring nodes.
     * All list values are cleared after each cluster calculation.
     */
    unique_ptr<unordered_map<Node*, list<Node*>>> _nodeToNeighbours;

    /**
     * @brief Tracks whether a node has been visited during cluster calculation.
     *
     * An unordered map for fast access of a node's visited state (0(1)).
     * The key is a shared_ptr to a Node, and the value is a boolean indicating whether the node has been visited during
     * the recursive cluster calculation in order to avoid redundant operations.
     * Each value is reset to false after each cluster calculation.
     */
    unique_ptr<unordered_map<Node*, bool>> _visitedNodes;

    /**
     * @brief Sorts nodes based on their coordinate components to optimize spatial search.
     *
     * Vector of maps, each corresponding to one dimension in the n-dimensional space.
     * It is facilitates spatial sorting by organizing nodes according to their coordinate values in each dimension 
     * allowing for targeted access to subsets of nodes based on their location in space, significantly bounding
     * the search space within a specified radius (from O(n^2) to O(n log n) for all nodes).
     * The key of each map is a double representing a specific coordinate component, and the value is a list
     * of shared_ptr to Node, containing all nodes that have this coordinate component in the corresponding dimension.
     * It is populated during the construction of the NodeCloud and is not modified afterwards.
     */
    unique_ptr<vector<map<double, list<Node*>>>> _coordinateComponentToNodeMaps;

    /**
    * @brief Evaluates whether a candidate node is a neighbour of the given node based on the specified radius.
    *
    * Determines if the Euclidean distance between the given node and a candidate node is within a specified radius.
    * If the distance is within the specified radius the method returns true.
    *
    * @param thisNode Shared pointer to node from which the distance is being measured
    * @param candidateNode Shared pointer to the node being assessed for neighbour status
    * @param radius The radius within which the candidate node must fall to be considered a neighbour.
    * @param filteredNodes A reference to a list of neighbouring nodes.
    * @return bool True if the candidate node is within the specified radius of the given node, false otherwise.
    */
    bool _assessNeighbour(const Node &thisNode, const Node &candidateNode, double radius, list<Node*>& filteredNodes) const;
    

    /**
    * @brief Clusters recursively neighbouring nodes starting from a given node until reaching a leaf node.
    *
    * Starting from a specified node, this method recursively explores all neighbouring nodes 
    * adding them to the current cluster. It uses the `_nodeToNeighbours` map to find neighbours and the `_visitedNodes`
    * map to track which nodes have already been included in a cluster. It ensures that all nodes reachable from the starting
    * node, directly or indirectly, are grouped into a single cluster.
    *
    * @param node The starting node from which the recursive neighbour search is initiated.
    * @param radius The radius used to determine neighbours for clustering.
    * @param cluster The cluster object to which discovered nodes are added.
    */
    void _searchNeighboursRecursively(Node *node, NodeCluster &cluster);

};


#endif //ALTAIRINTERVIEW_NODECLOUD_H
