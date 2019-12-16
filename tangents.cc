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

Result solve(Point a, Point b, Point q) {
  return solve(a.first, a.second, b.first, b.second, q.first, q.second);
}

void tangent(PointVec& points, const Point& p, Point* vl, Point* vr) {
  int m = points.size();
  int ltan = 0, rtan = 0;

  Result dir_prev = solve(points[0], points[1], p);
  for (int i = 1; i < m; i++) {
    int j = (i+1) % m;

    Result dir_next = solve(points[i], points[j], p);

    bool eprev = dir_prev == Result::LEFT;
    bool enext = dir_next == Result::LEFT;

    if (!eprev && enext) {
      rtan = i;
    }
    if (eprev && !enext) {
      ltan = i;
    }
    dir_prev = dir_next;
  }

  *vl = points[ltan];
  *vr = points[rtan];
}

// Check that the sign of cross-product doesn't change.
} // namespace

int main(int argc, const char** argv) {
  int m;
  std::cin >> m;
  PointVec points;

  for (int i = 0; i < m; i++) {
    int64 x, y;
    std::cin >> x >> y;
    points.emplace_back(x, y);
  }

  int n;
  std::cin >> n;

  for (int i = 0; i < n; i++) {
    Point p;
    std::cin >> p.first >> p.second;

    Point vl, vr;

    tangent(points, p, &vl, &vr);

    std::cout << vl.first << " " << vl.second << " " << vr.first << " " << vr.second << std::endl;
  }

  return 0;
}
