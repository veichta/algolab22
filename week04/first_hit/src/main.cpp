///1
// std
#include<iostream>
#include<vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

using namespace std;


long floor_to_long(const K::FT  &x) {
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (long) a;
}

void read_segments(vector<S> &segments, const int &n) {
  long r, s, t, u;
  for (int i=0; i<n; i++) {
    cin >> r >> s >> t >> u;
    P start = P(r, s);
    P end = P(t, u);
    
    segments[i] = S(start, end);
  }
}

void solve(const int &n) {
  long x, y, a, b; cin >> x >> y >> a >> b;
  P start = P(x, y);
  P direction = P(a, b);
  R ray = R(start, direction);

  vector<S> segments(n, S(P(0, 0), P(1, 1)));
  read_segments(segments, n);

  // shuffle for randomization
  random_shuffle(segments.begin(), segments.end());
  
  K::FT d;
  P intersection;
  
  bool hit = false;
  int i;

  // find first hit
  for (i=0; i<n; i++) {
    if (CGAL::do_intersect(ray, segments[i])) {
      hit = true;
      auto o = CGAL::intersection(ray, segments[i]);
      
      // check point intersection
      if (const P* op = boost::get<P>(&*o)) {
        d = CGAL::squared_distance(start, *op);
        intersection = *op;
      }
      
      // check segment intersection
      else if (const S* os = boost::get<S>(&*o)) {
        d = CGAL::squared_distance(start, os->source());
        intersection = os->source();
        
        K::FT dist = CGAL::squared_distance(start, os->target());
        if (dist < d) {
          d = dist;
          intersection = os->target();
        }
      }
      break;
    }
  }
  
  if (!hit) {
    cout << "no" << endl;
    return;
  }
  
  // check if there is a closer hit and use ray segment from start to intersection
  // update interection and ray segment with new closest points
  
  S ray_seg = S(start, intersection);
  for (; i<n; i++) {
    if (CGAL::do_intersect(ray_seg, segments[i])) {
      auto o = CGAL::intersection(ray, segments[i]);
      // check point intersection
      if (const P* op = boost::get<P>(&*o)) {
        K::FT dist = CGAL::squared_distance(start, *op);
        if (dist < d) {
          d = dist;
          intersection = *op;
          ray_seg = S(start, intersection);
        }
      }
      
      // check segment intersection
      else if (const S* os = boost::get<S>(&*o)) {
        K::FT dist = CGAL::squared_distance(start, os->source());
        if (dist < d) {
          d = dist;
          intersection = os->source();
          ray_seg = S(start, intersection);
        }
        
        dist = CGAL::squared_distance(start, os->target());
        if (dist < d) {
          d = dist;
          intersection = os->target();
          ray_seg = S(start, intersection);
        }
      }
    }
  }
  
  
  cout 
    << floor_to_long(intersection.x())
    << " " 
    << floor_to_long(intersection.y())
    << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  
  while (n != 0) {
    solve(n);
    cin >> n;
  }
}