#include<iostream>
#include<vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

void solve(int n) {
  // read restaurants
  vector<K::Point_2> restaurants;
  for(int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    restaurants.push_back(K::Point_2(x, y));
  }
  
  // build triangulation
  Triangulation t;
  t.insert(restaurants.begin(), restaurants.end());
  
  // check locations
  int m; cin >> m;
  for(int i=0; i<m; i++) {
    int x, y; cin >> x >> y;
    K::Point_2 location = K::Point_2(x, y);

    K::Point_2 nearest = t.nearest_vertex(location)->point();
    cout << CGAL::squared_distance(location, nearest) << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cout << setprecision(0) << fixed;
  
  int n; cin >> n;
  while (n != 0) {
    solve(n);
    cin >> n;
  }
  return 0;
}