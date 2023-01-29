#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/bipartite.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef Delaunay::Vertex_handle    VH;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge>             EdgeV;
typedef std::pair<K::Point_2,Index>   IPoint;
typedef K::Point_2                    P;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;  
typedef boost::graph_traits<graph>::edge_iterator                           edge_it;


long N, M, R; 


bool check_neighbours(const Delaunay &t, const VH &v1, const VH &v2) {
  // loop over all neighbors ov v1 and check if any is close to v2
  // make sure to ignore cases where the neighbor is v1 or v2
  auto c = t.incident_edges(v1);
  do {
    if ((not t.is_infinite(c)) and (t.segment(c).squared_length() <= R * R)) {
      auto n1 = c->first->vertex((c->second+1)%3);
      auto n2 = c->first->vertex((c->second+2)%3);
      
      long d1 = CGAL::squared_distance(v2->point(), n1->point());
      long d2 = CGAL::squared_distance(v2->point(), n2->point());
      
      if ( ((n1 != v1) and (n1 != v2) and (d1 <= R * R))
        or ((n2 != v1) and (n2 != v2) and (d2 <= R * R)) ) {
        return false;
      }
      
    }
  } while (++c != t.incident_edges(v1));
  
  return true;
}


void solve() {
  cin >> N >> M >> R;
  
  vector<IPoint> clients;
  clients.reserve(N);
  for (int i = 0; i < N; ++i) {
    int x, y; cin >> x >> y;
    clients.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay t;
  t.insert(clients.begin(), clients.end());

  // create graph
  graph G(N);
  int inference = 1;
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    auto v1 = e->first->vertex((e->second+1)%3);
    auto v2 = e->first->vertex((e->second+2)%3);
    
    if (t.segment(e).squared_length() <= R * R){
      boost::add_edge(v1->info(), v2->info(), G);
      
      // edge cases: check if noighbors of v2 are too close to v1 and vice versa
      inference *= check_neighbours(t, v1, v2);
      inference *= check_neighbours(t, v2, v1);
    }
  }
  
  inference *= is_bipartite(G); // check if bipartit

  vector<int> component_map(N);
  boost::connected_components(G, boost::make_iterator_property_map(
    component_map.begin(), boost::get(boost::vertex_index, G)
  ));
  
  for (int i=0; i<M; i++) {
    int x, y; 
    
    cin >> x >> y;
    P p1 = P(x, y);
    auto c1 = t.nearest_vertex(p1);
    int i1 = c1->info();
    
    cin >> x >> y;
    P p2 = P(x, y);
    auto c2 = t.nearest_vertex(p2);
    int i2 = c2->info();
    
    // check if sherlock and watson are in range of any client 
    // and if the clients are in a connected network
    int valid = (CGAL::squared_distance(p1, c1->point()) <= R * R);
    valid *= (CGAL::squared_distance(p2, c2->point()) <= R * R);
    valid *= (component_map[i1] == component_map[i2]);
    
    // check if sherlock and watson can directly connect
    if (CGAL::squared_distance(p1, p2) <= R * R) {
      valid = 1;
    }
    
    cout << "ny"[inference * valid];
  }
  cout << endl;
}


int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
  return 0;
}