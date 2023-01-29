#include<bits/stdc++.h>
using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<double ,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;

typedef Triangulation::Vertex_handle VH;
typedef Triangulation::Face_handle   FH;
typedef K::Point_2                   P;


int N, M;


void solve() {
  // read input
  vector<P> infected(N, P(0, 0));
  for (int i=0; i<N; i++) {
    int x, y; cin >> x >> y;
    infected[i] = P(x, y);
  }
  
  cin >> M;
  vector<double> distances(M, 0);
  vector<P> positions(M, P(0, 0));
  for (int i=0; i<M; i++) {
    long x, y, d; cin >> x >> y >> d;
    positions[i] = P(x, y);
    distances[i] = d;
  }
  
  
  Triangulation t;
  t.insert(infected.begin(), infected.end());
  
  // queue for BFS
  priority_queue<pair<double, FH>> q;
  
  // init all faces
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
    f->info() = 0;
    
    if (t.is_infinite(f)) {
      q.push(make_pair(DBL_MAX, f)); // start from outside with inf cost
    }
  }
  
  
  // fill faces with max over the min edge for all paths that lead to infinite 
  // face
  while (!q.empty()) {
    auto p = q.top(); q.pop();
    
    FH f = p.second;
    double cost = p.first;
    
    if (f->info() >= cost) continue; // already visited with max
    
    // as cost will be the max possible (priority queue) set info to it
    f->info() = cost;

    // iterate over all neighbours and add to queue with min(len(edge), min_edge)
    for (int i=0; i<3; i++) {
      //face
      FH neighbor = f->neighbor(i % 3);
      
      // edge
      P p1 = f->vertex((i+1)%3)->point();
      P p2 = f->vertex((i+2)%3)->point();
      
      // already visited faces
      if (neighbor->info() != 0) continue;
      
      
      double min_cost = min(cost, CGAL::squared_distance(p1, p2));
      q.push(make_pair(min_cost, neighbor));
    }
  }
  
  // answer queries
  for (int i = 0; i<M; i++) {
    double d = distances[i];
    P pos = positions[i];

    FH f = t.locate(pos);
    P nearest = t.nearest_vertex(pos)->point();
    
    if (CGAL::squared_distance(nearest, pos) < d) {
      cout << "n"; // to close initially
    } else {
      // can escape it min edge is larger then 2 * sqrt(d) -> 4 * d
      int valid = (f->info() >= 4 * d); 
      cout << "ny"[valid];
    }
  }
  cout << endl;
}


int main() {
  ios_base::sync_with_stdio(false);
  cin >> N;
  while (N) {
    solve();
    cin >> N;
  }
  return 0;
}