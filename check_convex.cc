#include <cmath>
#include <iostream>
#include <vector>

namespace {
// Check that the sign of cross-product doesn't change.
bool check_convex(int* points, int n) {
  bool sign = false;
  for (int i = 0; i < n; i++) {
    int ax = points[2 * i], ay = points[2 * i + 1];
    int bx = points[(2 * i + 2) % (2 * n)], by = points[(2 * i + 3) % (2 * n)];
    int cx = points[(2 * i + 4) % (2 * n)], cy = points[(2 * i + 5) % (2 * n)];

    int bax = bx - ax;
    int bay = by - ay;
    int cbx = cx - bx;
    int cby = cy - by;
    int crossproduct = bax * cby - bay * cbx;
    /*std::cout
      << "\t" << ax << " " << ay << std::endl
      << "\t" << bx << " " << by << std::endl
      << "\t" << cx << " " << cy << std::endl
      << "\t" << crossproduct << std::endl;
    */
    if (i == 0) {
      sign = crossproduct >= 0;
    } else {
      if (sign != (crossproduct >= 0)) {
        return false;
      }
    }
  }
  // std::cout << "here" << std::endl;
  return true;
}

} // namespace

int main(int argc, const char** argv) {
  int n;
  std::cin >> n;
  int points[2 * n];

  for (int i = 0; i < n; i++) {
    std::cin >> points[2 * i] >> points[2 * i + 1];
  }

  if (check_convex(points, n)) {
    std::cout << "CONVEX" << std::endl;
  } else {
    std::cout << "NOT_CONVEX" << std::endl;
  }

  return 0;
}
