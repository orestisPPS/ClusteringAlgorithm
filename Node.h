#ifndef ALTAIRINTERVIEW_NODE_H
#define ALTAIRINTERVIEW_NODE_H

#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;

/**
 * @brief Represents a Node in an n-dimensional space.
 *
 * This class encapsulates a point within a graph or spatial data structure, identifiable by a unique ID
 * and positioned within an n-dimensional space through a vector of coordinates.
 */
class Node {
public:
    /**
     * @brief Constructs a new Node object with a given ID.
     *
     * @param id The unique identifier for the node.
     */
    explicit Node(unsigned id);

    /**
     * @brief Gets the ID of the node.
     * 
     * @return unsigned The ID of the node.
     */
    unsigned getId() const;

    /**
     * @brief Gets a constant reference to the shared pointer of the coordinates vector.
     *
     * Provides read-only access to the node's coordinates vector through a shared pointer. This method ensures
     * the coordinates vector itself cannot be directly modified or reassigned. However, the contents of the vector,
     * representing the node's position in n-dimensional space, can still be modified if necessary.
     *
     * @return const std::shared_ptr<std::vector<double>>& A constant reference to the shared pointer of the coordinates vector.
     * @throws std::runtime_error if coordinates are not assigned.
     */
    const std::shared_ptr<std::vector<double>>& getCoordinatesVector() const;

    /**
     * @brief Sets the coordinates vector for the node.
     *
     * Allows the assignment of a new coordinates vector to the node, representing its position in n-dimensional space.
     * *
     * @param coordinates The shared pointer to the new coordinates vector.
     * @throws std::runtime_error if the input coordinates are null or empty.
     */
    void setCoordinatesVector(std::shared_ptr<std::vector<double>> coordinates);

protected:
    /** @brief Shared pointer to the position vector of the node in the n-dimensional space. */
    std::shared_ptr<std::vector<double>> _coordinates;

    /** @brief The unique identifier of the node, assigned at construction. */
    unsigned _id;
};

#endif // ALTAIRINTERVIEW_NODE_H
