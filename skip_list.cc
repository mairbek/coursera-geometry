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
      int count = update[i]->get_count(i);
      if (rm && update[i]->forward(i) == nxt) {
        update[i]->set_forward(i, nxt->forward(i), count);
      } else {
        update[i]->dec_count(i);
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
      update[i]->inc_count(i);
    }
    size_++;
  }

  int index_of(int value) {
    Node* pp = head_;
    int k = 0;
    int index = 0;
    for (int i = level_; i >= 0; i--) {
      Node* ptr = pp;
      while (ptr != nullptr && (ptr->value() <= value || ptr == head_)) {
        // std::cout << "[contains check] level " << i << " [" << k << "] " << ptr->value() << " count " << ptr->get_count(i) << std::endl;
        if (ptr->value() == value) {
          return index;
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
      return index;
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

  SkipList sk;

  /*
  for (int i = 1; i <= 5; i++) {
    sk.insert(-i);
    sk.insert(i);
  }
  */
  for (int i = 0; i < 10; i++) {
    sk.remove(0);
    sk.insert(0);
  }
  for (int i = 1; i <= 1000; i++) {
    sk.insert(i);
    sk.insert(-i);
  }
  for (int i = 0; i < 10; i++) {
    sk.remove(-i);
  }
  /*
  for (int i = 1; i <= 4; i++) {
    sk.insert(-i);
    sk.insert(i);
    }*/
  {
    std::cout << " index of " << -3 << " " << sk.index_of(-3) << std::endl;
    std::cout << " index of " << 3 << " " << sk.index_of(3) << std::endl;
    std::cout << " index of " << 4 << " " << sk.index_of(4) << std::endl;
    std::cout << " index of " << 5 << " " << sk.index_of(5) << std::endl;
    std::cout << " index of " << -500 << " " << sk.index_of(-500) << std::endl;
    std::cout << " index of " << 1001 << " " << sk.index_of(1001) << std::endl;
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
