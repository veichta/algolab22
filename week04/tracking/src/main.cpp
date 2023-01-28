#include<bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}


void solve() {
  int N, M, K, X, Y; cin >> N >> M >> K >> X >> Y;
  weighted_graph G((K+1)*N);
  weight_map weights = boost::get(boost::edge_weight, G);

  edge_desc e;
  for (int i=0; i<M; i++) {
    int u, v, t, d; cin >> u >> v >> t >> d;
    
    for (int k=0; k <= K; k++) {
      e = boost::add_edge(u + k*N, v + k*N, G).first; weights[e]=t;
      e = boost::add_edge(v + k*N, u + k*N, G).first; weights[e]=t;

      if (d and (k != K)) {
        e = boost::add_edge(u + k*N, v + (k + 1)*N, G).first; weights[e]=t;
        e = boost::add_edge(v + k*N, u + (k + 1)*N, G).first; weights[e]=t;
      }
    }
  }

  cout << dijkstra_dist(G, X, Y + K*N) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) {solve();}
  return 0;
}