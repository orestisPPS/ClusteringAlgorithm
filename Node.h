
#ifndef ALTAIRINTERVIEW_NODE_H
#define ALTAIRINTERVIEW_NODE_H

#include <vector>
#include <memory>
#include <stdexcept>
using namespace std;

/**
 * @brief Represents a node in a graph or spatial data structure.
 * @details This class represents a node in a graph or spatial data structure. Each node has a unique
 * identifier and a position in n-dimensional space.
 */
class Node {
public:
    /**
     * @brief Constructs a new Node object with a given ID.
     * @param id The unique identifier for the node.
     * @details This constructor initializes a Node object with a specific ID and initializes an empty position vector.
     */
    explicit Node(unsigned id);

    /**
     * @brief Gets the ID of the node.
     * @return unsigned The unique identifier of the node.
     * @details This method returns the ID assigned to the node at construction, providing a means
     * to uniquely identify the node.
     */
    unsigned getId() const;

    /**
     * @brief Gets a constant reference to the vector of coordinates.
     * @return const vector<double>& A constant reference to the vector containing the node's coordinates.
     * @details Provides read-only access to the node's coordinates vector. This method ensures that
     * the coordinates can be accessed efficiently while maintaining the integrity of the node's data.
     */
    const vector<double>& getCoordinatesVector() const;

    /**
     * @brief Sets the coordinates vector for the node.
     * @param coordinates A unique pointer to the new coordinates vector.
     * @throws runtime_error if the input coordinates are null.
     * @details Allows the assignment of a new coordinates vector to the node, replacing any existing
     * coordinates. This method ensures that the node's position can be updated while maintaining
     * the integrity of the data.
     */
    void setCoordinatesVector(unique_ptr<vector<double>> coordinates);

private:
    /** 
     * @brief Unique pointer to the position vector of the node in the n-dimensional space.
     * @details This unique pointer manages the coordinates of the node, ensuring proper memory
     * management and encapsulation of the node's position data.
     */
    unique_ptr<vector<double>> _coordinates;

    /** 
     * @brief The unique identifier of the node, assigned at construction.
     * @details This identifier is used to uniquely distinguish the node within a graph or
     * spatial data structure.
     */
    unsigned _id;
};

#endif // ALTAIRINTERVIEW_NODE_H
