#include <vector>
#include <iostream>

namespace {
  class Solution {
  public:
    int search(std::vector<int>& nums, int target) {
      int l = 0, r = nums.size() - 1;
      while (l <= r) {
        int m = (l + r) / 2;
        if (nums[m] == target) {
          return m;
        }
        // >= because might be one element
        if (nums[m] >= nums[l]) {
          if (nums[l] <= target && nums[m] > target) {
            r = m - 1;
          } else {
            l = m + 1;
          }
        } else {
          if (nums[m] < target && nums[r] >= target) {
            l = m + 1;
          } else {
            r = m - 1;
          }
        }
      }
      std::cout << l << " " << r << std::endl;
      return -r;
    }
  };
}

int main(int argc, const char** argv) {
  std::cout << "Init" << std::endl;
  Solution s;

  std::vector<int> nums;
  nums = {4,5,6,7,0,1,2};
  std::cout << s.search(nums, 0) << std::endl;

  nums = {3, 1};
  std::cout << s.search(nums, 1) << std::endl;

  nums = {5, 1, 3};
  std::cout << s.search(nums, 5) << std::endl;

  nums = {5, 1, 3};
  std::cout << s.search(nums, 6) << std::endl;

  nums = {5, 1, 3};
  std::cout << s.search(nums, 2) << std::endl;


  return 0;
}
