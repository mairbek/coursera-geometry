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

  bool debug = false;

  std::set<int> binary;
  int count = 0;
  for (const auto& e : events) {
    if (e.type == 0) {
      // std::cout << "1" << std::endl;
      binary.insert(e.y1);
    } else if (e.type == 1) {
      // std::cout << "2" << std::endl;
      int k = 0;
      auto it = binary.lower_bound(e.y1);
      while(it != binary.end() && *it <= e.y2) {
        k++;
        it++;
      }
      // int k = binary.distance(y1, y2);
      count += k;
    } else {
      // std::cout << "erase " << e.y1 << std::endl;
      binary.erase(e.y1);
    }
  }

  std::cout << count << std::endl;

  return 0;
}
