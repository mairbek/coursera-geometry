#include <iostream>

namespace {
/*
  clean implementation
  http://cs.colby.edu/courses/F13/cs231/LectureNotes/lec35/SkipList.java
*/

int rand_num(int n) {
  return std::rand() % n;
}

int rand_geometric(int max_num) {
  int lev = 0;
  while (lev < max_num && rand_num(2) == 0) {
    lev++;
  }
  return lev;
}

class Node {
 public:
  Node(int value, int levels) : value_(value), levels_(levels) {
    forward_ = new Node*[levels_+1];
    for (int i = 0; i <= levels_; i++) {
      forward_[i] = nullptr;
    }
    counts_ = new int[levels_+1];
    for (int i = 0; i <= levels_; i++) {
      counts_[i] = 0;
    }
  }

  void print(int i) {
    std::cout << " value [" << value_ << ", [" << counts_[i] << "]]";
  }

  int value() {
    return value_;
  }

  void inc_count(int level) {
    counts_[level]++;
  }

  void dec_count(int level) {
    counts_[level]--;
  }


  int levels() { return levels_; }

  Node* forward(int level) {
    return forward_[level];
  }

  Node* next(int layer) {
    return forward_[layer];
  }

  void set_forward(int level, Node* ptr, int cnt) {
    forward_[level] = ptr;
    counts_[level] = cnt;
  }

  int get_count(int level) {
    return counts_[level];
  }

 private:
  int value_;
  int levels_;
  int* counts_;
  Node** forward_;
};

class SkipList {
 public:
  SkipList() : level_(1), size_(0) {
    head_ = new Node(-1234, level_);
  }

  int size() { return size_; }

  bool remove(int value) {
    Node** update = new Node*[level_ + 1];
    Node* ptr = head_;
    Node* nxt = nullptr;
    for (int i = level_; i >= 0; i--) {
      while(true) {
        nxt = ptr->forward(i);
        if (nxt == nullptr) {
          break;
        }
        if (value <= nxt->value()) {
          break;
        }
        ptr = nxt;
      }
      update[i] = ptr;
    }
    if (nxt == nullptr || nxt->value() != value) {
      return false;
    }
    bool rm = update[0]->get_count(0) == 1;
    for (int i = 0; i <= level_; i++) {
      if (rm && update[i]->forward(i) == nxt) {
        int count = update[i]->get_count(i);
        int nxt_count = nxt->get_count(i);
        update[i]->set_forward(i, nxt->forward(i), count + nxt_count - 1);
      } else {
        if (update[i]->forward(i) != nullptr) {
          update[i]->dec_count(i);
        }
      }
    }
    if (rm) {
      delete nxt;
    }
    size_--;
    return true;
  }

  // http://code.activestate.com/recipes/576930/
  void insert(int value) {
    int new_level = rand_geometric(size_);
    // std::cout << "adjust level from " << level_ << " to " << new_level << std::endl;
    if (new_level > level_) {
      auto new_head = new Node(-1234, new_level);
      for (int i = 0; i <= level_; i++) {
        new_head->set_forward(i, head_->forward(i), head_->get_count(i));
      }
      std::swap(head_, new_head);
      delete new_head;
      level_ = new_level;
    }
    Node** update = new Node*[level_ + 1];
    Node* ptr = head_;
    Node* nxt = nullptr;
    int* widths = new int[level_ + 1];
    for (int i = level_; i >= 0; i--) {
      widths[i] = 0;
      while(true) {
        nxt = ptr->forward(i);
        if (nxt == nullptr) {
          break;
        }
        if (value <= nxt->value()) {
          break;
        }
        widths[i] += ptr->get_count(i);
        ptr = nxt;
      }
      update[i] = ptr;
    }
    if (nxt != nullptr && nxt->value() == value) {
      new_level = -1;
    }
    Node* inserted = new Node(value, new_level);
    int w = 0;
    for (int i = 0; i <= new_level; i++) {
      int count = update[i]->get_count(i);
      inserted->set_forward(i, update[i]->forward(i), std::max(0, count - w));
      update[i]->set_forward(i, inserted, w+1);
      w += widths[i];
    }
    for (int i = new_level + 1; i <= level_; i++) {
      if (update[i]->forward(i) != nullptr) {
        update[i]->inc_count(i);
      }
    }
    size_++;
  }

  int count_range(int lower, int upper) {
    int lo_idx = index_of(lower, false);
    int up_idx = index_of(upper, true);
    if (lo_idx < 0) {
      lo_idx *= -1;
    }
    if (lo_idx > size_) {
      return 0;
    }
    if (up_idx < 0) {
      up_idx *= -1;
    }
    up_idx = std::min(up_idx, size_);
    return up_idx - lo_idx;
  }

  int index_of(int value, bool upper) {
    Node* pp = head_;
    int k = 0;
    int index = 0;
    for (int i = level_; i >= 0; i--) {
      Node* ptr = pp;
      while (ptr != nullptr && (ptr->value() <= value || ptr == head_)) {
        if (ptr->value() == value) {
          return upper ? index : index - pp->get_count(0);
        }
        pp = ptr;
        index += ptr->get_count(i);
        ptr = ptr->forward(i);
        k++;
      }
      index -= pp->get_count(i);
    }
    bool result = pp != nullptr && pp->value() == value;
    if (result) {
      return upper ? index : index - pp->get_count(0);
    }
    return -index;
  }

  void print_nodes() {
    for (int i = level_; i >= 0; i--) {
      std::cout << "level " << i << std::endl;
      Node* ptr = head_;
      while (ptr != nullptr) {
        ptr->print(i);
        ptr = ptr->next(i);
      }
      std::cout << std::endl;
    }
  }

 private:
  int level_;
  int size_;
  Node* head_;
};

} // namespace

int main(int argc, const char** argv) {
  std::srand(1112345545);
  std::cout << "Hello skippy" << std::endl;

  /*
  {
    SkipList sk;

    for (int i = 0; i < 10; i++) {
      sk.insert(3);
    }
    for (int i = 2; i <= 4; i++) {
      sk.insert(i);
    }
    sk.insert(10);
    std::cout << " Test 1" << std::endl;
    sk.print_nodes();
    std::cout << "   count 2, 3 = " << sk.count_range(2, 3) << std::endl;
    std::cout << "   count 3, 3 = " << sk.count_range(3, 3) << std::endl;
    std::cout << "   count -1, 11 = " << sk.count_range(-1, 11) << std::endl;
  }

  {
    SkipList sk;
    std::cout << " Test 2" << std::endl;
    sk.insert(10);
    sk.remove(10);
    sk.print_nodes();
    std::cout << "   count 2, 3 = " << sk.count_range(2, 3) << std::endl;
  }
*/
  {
    SkipList sk;
    std::cout << " Test 3" << std::endl;
    for (int i = 0; i < 6; i++) {
      sk.insert(i);
    }
    for (int i = 0; i < 4; i++) {
      sk.insert(i);
    }
    sk.remove(5);
    sk.print_nodes();
    std::cout << " index of -1 " << sk.index_of(-1, false) << std::endl;
    std::cout << " index of -1 " << sk.index_of(-1, true) << std::endl;
    std::cout << " count -1, -1 = " << sk.count_range(-1, -1) << std::endl;

    std::cout << " index of 0 " << sk.index_of(0, false) << std::endl;
    std::cout << " index of 0 " << sk.index_of(0, true) << std::endl;
    std::cout << " count 0, 0 = " << sk.count_range(0, 0) << std::endl;

    std::cout << " index of 4 " << sk.index_of(4, false) << std::endl;
    std::cout << " index of 4 " << sk.index_of(4, true) << std::endl;
    std::cout << " count 4, 4 = " << sk.count_range(4, 4) << std::endl;

    std::cout << " index of 5 " << sk.index_of(5, false) << std::endl;
    std::cout << " index of 5 " << sk.index_of(5, true) << std::endl;
    std::cout << " count 5, 5 = " << sk.count_range(5, 5) << std::endl;

/*
    std::cout << "   count 1, 10 = " << sk.count_range(1, 10) << std::endl;
    std::cout << "   count 1, 100 = " << sk.count_range(1, 100) << std::endl;
    std::cout << "   count 100, 100 = " << sk.count_range(100, 100) << std::endl;
    std::cout << "   count 101, 100 = " << sk.count_range(101, 100) << std::endl;
    std::cout << "   count 1000, 1000 = " << sk.count_range(1000, 1000) << std::endl;
*/
  }
  /*
  {
    auto v = sk.contains(11) ? "true" : "false";
    std::cout << " contains " << v << std::endl;
  }
  {
    auto v = sk.contains(0) ? "true" : "false";
    std::cout << " contains " << v << std::endl;
  }
  {
    auto v = sk.contains(4) ? "true" : "false";
    std::cout << " contains " << v << std::endl;
  }
  {
    auto v = sk.contains(1001) ? "true" : "false";
    std::cout << " contains " << v << std::endl;
  }
  {
    auto v = sk.contains(-1) ? "true" : "false";
    std::cout << " contains " << v << std::endl;
  }
  */
  return 0;
}
