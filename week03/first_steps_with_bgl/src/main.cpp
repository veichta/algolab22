// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


typedef boost::adjacency_list<
  boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> 
> weighted_graph;

typedef boost::property_map<
  weighted_graph, 
  boost::edge_weight_t
>::type weight_map;

typedef boost::graph_traits<weighted_graph>::edge_descriptor   edge_desc;

using namespace std;

// ----------------------------------- code -----------------------------------
int kruskal_weight(const weighted_graph &G, const weight_map &weights) {
  vector<edge_desc> mst;    // vector to store MST edges (not a property map!)

  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  int mst_weight = 0;
  for (vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    mst_weight += weights[*it];
  }
  
  return mst_weight;
}

int dijkstra_max_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  int res = 0;
  for (int v=1; v < n; v++) {
    res = max(res, dist_map[v]);
  }
  return res;
}

void solve() {
  int n; int m; cin >> n >> m;
  
  // init the Graph
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  edge_desc e;
  for (int i=0; i<m; i++) {
    int u; int v; int w; cin >> u >> v >> w;
    e = boost::add_edge(u, v, G).first; weights[e]=w;
  }
  
  // output results
  cout << kruskal_weight(G, weights) << " " << dijkstra_max_dist(G, 0) << endl;
}

int main() {
  int T; cin >> T;
  
  for (int t=0; t<T; t++){
    solve();
  }
  return 0;
}