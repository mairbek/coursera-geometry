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

ResultPoly solve_triangle(
    int ax,
    int ay,
    int bx,
    int by,
    int cx,
    int cy,
    int qx,
    int qy) {
  {
    auto dir = solve_point(ax, ay, bx, by, qx, qy);
    if (dir == ResultPoint::ON_SEGMENT) {
      return ResultPoly::BORDER;
    }
    if (dir != ResultPoint::LEFT) {
      return ResultPoly::OUTSIDE;
    }
  }
  {
    auto dir = solve_point(bx, by, cx, cy, qx, qy);
    if (dir == ResultPoint::ON_SEGMENT) {
      return ResultPoly::BORDER;
    }
    if (dir != ResultPoint::LEFT) {
      return ResultPoly::OUTSIDE;
    }
  }
  {
    auto dir = solve_point(cx, cy, ax, ay, qx, qy);
    if (dir == ResultPoint::ON_SEGMENT) {
      return ResultPoly::BORDER;
    }
    if (dir != ResultPoint::LEFT) {
      return ResultPoly::OUTSIDE;
    }
  }

  return ResultPoly::INSIDE;
}

} // namespace

int main(int argc, const char** argv) {
  int ax, ay, bx, by, cx, cy;

  std::cin >> ax >> ay >> bx >> by >> cx >> cy;

  auto dir = solve_point(ax, ay, bx, by, cx, cy);

  if (dir == ResultPoint::RIGHT) {
    std::swap(bx, cx);
    std::swap(by, cy);
  }

  int n;

  std::cin >> n;

  for (int i = 0; i < n; i++) {
    int qx, qy;
    std::cin >> qx >> qy;

    auto result = solve_triangle(ax, ay, bx, by, cx, cy, qx, qy);

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
