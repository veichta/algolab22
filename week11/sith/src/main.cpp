#include<iostream>
#include<vector>
#include<tuple>

#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/Delaunay_triangulation_2.h>
#include<CGAL/Triangulation_vertex_base_with_info_2.h>
#include<CGAL/Triangulation_face_base_2.h>
#include<boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

using namespace std;

int check_round(const vector<IPoint> &points, int emp_size, long dist, int n) {
  Delaunay t;
  t.insert(points.begin() + emp_size, points.end());
  
  // get components
  boost::disjoint_sets_with_storage<> uf(n);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index u = e->first->vertex((e->second+1)%3)->info();
    Index v = e->first->vertex((e->second+2)%3)->info();
    
    // skip edges which are too far
    if (t.segment(e).squared_length() > dist) {
      continue;
    }
    
    Index c1 = uf.find_set(u);
    Index c2 = uf.find_set(v);
    if (c1 != c2) {
      // this edge connects two different components
      uf.link(c1, c2);
    }
  }
  
  // calc comp sizes
  vector<int> comp_size(n, 0);
  for(int i=0; i<n; i++) {
    Index comp = uf.find_set(i);
    comp_size[comp]++;
  }

  // find max
  int max_comp = 0;
  for (int s : comp_size) {
    max_comp = max(max_comp, s);
  }

  return max_comp;
}

void solve() {
  long n, dist;
  cin >> n >> dist;
  
  dist = dist * dist;

  vector<IPoint> points;
  for (int i=0; i<n; ++i) {
    int x, y; cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  
  // bin-search first day where comp_size is smaller then the emperior size
  // result will be that day - 1
  int low = 1;
  int high = n/2+1;
  while(low < high) {
    int emp_size = (low + high) / 2;
    int comp_size = check_round(points, emp_size, dist, n);
    
    if (comp_size >= emp_size) {
      low = emp_size+1;
    } else {
      high = emp_size;
    }
  }
  
  cout << low-1 << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
}