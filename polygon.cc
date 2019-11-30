#include <iostream>

namespace {
enum class ResultPoint {
  ON_SEGMENT, ON_LINE, LEFT, RIGHT
};

/*
void print_result_point(ResultPoint result) {
  switch(result) {
    case ResultPoint::ON_SEGMENT:
      std::cout << "ON_SEGMENT" << std::endl;
      break;
    case ResultPoint::ON_LINE:
      std::cout << "ON_LINE" << std::endl;
      break;
    case ResultPoint::LEFT:
      std::cout << "LEFT" << std::endl;
      break;
    case ResultPoint::RIGHT:
      std::cout << "RIGHT" << std::endl;
      break;

  }
}
*/

ResultPoint solve_point(int ax, int ay, int bx, int by, int qx, int qy) {
  int det = ax * (by - qy) - ay * (bx - qx) + (bx * qy - by * qx);
  if (det > 0) {
    return ResultPoint::LEFT;
  }
  if (det < 0) {
    return ResultPoint::RIGHT;
  }
  if (ax >= bx && qx >= bx && qx <= ax) {
    return ResultPoint::ON_SEGMENT;
  }
  if (qx >= ax && qx <= bx) {
    return ResultPoint::ON_SEGMENT;
  }
  return ResultPoint::ON_LINE;
}

enum class ResultPoly {BORDER, INSIDE, OUTSIDE};

ResultPoly solve_poly(
    int m,
    int* points,
    int qx,
    int qy) {

  int num_crossed = 0;

  for (int i = 0; i < m; i++) {
    int ax = points[2 * i], ay = points[2 * i + 1];
    int bx = points[(2 * i + 2) % (2 * m)], by = points[(2 * i + 3) % (2 * m)];
    if (ay == by) {
      // std::cout << "horizontal" << std::endl;
      continue;
    }

    // b is the upper point.
    if (ay > by) {
      std::swap(ax, bx);
      std::swap(ay, by);
    }

    // std::cout << ax << " " << ay << " " << bx << " " << by << " " << qx << " " << qy << std::endl;
    auto dir = solve_point(ax, ay, bx, by, qx, qy);
    // print_result_point(dir);
    if (dir == ResultPoint::ON_SEGMENT) {
      return ResultPoly::BORDER;
    }
    if (qy >= ay && qy < by && dir == ResultPoint::RIGHT) {
      num_crossed++;
    }
  }

  // std::cout << "num crossed " << num_crossed << std::endl;
  if (num_crossed % 2 == 0) {
    return ResultPoly::OUTSIDE;
  }
  return ResultPoly::INSIDE;
}

} // namespace

int main(int argc, const char** argv) {
  int m;
  std::cin >> m;

  int points[2 * m];

  for (int i = 0; i < m; i++) {
    std::cin >> points[2 * i] >> points[2 * i + 1];
  }
  int n;

  std::cin >> n;

  for (int i = 0; i < n; i++) {
    int qx, qy;
    std::cin >> qx >> qy;

    auto result = solve_poly(m, points, qx, qy);
    switch (result) {
      case ResultPoly::BORDER:
        std::cout << "BORDER" << std::endl;
        break;
      case ResultPoly::INSIDE:
        std::cout << "INSIDE" << std::endl;
        break;
      case ResultPoly::OUTSIDE:
        std::cout << "OUTSIDE" << std::endl;
        break;
    }
  }

  return 0;
}
