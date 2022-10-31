///1
#include<iostream>
#include<vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2   P;
typedef K::Segment_2 S;
typedef K::Ray_2     R;

using namespace std;

int read_input(vector<R> &bikers) {
  int n; cin >> n;
  for (int i=0; i<n; i++) {
    long y0, x1, y1; cin >> y0 >> x1 >> y1;
    R biker = R(P(0, y0), P(x1, y1));
    bikers.push_back(biker);
  }
  return n;
}

bool bikers_intersect(R &biker1, R &biker2) {
  bool to_inf = true;
  
  if (CGAL::do_intersect(biker1, biker2)) {
    auto o = CGAL::intersection(biker1, biker2);
    if (const P* op = boost::get<P>(&*o)) {
      K::FT d1 = CGAL::squared_distance(*op, biker1.source());
      K::FT d2 = CGAL::squared_distance(*op, biker2.source());
      
      if (d1 > d2) {
        to_inf = false;
      } else if ((d1 == d2) && (biker1.source().y() > biker2.source().y())) {
        to_inf = false;
      }
    }
  }
  return to_inf;
}

void solve() {
  vector<R> bikers;
  int n = read_input(bikers);
  
  for (int i=0; i<n; i++) {
    R biker1 = bikers[i];
    bool to_inf = true;
    for (int j=0; j<n; j++) {
      if (i != j) {
        R biker2 = bikers[j];
        if (CGAL::do_intersect(biker1, biker2)) {
          auto o = CGAL::intersection(biker1, biker2);
          if (const P* op = boost::get<P>(&*o)) {
            K::FT d1 = CGAL::squared_distance(*op, biker1.source());
            K::FT d2 = CGAL::squared_distance(*op, biker2.source());
            
            if (d1 > d2) {
              to_inf = false;
              break;
            } else if ((d1 == d2) && (biker1.source().y() > biker2.source().y())) {
              to_inf = false;
              break;
            }
          }
        }
      }
    }
    if (to_inf) {
      cout << i << " ";
    }
  }
  cout << endl;
}


int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) { solve(); }
}