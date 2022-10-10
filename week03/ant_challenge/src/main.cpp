///
// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>


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
vector<edge_desc> kruskal_weight(const weighted_graph &G) {
  vector<edge_desc> mst;
  
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  return mst;
}

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

void solve() {
  int n; int m; int s; int a; int b;
  cin >> n >> m >> s >> a >> b;
  
  // read in edges
  vector<vector<int>> weights(m, vector<int>(s, 0));
  vector<pair<int, int>> edges;
  
  for (int i=0; i<m; i++) {
    int u; int v; int w;
    cin >> u >> v;
    edges.push_back(make_pair(u, v));
    
    for (int j=0; j<s; j++) {
      cin >> w;
      weights[i][j] = w;
    }
    
  }
  
  // read in hive locations
  vector<int> hives(s, 0);
  for (int i=0; i<s; i++) {
    int hive_i; cin >> hive_i;
    hives[i] = hive_i;
  }
  
  // solution
  edge_desc e;
  weighted_graph privat_G(n);
  weight_map mst_weights = boost::get(boost::edge_weight, privat_G);
  
  for (int species=0; species<s; species++) {
    // init the species specific graph
    weighted_graph G(n);
    weight_map weights_map = boost::get(boost::edge_weight, G);
    
    for (int i=0; i<m; i++) {
      int u = edges[i].first;
      int v = edges[i].second;
      e = boost::add_edge(u, v, G).first; 
      weights_map[e]=weights[i][species];
    } 
    
    // build mst (private network)
    vector<edge_desc> mst = kruskal_weight(G);
    
    // add mst to overall private network
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
      int u = boost::source(*it, G);
      int v = boost::target(*it, G);
      int w = weights_map[*it];
      e = boost::add_edge(u, v, privat_G).first; mst_weights[e] = w;
    }
  }
  
  // find shortest path from a -> b
  cout << dijkstra_dist(privat_G, a, b) << endl;
}

int main() {
  int T; cin >> T;
  
  for (int t=0; t<T; t++){
    solve();
  }
  return 0;
}