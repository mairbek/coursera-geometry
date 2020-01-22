#include <iostream>
#include <set>
#include <vector>

namespace {
// hope this works
using int64 = int;

using Point = std::pair<int, int>;

struct Segment {
  Segment(Point first, Point second, float slope, int y) : first(first), second(second), slope(slope), y(y) {}
  Point first;
  Point second;

  float slope;
  float y;
};

enum class Intersection {
  POINT, LINE, EMPTY
};


bool on_segment(Point p1, Point p2, Point p) {
  int64 minx = std::min(p1.first, p2.first);
  int64 maxx = std::max(p1.first, p2.first);

  int64 miny = std::min(p1.second, p2.second);
  int64 maxy = std::max(p1.second, p2.second);

  int64 x = p.first;
  int64 y = p.second;

  return x >= minx && x <= maxx && y >= miny && y <= maxy;
}

bool on_segment(Point p1, Point p2, float x, float y) {
  int64 minx = std::min(p1.first, p2.first);
  int64 maxx = std::max(p1.first, p2.first);

  int64 miny = std::min(p1.second, p2.second);
  int64 maxy = std::max(p1.second, p2.second);

  return x >= minx && x <= maxx && y >= miny && y <= maxy;
}


bool share_point(Point p1, Point p2, Point p3, Point p4, float* resultx, float* resulty) {
  if (p1 == p3 && !on_segment(p3, p4, p2) && !on_segment(p1, p2, p4)) {
    *resultx = p1.first;
    *resulty = p1.second;
    return true;
  }
  if (p2 == p3 && !on_segment(p3, p4, p1) && !on_segment(p1, p2, p4)) {
    *resultx = p2.first;
    *resulty = p2.second;
    return true;
  }
  if (p1 == p4 && !on_segment(p3, p4, p2) && !on_segment(p1, p2, p3)) {
    *resultx = p1.first;
    *resulty = p1.second;
    return true;
  }
  if (p2 == p4 && !on_segment(p3, p4, p1) && !on_segment(p1, p2, p3)) {
    *resultx = p2.first;
    *resulty = p2.second;
    return true;
  }
  return false;
}

Intersection find_x(Point p11, Point p12, Point p21, Point p22, float* resultx, float* resulty, bool debug) {
  if (debug) {
    std::cout << p11.first << " " << p11.second << std::endl
              << p12.first << " " << p12.second << std::endl
              << p21.first << " " << p21.second << std::endl
              << p22.first << " " << p22.second << std::endl
              << std::endl;
  }
  int64 a1 = p12.second - p11.second;
  int64 b1 = p11.first - p12.first;
  int64 c1 = a1 * p11.first + b1 * p11.second;

  if (debug) {
    std::cout << a1 << " " << b1 << " " << c1 << std::endl;
  }

  int64 a2 = p22.second - p21.second;
  int64 b2 = p21.first - p22.first;
  int64 c2 = a2 * p21.first + b2 * p21.second;

  if (debug) {
    std::cout << a2 << " " << b2 << " " << c2 << std::endl;
  }

  int64 det = a1 * b2 - a2 * b1;

  if (debug) {
    std::cout << "det " << det << std::endl;
  }

  if (det == 0) {
    if (share_point(p11, p12, p21, p22, resultx, resulty)) {
      return Intersection::POINT;
    }
    if (on_segment(p11, p12, p21) || on_segment(p11, p12, p22)) {
      return Intersection::LINE;
    }
    if (on_segment(p21, p22, p11) || on_segment(p21, p22, p12)) {
      return Intersection::LINE;
    }
    return Intersection::EMPTY;
  }

  *resultx = float(b2 * c1 - b1 * c2) / det;
  *resulty = float(a1 * c2 - a2 * c1) / det;

  if (debug) {
    std::cout << "result ff " << *resultx << ", " << *resulty << std::endl;
  }

  if (on_segment(p11, p12, *resultx, *resulty) && on_segment(p21, p22, *resultx, *resulty)) {
    return Intersection::POINT;
  }
  return Intersection::EMPTY;
}


Intersection find_xx(Segment* seg1, Segment* seg2, float* resultx, float* resulty, bool debug) {
  return find_x(seg1->first, seg1->second, seg2->first, seg2->second, resultx, resulty, debug);
}


struct Event {
  float x;
  float y;
  int type; // 0 intersection, 1 upper, 2 lower
  Segment* seg;
};

struct Status {
  float y;
  float slope;
  Segment* seg;
};

} // namespace

bool event_comparator(const Event& a, const Event& b) {
  if (a.x == b.x) {
    if (a.type == b.type) {
      return a.y > b.y;
    }
    return a.type < b.type;
  }
  return a.x < b.x;
}

bool status_comparator(const Status& a, const Status& b) {
  if (a.y == b.y) {
    return a.slope > b.slope;
  }
  return a.y > b.y;
}

using StatusSet = std::set<Status, bool(*) (const Status& a, const Status& b)>;

class Solver {
 public:
  Solver() : events_(&event_comparator), statuses_(&status_comparator), debug_(false) {
  }

  void add_segment(Point p, Point q) {
    if (p.first > q.first) {
      std::swap(p, q);
    }
    // Leak, whatever
    Segment* seg = new Segment(p, q, -1, -1000);
    seg->y = p.second;
    seg->slope = float(q.second - p.second) / float(q.first - p.first);

    if (debug_) {
      std::cout << "init: added segment "
                << "[" << seg->first.first << " " << seg->first.second << "] "
                << "[" << seg->second.first << " " << seg->second.second << "] "
                << " y = " << seg->y << " slope = " << seg->slope << std::endl;
    }

    Event left;
    left.type = 1;
    left.x = p.first;
    left.y = p.second;
    left.seg = seg;
    events_.insert(left);

    Event right;
    right.type = 2;
    right.x = q.first;
    right.y = q.second;
    right.seg = seg;
    events_.insert(right);
  }

  void solve() {
    while (events_.size() > 0) {
      auto it = events_.begin();

      if (debug_) {
        std::cout << " [event] "<< it->type
                  << " [" << it->seg->first.first << " " << it->seg->first.second << "] "
                  << " [" << it->seg->second.first << " " << it->seg->second.second << "] "
                  << " (" << it->x << " " << it->y << ") "
                  << std::endl;
      }
      if (it->type == 1) {
        if (debug_) {
          std::cout << " [++] inserting " << std::endl;
        }

        Status st;
        st.seg = it->seg;
        st.y = st.seg->y;
        st.slope = st.seg->slope;

        auto p = statuses_.insert(st);

        StatusSet::iterator inserted_it = p.first;
        check_up_xx(inserted_it, it->x);
        check_bottom_xx(inserted_it, it->x);

      }

      if (it->type == 0) {
        if (debug_) {
          std::cout << " [xx] intersection" << std::endl;
        }
        Status st;
        st.seg = it->seg;
        st.y = st.seg->y;
        st.slope = st.seg->slope;

        auto find_it = statuses_.find(st);

        if (find_it == statuses_.end()) {
          std::cout << " xxx bullshit " << std::endl;
        }

        /*
        std::cout << "before swap" << std::endl;
        print_statuses();
        */
        auto nxt = std::next(find_it);
        results_.emplace_back(std::make_pair(find_it->seg, nxt->seg));

        Segment* top = find_it->seg;
        Segment* bottom = nxt->seg;

        // remove
        statuses_.erase(find_it);
        statuses_.erase(nxt);

        top->y = it->y;
        bottom->y = it->y;

        // swap

        {
          Status st;
          st.seg = bottom;
          st.y = bottom->y;
          st.slope = bottom->slope;
          auto p = statuses_.insert(st);
          auto inserted_it = p.first;
          check_up_xx(inserted_it, it->x);
        }
        {
          Status st;
          st.seg = top;
          st.y = top->y;
          st.slope = top->slope;
          auto p = statuses_.insert(st);
          auto inserted_it = p.first;
          check_bottom_xx(inserted_it, it->x);
        }
        /*
        std::cout << "after swap" << std::endl;
        print_statuses();
        */
      }

      if (it->type == 2) {
        if (debug_) {
          std::cout << " [--] removing " << std::endl;
        }

        Status st;
        st.seg = it->seg;
        st.y = st.seg->y;
        st.slope = st.seg->slope;

        auto find_it = statuses_.find(st);
        if (find_it == statuses_.end()) {
          std::cout << "crash!" << std::endl;
        }

        auto remove_it = statuses_.erase(find_it);
        check_up_xx(remove_it, it->x);
      }

      events_.erase(it);
    }
  }

  void print_statuses() {

    std::cout << "!!!!!" << std::endl;
    for (auto& st : statuses_) {
      std::cout << " [" << st.seg->first.first << " " << st.seg->first.second << "] "
                << " [" << st.seg->second.first << " " << st.seg->second.second << "] "<< std::endl;
    }

  }

  void check_up_xx(StatusSet::iterator it, float xline) {
    if (debug_) {
      std::cout << " [xx++] checking up intersection" << std::endl;
    }
    if (it == statuses_.begin()) {
      if (debug_) {
        std::cout << " [xx++] first element skipping " << std::endl;
      }
      return;
    }
    auto prev = std::prev(it);
    float xpx, xpy;
    auto result = find_xx(prev->seg, it->seg, &xpx, &xpy, false);
    if (result != Intersection::POINT) {
      if (debug_) {
        std::cout << " [xx++] no intersections" << std::endl;
      }
      return;
    }
    if (xpx < xline) {
      if (debug_) {
        std::cout << " [xx++] intersection behind" << std::endl;
      }
      return;
    }
    if (debug_) {
      std::cout << " [xx++] inserting intersection "
                << " [" << prev->seg->first.first << " " << prev->seg->first.second << "] "
                << " [" << prev->seg->second.first << " " << prev->seg->second.second << "] "
                << " [" << it->seg->first.first << " " << it->seg->first.second << "] "
                << " [" << it->seg->second.first << " " << it->seg->second.second << "] "
                << "[" << xpx << " " << xpy << "]" << std::endl;
    }
    Event xevent;
    xevent.x = xpx;
    xevent.y = xpy;
    xevent.seg = prev->seg;
    xevent.type = 0;
    events_.insert(xevent);
  }

  void check_bottom_xx(StatusSet::iterator it, int xline) {
    if (debug_) {
      std::cout << " [xx++] checking bottom intersection" << std::endl;
    }
    auto next = std::next(it);
    if (next == statuses_.end()) {
      if (debug_) {
        std::cout << " [xx++] last element skipping " << std::endl;
      }
      return;
    }
    float xpx, xpy;
    auto result = find_xx(it->seg, next->seg, &xpx, &xpy, false);
    if (result != Intersection::POINT) {
      if (debug_) {
        std::cout << " [xx++] no intersections" << std::endl;
      }
      return;
    }
    if (xpx < xline) {
      if (debug_) {
        std::cout << " [xx++] intersection behind" << std::endl;
      }
      return;
    }
    if (debug_) {
      std::cout << " [xx++] inserting intersection "
                << " [" << it->seg->first.first << " " << it->seg->first.second << "] "
                << " [" << it->seg->second.first << " " << it->seg->second.second << "] "
                << " [" << next->seg->first.first << " " << next->seg->first.second << "] "
                << " [" << next->seg->second.first << " " << next->seg->second.second << "] "
                << " [" << xpx << " " << xpy << "]" << std::endl;
    }
    Event xevent;
    xevent.x = xpx;
    xevent.y = xpy;
    xevent.seg = it->seg;
    xevent.type = 0;
    events_.insert(xevent);
  }

  void print_results() {
    std::cout << results_.size() << std::endl;

    for (auto p : results_) {
      Segment* a = p.first;
      Segment* b = p.second;
      std::cout << a->first.first << " " << a->first.second << " " << a->second.first << " " << a->second.second << " ";
      std::cout << b->first.first << " " << b->first.second << " " << b->second.first << " " << b->second.second;
      std::cout << std::endl;
    }
  }

 private:
  std::set<Event, bool(*) (const Event& a, const Event& b)> events_;
  StatusSet statuses_;
  std::vector<std::pair<Segment*, Segment*>> results_;
  bool debug_;
};

int main(int argc, const char** argv) {
  int n;
  std::cin >> n;

  Solver sol;

  for (int i = 0; i < n; i++) {
    Point p, q;
    std::cin >> p.first >> p.second >> q.first >> q.second;
    sol.add_segment(p, q);
  }

  sol.solve();

  sol.print_results();

  return 0;
}
