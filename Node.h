
#ifndef ALTAIRINTERVIEW_NODE_H
#define ALTAIRINTERVIEW_NODE_H

#include <array>
#include <memory>
#include <stdexcept>

using namespace std;

/**
 * @brief Represents a node in a graph or spatial data structure.
 * @details This class represents a node in a graph or spatial data structure. Each node has a unique
 * identifier and a position in n-dimensional space.
 */
template <unsigned dimensions>
class Node {
public:
    /**
     * @brief Constructs a new Node object with the given coordinate vector.
     * @param coordinates A vector of doubles representing the position of the node in n-dimensional space.
     */
    explicit Node(array<double, dimensions> coordinates) : _coordinates(std::move(coordinates)){}
    /**
     * @brief Gets a constant reference to the vector of coordinates.
     * @return const vector<double>& A constant reference to the vector containing the node's coordinates.
     * @details Provides read-only access to the node's coordinates vector.
     */
    const array<double, dimensions>& getCoordinatesVector() const;

private:
    /** 
     * @brief Unique pointer to the position vector of the node in the n-dimensional space.
     * @details This unique pointer manages the coordinates of the node
     */
    array<double, dimensions> _coordinates;

    static_assert(dimensions > 0, "Node must have at least one dimension.");
    static_assert(dimensions < 4, "Node must have at most three dimensions.");
};

#endif // ALTAIRINTERVIEW_NODE_H
