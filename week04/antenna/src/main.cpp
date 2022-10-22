#include<iostream>
#include<vector>
#include<iomanip>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using namespace std;

typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

int solve(int n) {
  // Read points
  vector<K::Point_2> P;
  for(int i=0; i<n; i++) {
    long x; long y; cin >> x >> y;
    P.push_back(K::Point_2(x, y));
  }
  
  // Find min enclosing circle
  Min_circle mc(P.begin(), P.end(), true);
  Traits::Circle c = mc.circle();
  auto r = ceil(CGAL::sqrt(c.squared_radius()));
  std::cout << r << std::endl;
  
  return 0;
}

int main() {

  int n; cin >> n;
  while(n != 0) {
    solve(n);
    cin >> n;
  }
  
  return 0;
}