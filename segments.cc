#include <iostream>
#include <set>
#include <vector>

namespace {
// hope this works
using int64 = int;

using Point = std::pair<int, int>;

struct Segment {
  Segment(Point first, Point second, float id, int x) : first(first), second(second), id(id), x(x) {}
  Point first;
  Point second;

  int id;
  int x;
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

bool share_point(Point p1, Point p2, Point p3, Point p4, Point *result) {
  if (p1 == p3 && !on_segment(p3, p4, p2) && !on_segment(p1, p2, p4)) {
    *result = p1;
    return true;
  }
  if (p2 == p3 && !on_segment(p3, p4, p1) && !on_segment(p1, p2, p4)) {
    *result = p2;
    return true;
  }
  if (p1 == p4 && !on_segment(p3, p4, p2) && !on_segment(p1, p2, p3)) {
    *result = p1;
    return true;
  }
  if (p2 == p4 && !on_segment(p3, p4, p1) && !on_segment(p1, p2, p3)) {
    *result = p2;
    return true;
  }
  return false;
}

Intersection find_x(Point p11, Point p12, Point p21, Point p22, Point* result, bool debug) {
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
    std::cout << "det" << det << std::endl;
  }

  if (det == 0) {
    if (share_point(p11, p12, p21, p22, result)) {
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

  result->first = (b2 * c1 - b1 * c2) / det;
  result->second = (a1 * c2 - a2 * c1) / det;

  if (debug) {
    std::cout << "result " << result->first << ", " << result->second << std::endl;
  }

  if (on_segment(p11, p12, *result) && on_segment(p21, p22, *result)) {
    return Intersection::POINT;
  }
  return Intersection::EMPTY;
}


Intersection find_xx(Segment* seg1, Segment* seg2, Point* result, bool debug) {
  return find_x(seg1->first, seg1->second, seg2->first, seg2->second, result, debug);
}


struct Event {
  int y;
  int type; // 0 intersection, 1 upper, 2 lower
  Segment* seg;

  // For intersections.
  Point p;
};

struct Status {
  int x;
  float id;
  Segment* seg;
};

} // namespace

int main(int argc, const char** argv) {
  int n;
  std::cin >> n;

  auto cmp = [](Event a, Event b) { if (a.y == b.y) {
      return a.type < b.type;
    }
    return a.y > b.y;
  };
  std::multiset<Event, decltype(cmp)> events(cmp);

  for (int i = 0; i < n; i++) {
    Point p, q;
    std::cin >> p.first >> p.second >> q.first >> q.second;

    if (p.second < q.second) {
      std::swap(p, q);
    }

    // leak memory, whatever
    Segment* seg = new Segment(p, q, -1, -1000);

    seg->x = p.first;

    Event ue;
    ue.type = 1;
    ue.y = p.second;
    ue.seg = seg;

    Event le;
    le.type = 2;
    le.y = q.second;
    le.seg = seg;

    events.insert(ue);
    events.insert(le);
  }

  auto seg_cmp = [](Status a, Status b) {
    if (a.x == b.x) {
      if (a.id < b.id) {
        return a.id < b.id;
      }
      if (a.seg == b.seg) {
        return false;
      }
      return a.seg < b.seg;
    }
    return a.x < b.x;
  };

  std::set<Status, decltype(seg_cmp)> statuses(seg_cmp);

  int current_id = 0;

  std::vector<std::pair<Segment*, Segment*>> results;

  while (events.size() > 0) {
    auto it = events.begin();

    std::cout << it->type
              << " [" << it->seg->first.first << " " << it->seg->first.second << "] "
              << " [" << it->seg->second.first << " " << it->seg->second.second << "] "
              << std::endl;


    if (it->type == 1) {
      current_id++;

      std::cout << " insert !! " << std::endl;

      Status st;
      st.seg = it->seg;
      st.seg->id = current_id;
      st.x = st.seg->x;
      st.id = st.seg->id;

      auto p = statuses.insert(st);

      if (!p.second) {
        std::cout << "WTF" << std::endl;
      }

      auto inserted_it = p.first;

      if (inserted_it == statuses.begin()) {
        std::cout << " +++ first, ignore check" << std::endl;
      } else {
        std::cout << " +++ check left intersection" << std::endl;
        auto prev = std::prev(inserted_it);
        Point xp;
        auto result = find_xx(prev->seg, inserted_it->seg, &xp, false);
        if (result == Intersection::POINT) {
          if (xp.second < it->y) {
            std::cout << " +++ adding intersection point " << std::endl;

            Event xevent;
            xevent.y = xp.second;
            xevent.seg = prev->seg;
            xevent.type = 0;
            xevent.p = xp;
            events.insert(xevent);
          } else {
            std::cout << " +++ ignoring intersection point " << std::endl;
          }
        } else {
          std::cout << " +++ no intersection point " << std::endl;
        }
      }
      auto nxt = std::next(inserted_it);
      if (nxt == statuses.end()) {
        std::cout << " +++ last, ignore check" << std::endl;
      } else {
        std::cout << " +++ check right intersection" << std::endl;
        Point xp;
        auto result = find_xx(inserted_it->seg, nxt->seg, &xp, false);
        if (result == Intersection::POINT) {
          if (xp.second < it->y) {
            Event xevent;
            xevent.y = xp.second;
            xevent.seg = inserted_it->seg;
            xevent.type = 0;
            xevent.p = xp;
            events.insert(xevent);
            std::cout << " +++ adding intersection point " << std::endl;
          } else {
            std::cout << " +++ ignoring intersection point " << std::endl;
          }
        } else {
          std::cout << " +++ no intersection point " << std::endl;
        }
      }
    }

    if (it->type == 2) {
      std::cout << " remove !! " << std::endl;

      Status st;
      st.seg = it->seg;
      st.x = st.seg->x;
      st.id = st.seg->id;

      auto find_it = statuses.find(st);

      if (find_it == statuses.end()) {
        std::cout << "oh fuck check again" << std::endl;
        st.x = st.seg->second.first;
        find_it = statuses.find(st);
      }

      if (find_it == statuses.end()) {
        std::cout << " +++ bullshit " << std::endl;
        return 1;
      } else {
        if (find_it != statuses.begin() && std::next(find_it) != statuses.end()) {
          auto prev = std::prev(find_it);
          auto nxt = std::next(find_it);
          Point xp;
          auto result = find_xx(prev->seg, nxt->seg, &xp, false);
          if (result == Intersection::POINT) {
            if (xp.second < it->y) {
              Event xevent;
              xevent.y = xp.second;
              xevent.seg = prev->seg;
              xevent.type = 0;
              xevent.p = xp;
              events.insert(xevent);
              std::cout << " +++ adding intersection point " << std::endl;
            } else {
              std::cout << " +++ ignoring intersection point " << std::endl;
            }
          }
        }
        statuses.erase(find_it);
      }
    }

    if (it-> type == 0) {
      std::cout << " xxx Intersection " << std::endl;

      Status st;
      st.seg = it->seg;
      st.x = st.seg->x;
      st.id = st.seg->id;

      auto find_it = statuses.find(st);

      if (find_it == statuses.end()) {
        std::cout << " xxx bullshit " << std::endl;
        return 1;
      }

      auto nxt = std::next(find_it);
      results.emplace_back(std::make_pair(find_it->seg, nxt->seg));

      Segment* left = find_it->seg;
      Segment* right = nxt->seg;

      // remove
      statuses.erase(find_it);
      statuses.erase(nxt);

      int min_id = left->id;
      int max_id = right->id;
      if (min_id > max_id) {
        std::swap(min_id, max_id);
      }
      left->x = it->p.first;
      left->id = max_id;
      right->x = it->p.first;
      right->id = min_id;

      {
        Status left_st;
        left_st.seg = left;
        left_st.x = left->x;
        left_st.id = left->id;
        auto p = statuses.insert(left_st);
        auto inserted_it = p.first;

        auto nxt = std::next(inserted_it);
        if (nxt == statuses.end()) {
          std::cout << " +++ last, ignore check" << std::endl;
        } else {
          std::cout << " +++ check right intersection" << std::endl;
          Point xp;
          auto result = find_xx(inserted_it->seg, nxt->seg, &xp, false);
          if (result == Intersection::POINT) {
            if (xp.second < it->y) {
              Event xevent;
              xevent.y = xp.second;
              xevent.seg = inserted_it->seg;
              xevent.type = 0;
              xevent.p = xp;
              events.insert(xevent);
              std::cout << " +++ adding intersection point " << std::endl;
            } else {
              std::cout << " +++ ignoring intersection point " << std::endl;
            }
          } else {
            std::cout << " +++ no intersection point " << std::endl;
          }
        }
      }

      {
        Status right_st;
        right_st.seg = right;
        right_st.x = right->x;
        right_st.id = right->id;
        auto p = statuses.insert(right_st);
        auto inserted_it = p.first;
        if (inserted_it == statuses.begin()) {
          std::cout << " +++ first, ignore check" << std::endl;
        } else {
          std::cout << " +++ check left intersection" << std::endl;
          auto prev = std::prev(inserted_it);
          Point xp;
          auto result = find_xx(prev->seg, inserted_it->seg, &xp, false);
          if (result == Intersection::POINT) {
            if (xp.second < it->y) {
              std::cout << " +++ adding intersection point " << std::endl;

              Event xevent;
              xevent.y = xp.second;
              xevent.seg = prev->seg;
              xevent.type = 0;
              xevent.p = xp;
              events.insert(xevent);
            } else {
              std::cout << " +++ ignoring intersection point " << std::endl;
            }
          } else {
            std::cout << " +++ no intersection point " << std::endl;
          }
        }
      }
    }
    events.erase(it);
  }

  // TODO
  // Extract a class or some shit to reduce code duplication
  // Figure out ordering

  std::cout << results.size() << std::endl;

  for (auto p : results) {
    Segment* a = p.first;
    Segment* b = p.second;
    std::cout << a->first.first << " " << a->first.second << " " << a->second.first << " " << a->second.second << " ";
    std::cout << b->first.first << " " << b->first.second << " " << b->second.first << " " << b->second.second;
    std::cout << std::endl;
  }

  return 0;
}
