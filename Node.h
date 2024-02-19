
#ifndef ALTAIRINTERVIEW_NODE_H
#define ALTAIRINTERVIEW_NODE_H

#include <array>
#include <memory>
#include <stdexcept>

using namespace std;


template <unsigned dimensions>
class Node {
public:

    explicit Node(array<double, dimensions> coordinates) : _coordinates(std::move(coordinates)){}

    inline const array<double, dimensions>& getCoordinatesVector() const {
        return _coordinates;
    }

private:

    array<double, dimensions> _coordinates;

    static_assert(dimensions > 0, "Node must have at least one dimension.");
    static_assert(dimensions < 4, "Node must have at most three dimensions.");
};

#endif // ALTAIRINTERVIEW_NODE_H
