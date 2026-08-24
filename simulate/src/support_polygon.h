#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

namespace support_polygon
{
struct Point2
{
    double x;
    double y;
};

inline double cross(const Point2& origin, const Point2& a, const Point2& b)
{
    return (a.x - origin.x) * (b.y - origin.y) -
           (a.y - origin.y) * (b.x - origin.x);
}

inline std::vector<Point2> convexHull(std::vector<Point2> points)
{
    std::sort(points.begin(), points.end(), [](const Point2& a, const Point2& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
    points.erase(std::unique(points.begin(), points.end(), [](const Point2& a, const Point2& b) {
        return std::fabs(a.x - b.x) < 1.0e-9 && std::fabs(a.y - b.y) < 1.0e-9;
    }), points.end());
    if (points.size() < 3) return points;

    std::vector<Point2> hull(2 * points.size());
    std::size_t count = 0;
    for (const auto& point : points)
    {
        while (count >= 2 && cross(hull[count - 2], hull[count - 1], point) <= 0.0)
            --count;
        hull[count++] = point;
    }
    const std::size_t lower_count = count + 1;
    for (auto it = points.rbegin() + 1; it != points.rend(); ++it)
    {
        while (count >= lower_count && cross(hull[count - 2], hull[count - 1], *it) <= 0.0)
            --count;
        hull[count++] = *it;
    }
    hull.resize(count - 1);
    return hull;
}

inline bool contains(const std::vector<Point2>& polygon, const Point2& point)
{
    if (polygon.size() < 3) return false;
    constexpr double tolerance = 1.0e-9;
    for (std::size_t i = 0; i < polygon.size(); ++i)
    {
        if (cross(polygon[i], polygon[(i + 1) % polygon.size()], point) < -tolerance)
            return false;
    }
    return true;
}
}  // namespace support_polygon
