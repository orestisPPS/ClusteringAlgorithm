
#ifndef ALTAIRINTERVIEW_NODE_H
#define ALTAIRINTERVIEW_NODE_H

#include <array>
#include <memory>
#include <stdexcept>

using namespace std;


template <unsigned dimensions>
class Node {
public:

    explicit Node(array<double, dimensions> coordinates) : _coordinates(coordinates){}
    
    //Move constructor
    Node(Node &&other) noexcept : _coordinates(other._coordinates) {}

    constexpr const array<double, dimensions>& getCoordinatesVector() const {
        return _coordinates;
    }
    
    constexpr double distanceFrom(const Node<dimensions> &other) const {
        return sqrt(_sumOfSquaresFromUnrolled(other));
    }

    constexpr double sumOfSquaresFrom(const Node<dimensions> &other) const {
        return _sumOfSquaresFromUnrolled<dimensions>(other);
    }

private:

    array<double, dimensions> _coordinates;
    
    template<unsigned iDimension>
    constexpr double _sumOfSquaresFromUnrolled(const Node<dimensions> &other) const {
        if constexpr (iDimension > 0) {
            double diff = _coordinates[iDimension - 1] - other._coordinates[iDimension - 1];
            return diff * diff + _sumOfSquaresFromUnrolled<iDimension - 1>(other);
        } else {
            return 0;
        }
    }

    static_assert(dimensions > 0, "Node must have at least one dimension.");
    static_assert(dimensions < 4, "Node must have at most three dimensions.");
};

#endif // ALTAIRINTERVIEW_NODE_H
