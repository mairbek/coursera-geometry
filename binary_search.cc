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
        if (nums[m] > target) {
          r = m - 1;
        } else {
          l = m + 1;
        }
      }
      return -1;
    }
  };
}

int main(int argc, const char** argv) {
  std::cout << "Whatever bro" << std::endl;

  Solution s;

  std::vector<int> nums;
  nums = {-1,0,3,5,9,12};
  std::cout << s.search(nums, 9) << std::endl;

  nums = {-1,0,3,5,9,12,13};
  std::cout << s.search(nums, 13) << std::endl;

  nums = {-1,0,3,5,9,12,13};
  std::cout << s.search(nums, 7) << std::endl;

  nums = {};
  std::cout << s.search(nums, 7) << std::endl;

  nums = {1};
  std::cout << s.search(nums, 7) << std::endl;

  nums = {1, 2};
  std::cout << s.search(nums, 2) << std::endl;

  nums = {1, 2, 3};
  std::cout << s.search(nums, 2) << std::endl;

  return 0;
}
