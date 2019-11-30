#include <iostream>

namespace {
enum class Result {
  ON_SEGMENT, ON_LINE, LEFT, RIGHT
};

Result solve(int ax, int ay, int bx, int by, int qx, int qy) {
  int det = ax * (by - qy) - ay * (bx - qx) + (bx * qy - by * qx);
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
} // namespace

int main(int argc, const char** argv) {
  int ax, ay, bx, by;

  std::cin >> ax >> ay >> bx >> by;

  int n;

  std::cin >> n;

  for (int i = 0; i < n; i++) {
    int qx, qy;
    std::cin >> qx >> qy;

    Result result = solve(ax, ay, bx, by, qx, qy);

    switch(result) {
      case Result::ON_SEGMENT:
        std::cout << "ON_SEGMENT" << std::endl;
        break;
      case Result::ON_LINE:
        std::cout << "ON_LINE" << std::endl;
        break;
      case Result::LEFT:
        std::cout << "LEFT" << std::endl;
        break;
      case Result::RIGHT:
        std::cout << "RIGHT" << std::endl;
        break;

    }
  }
  return 0;
}
