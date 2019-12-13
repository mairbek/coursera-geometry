#include <cmath>
#include <tgmath.h>
#include <iostream>
#include <vector>

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

int search(double* nums, int size, double target) {
  int l = 0, r = size - 1, m = 0;
  while (l <= r) {
    m = (l + r) / 2;
    if (nums[m] == target) {
      return m;
    }
    // >= because might be one element
    if (nums[m] >= nums[l]) {
      if (nums[l] <= target && nums[m] > target) {
        r = m - 1;
        if (nums[r] < target) {
          return -r;
        }
      } else {
        l = m + 1;
      }
    } else {
      if (nums[m] < target && nums[r] >= target) {
        l = m + 1;
        if (nums[l] > target) {
          return -m;
        }
      } else {
        r = m - 1;
      }
    }
  }
  return -m;
}

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

void center_point(int* points, double* zx, double* zy) {
  int k = 0;
  int ax = points[k++], ay = points[k++];
  int bx = points[k++], by = points[k++];
  int cx = points[k++], cy = points[k++];

  *zx = ((double)(ax + bx + cx)) / 3;
  *zy = ((double)(ay + by + cy)) / 3;
}

double pseudoangle(double dx, double dy) {
  double p = dx/(std::abs(dx) + std::abs(dy));
  if (dy < 0) {
    return p - 1;
  }
  return 1 - p;
}


} // namespace

int main(int argc, const char** argv) {
  int m;
  std::cin >> m;

  int points[2 * m];

  for (int i = 0; i < m; i++) {
    std::cin >> points[2 * i] >> points[2 * i + 1];
  }

  double zx, zy;
  center_point(points, &zx, &zy);

  double alphas[m];

  for (int i = 0; i < m; i++) {
    int ax = points[2 * i], ay = points[2 * i + 1];
    alphas[i] = pseudoangle(ax - zx, ay - zy);
  }
  int n;

  std::cin >> n;

  for (int i = 0; i < n; i++) {
    int qx, qy;
    std::cin >> qx >> qy;

    auto alpha = pseudoangle(qx - zx, qy - zy);
    int idx = std::abs(search(alphas, m, alpha));
    int ax = points[2 * idx], ay = points[2 * idx + 1];
    int bx = points[(2 * idx + 2) % (2 * m)], by = points[(2 * idx + 3) % (2 * m)];
    auto dir = solve_point(ax, ay, bx, by, qx, qy);
    switch (dir) {
      case ResultPoint::ON_SEGMENT:
        std::cout << "BORDER" << std::endl;
        break;
      case ResultPoint::LEFT:
        std::cout << "INSIDE" << std::endl;
        break;
      case ResultPoint::RIGHT:
      case ResultPoint::ON_LINE:
          std::cout << "OUTSIDE" << std::endl;
        break;
    }
  }

  return 0;
}
