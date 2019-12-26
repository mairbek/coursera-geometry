#include <iostream>

namespace {
/*
  clean implementation
  http://cs.colby.edu/courses/F13/cs231/LectureNotes/lec35/SkipList.java
*/

int rand_num(int n) {
  return std::rand() % n;
}

int rand_geometric() {

}

class Node {
 public:
  Node(int value, int levels) : value_(value), levels_(levels) {
    forward_ = new Node*[levels_+1];
    for (int i = 0; i <= levels_; i++) {
      forward_[i] = nullptr;
    }
  }

  void adjust_level(int newLevel) {
    // do nothing for now
  }

  int value() {
    return value_;
  }

 private:
  int value_;
  int levels_;
  Node** forward_;
};

class SkipList {
 public:
  SkipList() : level_(1), size_(0) {
    head_ = new Node(0, level_);
  }

  int size() { return size_; }

  void insert(int value) {
    int newLevel = rand_num()
  }

 private:
  int level_;
  int size_;
  Node* head_;
};

} // namespace

int main(int argc, const char** argv) {
  std::srand(time(nullptr));
  for (int i = 0; i < 50; i++) {
    int flip = std::rand() % 2;
    std::cout << "Hello skippy -- flip " << flip << std::endl;
  }

  return 0;
}
