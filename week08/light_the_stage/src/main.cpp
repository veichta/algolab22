#include<iostream>
#include<vector>

#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/Delaunay_triangulation_2.h>

using namespace std;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void solve() {
  int n, m; cin >> m >> n;
  
  // read persons
  vector<pair<K::Point_2, long>> persons;
  for(int i=0; i<m; i++) {
    int x, y; cin >> x >> y;
    long r; cin >> r;
    
    K::Point_2 loc = K::Point_2(x, y);
    persons.push_back(make_pair(loc, r));
  }
  
  // read lights
  long h; cin >> h;
  vector<K::Point_2> lights;
  for(int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    
    K::Point_2 loc = K::Point_2(x, y);
    lights.push_back(loc);
  }
  
  // binary search over lowest rounds (checking if all players are out or not)
  // storing lowest round per player in which is is out
  int l = 0; int r = n-1;
  vector<int> lowest_out(m, n+1);
   while (l <= r) {
    int mid = (l + r) / 2;
    
    // only check [l, mid] as [0, mid] will be timelimit
    Triangulation t;
    t.insert(lights.begin() + l, lights.begin() + mid + 1);
    
    int players_in = m;
    for (int i=0; i<m; i++) {
      if (lowest_out[i] < mid) {
        players_in--;
        continue;
      }
      
      K::Point_2 loc = persons[i].first;
      int r = persons[i].second;
      K::Point_2 nearest_lamp = t.nearest_vertex(loc)->point();
      
      // check if circles overlap
      auto dist = CGAL::squared_distance(nearest_lamp, loc);
      if (dist < (h+r) * (h+r)) {
        lowest_out[i] = min(lowest_out[i], mid);
        players_in--;
      }
    }
    
    if (players_in == 0) {
      // all players are out -> decrease upper end
      r = mid;
      if (r == l) { break; }
    } else {
      // some players are left -> increase lower end
      l = mid + 1;
    }
  }
  
  int last_round = 0;
  for (int i=0; i<m; i++) {
    last_round = max(last_round, lowest_out[i]);
  }
  
  for (int i=0; i<m; i++) {
    if (lowest_out[i] == last_round) {
      cout << i << " ";
    }
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);

  int T; cin >> T;
  while (T--) { solve(); }
  return 0;
}