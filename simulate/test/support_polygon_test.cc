#include "support_polygon.h"

#include <cassert>
#include <vector>

int main()
{
    using support_polygon::Point2;
    const std::vector<Point2> contacts{
        {1.0, 1.0}, {-1.0, -1.0}, {1.0, -1.0}, {-1.0, 1.0},
        {0.0, 0.0}, {1.0, 1.0},
    };

    const auto hull = support_polygon::convexHull(contacts);
    assert(hull.size() == 4);
    assert(support_polygon::contains(hull, {0.0, 0.0}));
    assert(support_polygon::contains(hull, {1.0, 0.0}));
    assert(!support_polygon::contains(hull, {1.01, 0.0}));

    const std::vector<Point2> line{{-1.0, 0.0}, {1.0, 0.0}};
    assert(!support_polygon::contains(line, {0.0, 0.0}));
}
