#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

namespace {
using int64 = long long;
using Point = std::pair<int64, int64>;
using PointVec = std::vector<Point>;

enum class Result {
  ON_SEGMENT, ON_LINE, LEFT, RIGHT
};

Result solve(int64 ax, int64 ay, int64 bx, int64 by, int64 qx, int64 qy) {
  int64 det = ax * (by - qy) - ay * (bx - qx) + (bx * qy - by * qx);
  if (det > 0) {
    return Result::LEFT;
  }
  if (det < 0) {
    return Result::RIGHT;
  }
  if (ax >= bx && qx >= bx && qx <= ax) {
    return Result::ON_SEGMENT;
  }
  if (qx >= ax && qx <= bx) {
    return Result::ON_SEGMENT;
  }
  return Result::ON_LINE;
}

  PointVec build_hull(const PointVec& points) {
    PointVec upper_hull;
  for (size_t i = 0; i < points.size(); i++) {
    upper_hull.push_back(points[i]);
    while(upper_hull.size() > 2) {

      Point c = upper_hull.back();
      upper_hull.pop_back();
      Point b = upper_hull.back();
      upper_hull.pop_back();
      Point a = upper_hull.back();
      upper_hull.pop_back();

      Result dir = solve(a.first, a.second, b.first, b.second, c.first, c.second);
      if (dir == Result::RIGHT) {
        upper_hull.push_back(a);
        upper_hull.push_back(b);
        upper_hull.push_back(c);
        break;
      } else {
        upper_hull.push_back(a);
        upper_hull.push_back(c);
      }
    }
  }
  PointVec lower_hull;
  for (size_t i = 0; i < points.size(); i++) {
    lower_hull.push_back(points[i]);
    if (lower_hull.size() < 3) {
      continue;
    }

    while(lower_hull.size() > 2) {
      Point c = lower_hull.back();
      lower_hull.pop_back();
      Point b = lower_hull.back();
      lower_hull.pop_back();
      Point a = lower_hull.back();
      lower_hull.pop_back();

      Result dir = solve(a.first, a.second, b.first, b.second, c.first, c.second);
      if (dir == Result::LEFT) {
        lower_hull.push_back(a);
        lower_hull.push_back(b);
        lower_hull.push_back(c);
        break;
      } else {
        lower_hull.push_back(a);
        lower_hull.push_back(c);
      }
    }
  }
  PointVec hull;
  for (size_t i = 1; i < lower_hull.size(); i++) {
    hull.push_back(lower_hull[i]);
  }
  for (int i = upper_hull.size() - 2; i >= 0 ; i--) {
    hull.push_back(upper_hull[i]);
  }
  return hull;
}

} // namespace

int main(int argc, const char** argv) {
  int n;
  std::cin >> n;
  PointVec points;

  for (int i = 0; i < n; i++) {
    int64 x, y;
    std::cin >> x >> y;
    points.emplace_back(x, y);
  }

  std::sort(points.begin(), points.end(), [](Point a, Point b) {
    return a.first == b.first ? a.second < b.second : a.first < b.first;
  });

  PointVec hull = build_hull(points);

  std::cout << hull.size() << std::endl;
  for (const auto& point : hull) {
    std::cout << point.first << " " << point.second << " ";
  }
  std::cout << std::endl;

  return 0;
}
