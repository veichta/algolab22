// STD
#include<iostream>

// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

using namespace std;

void solve(int n) {
  // create photon ray
  long x; long y; long a; long b;
  cin >> x >> y >> a >> b;
  
  const P source = P(x, y);
  const P dir = P(a, b);
  R photon = R(source, dir);
  
  bool hit = false;
  
  // read segments and check for intersection
  for (int i=0; i<n; i++) {
    long x; long y; long a; long b;
    cin >> x >> y >> a >> b;
    
    if (!hit) { // only check if not already found a hit
      P start = P(x, y);
      P end = P(a, b);
      
      S obstacle = S(start, end);
      if (CGAL::do_intersect(photon, obstacle)) {
        hit = true;
      }
    }
  }
  
  if (hit) {
    cout << "yes" << endl;
  } else {
    cout << "no" << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int n; cin >> n;
  
  while (n != 0) { 
    solve(n);
    
    cin >> n;
  }
  
  return 0;
}