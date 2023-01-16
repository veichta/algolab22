#include<iostream>
#include<vector>
#include<climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include<boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

using namespace std;

typedef vector<int> VI;
typedef vector<vector<int>> VVI;

int N, M, A, S, C, D;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  VI dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

void calc_pairwise_dist(
  const VVI &edges,
  const VI &agents,
  const VI &shelters,
  VVI &dists
) {
  weighted_graph G(N);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  edge_desc e;
  for (int i=0; i<M; i++) {
    int u = edges[i][1];
    int v = edges[i][2];
    int w = edges[i][3];
    
    e = boost::add_edge(u, v, G).first; weights[e] = w;
    if (not edges[i][0]) { // lift -> add reverse edge
      e = boost::add_edge(v, u, G).first; weights[e] = w;
    }
  }
  
  for (int i=0; i<A; i++) {
    int agent = agents[i];

    // calc distances
    VI dist_map(N);
    boost::dijkstra_shortest_paths(G, agent,
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));
        
    for (int j=0; j<S; j++) {
      int shelter = shelters[j];
      dists[i][j] = dist_map[shelter];
    }
  }
}

bool check(const VVI &dists, long t) {
  weighted_graph G(A + C * S);
  
  for (int i=0; i<A; i++) {
    for (int j=0; j<S; j++) {
      if (dists[i][j] == INT_MAX) {
        continue;
      }
      
      if (dists[i][j] < t - D) {
        boost::add_edge(i, A + j, G);
      }
      if ((dists[i][j] < t - 2 * D) and C > 1) {
        boost::add_edge(i, (A + S) + j, G);
      }
    }
  }
  
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);

  // compute max matching
  boost::edmonds_maximum_cardinality_matching(
      G, 
      boost::make_iterator_property_map(
          mate_map.begin(), 
          boost::get(boost::vertex_index, G)
      )
  );

  // get size of matching
  int matching_size = boost::matching_size(
      G, 
      boost::make_iterator_property_map(
          mate_map.begin(), 
          boost::get(boost::vertex_index, G)
      )
  );

  return matching_size == A;
}

void solve() {
  cin >> N >> M >> A >> S >> C >> D;
  
  VVI edges(M, VI(4, 0));
  for (int i=0; i<M; i++) {
    string s; cin >> s; 
    int x, y, z; cin >> x >> y >> z;
    edges[i] = {s == "S", x, y, z};
  }
  
  VI agents(A, 0);
  for (int i=0; i<A; i++) {
    cin >> agents[i];
  }
  
  VI shelters(S, 0);
  for (int i=0; i<S; i++) {
    cin >> shelters[i];
  }
  
  VVI dists(A, VI(S, INT_MAX));
  calc_pairwise_dist(edges, agents, shelters, dists);

  long low = 0; long high = INT_MAX;
  while (low < high) {
    long mid = (low + high) / 2;

    if (not check(dists, mid)) {
      low = mid+1;
    } else {
      high = mid;
    }
  }
  
  cout << low - 1 << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
}