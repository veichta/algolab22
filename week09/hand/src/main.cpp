#include<iostream>
#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;


void solve() {
  int n, k, f0, s0; cin >> n >> k >> f0 >> s0;
 
  vector<P> tents;
  for(int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    
    tents.push_back(P(x, y));
  }
  
  int s = 0;
  int f = 0;
  
  cout << s << " " << f << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
  return 0;
}