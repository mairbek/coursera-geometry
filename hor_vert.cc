#include <iostream>
#include <algorithm>
#include <set>
#include <list>
#include <map>

namespace {
using Point = std::pair<int, int>;

struct Event {
  int x;
  int type;
  int y1, y2;
};
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
  int n;
  std::cin >> n;

  auto cmp = [](Event a, Event b) { if (a.x == b.x) {
      return a.type < b.type;
    }
    return a.x < b.x;
    };

  std::multiset<Event, decltype(cmp)> events(cmp);


  for (int i = 0; i < n; i++) {
    Point p, q;

    std::cin >> p.first >> p.second >> q.first >> q.second;

    if (p.first == q.first) {
      Event e;
      e.x = p.first;
      if (p.second <= q.second) {
        e.y1 = p.second;
        e.y2 = q.second;
      } else {
        e.y1 = q.second;
        e.y2 = p.second;
      }
      e.type = 1;
      events.insert(e);
    } else {
      if (q.first < p.first) {
        std::swap(p, q);
      }
      Event begin;
      begin.type = 0;
      begin.x = p.first;
      begin.y1 = p.second;
      events.insert(begin);
      Event end;
      end.type = 2;
      end.x = q.first;
      end.y1 = q.second;
      events.insert(end);
    }
  }

  SkipList sk;
  int count = 0;
  for (const auto& e : events) {
    if (e.type == 0) {
      sk.insert(e.y1);
    } else if (e.type == 1) {
      count += sk.count_range(e.y1, e.y2);
    } else {
      sk.remove(e.y1);
    }
  }

  std::cout << count << std::endl;

  return 0;
}
