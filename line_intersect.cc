#include <cmath>
#include <iostream>
#include <vector>
namespace {
using int64 = long long;
using Point = std::pair<int64, int64>;

enum class Intersection {
  POINT, LINE, EMPTY
};


bool on_segment(Point p1, Point p2, Point p) {
  int64 minx = std::min(p1.first, p2.first);
  int64 maxx = std::max(p1.first, p2.first);

  int64 miny = std::min(p1.second, p2.second);
  int64 maxy = std::max(p1.second, p2.second);

  int64 x = p.first;
  int64 y = p.second;

  return x >= minx && x <= maxx && y >= miny && y <= maxy;
}

bool share_point(Point p1, Point p2, Point p3, Point p4, Point *result) {
  if (p1 == p3 && !on_segment(p3, p4, p2) && !on_segment(p1, p2, p4)) {
    *result = p1;
    return true;
  }
  if (p2 == p3 && !on_segment(p3, p4, p1) && !on_segment(p1, p2, p4)) {
    *result = p2;
    return true;
  }
  if (p1 == p4 && !on_segment(p3, p4, p2) && !on_segment(p1, p2, p3)) {
    *result = p1;
    return true;
  }
  if (p2 == p4 && !on_segment(p3, p4, p1) && !on_segment(p1, p2, p3)) {
    *result = p2;
    return true;
  }
  return false;
}

Intersection solve(Point p11, Point p12, Point p21, Point p22, Point* result, bool debug) {
  if (debug) {
    std::cout << p11.first << " " << p11.second << std::endl
              << p12.first << " " << p12.second << std::endl
              << p21.first << " " << p21.second << std::endl
              << p22.first << " " << p22.second << std::endl
              << std::endl;
  }
  int64 a1 = p12.second - p11.second;
  int64 b1 = p11.first - p12.first;
  int64 c1 = a1 * p11.first + b1 * p11.second;

  if (debug) {
    std::cout << a1 << " " << b1 << " " << c1 << std::endl;
  }

  int64 a2 = p22.second - p21.second;
  int64 b2 = p21.first - p22.first;
  int64 c2 = a2 * p21.first + b2 * p21.second;

  if (debug) {
    std::cout << a2 << " " << b2 << " " << c2 << std::endl;
  }

  int64 det = a1 * b2 - a2 * b1;

  if (debug) {
    std::cout << "det" << det << std::endl;
  }

  if (det == 0) {
    if (share_point(p11, p12, p21, p22, result)) {
      return Intersection::POINT;
    }
    if (on_segment(p11, p12, p21) || on_segment(p11, p12, p22)) {
      return Intersection::LINE;
    }
    if (on_segment(p21, p22, p11) || on_segment(p21, p22, p12)) {
      return Intersection::LINE;
    }
    return Intersection::EMPTY;
  }

  result->first = (b2 * c1 - b1 * c2) / det;
  result->second = (a1 * c2 - a2 * c1) / det;

  if (debug) {
    std::cout << "result " << result->first << ", " << result->second << std::endl;
  }

  if (on_segment(p11, p12, *result) && on_segment(p21, p22, *result)) {
    return Intersection::POINT;
  }
  return Intersection::EMPTY;
}


} // namespace

int main(int argc, const char** argv) {
  Point p11, p12;
  std::cin >> p11.first >> p11.second >> p12.first >> p12.second;

  Point p21, p22;
  std::cin >> p21.first >> p21.second >> p22.first >> p22.second;

  Point result;

  Intersection is = solve(p11, p12, p21, p22, &result, false);

  switch(is) {
    case Intersection::POINT:
      std::cout << "The intersection point is (" << result.first << ", " << result.second << ")." << std::endl;
      break;
    case Intersection::LINE:
      std::cout << "A common segment of non-zero length." << std::endl;
      break;
    case Intersection::EMPTY:
      std::cout << "No common points." << std::endl;
      break;
  }

  return 0;
}
