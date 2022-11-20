#include <iostream>
#include<vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef traits::vertex_descriptor                               vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};


int solve() {
  int n, m, s; cin >> n >> m >> s;
   
  // read l_i
  vector<int> max_per_state(s, 0);
  for(int i=0; i<s; i++) {
    int max_s; cin >> max_s;
    max_per_state[i] = max_s;
  }
   
   // read s_j
  vector<int> in_state(m, 0);
  for(int i=0; i<m; i++) {
    int state; cin >> state;
    in_state[i] = state-1;
  }
  
  // read bids
  int max_bid = 0;
  vector<vector<int>> bids(n, vector<int>(m, 0));
  for(int i=0; i<n; i++) {
    for(int j=0; j<m; j++) {
      int bid; cin >> bid;
      bids[i][j] = bid;
      max_bid = max(max_bid, bid);
    }
  }

  // Create graph, edge adder class and propery maps
  graph G(n+m+s);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // Add the edges
  for (int b_idx=0; b_idx<n; b_idx++) {
    // add buyers: each buyer can at most buy one site
    adder.add_edge(v_source, b_idx, 1, 0);

    // add bids for all sites
    for (int site_idx=0; site_idx<m; site_idx++) {
      // avoid negative costs
      adder.add_edge(b_idx, n+site_idx, 1, max_bid-bids[b_idx][site_idx]);
    }
  }
    
  // add sites to states
  for(int i=0; i<m; i++) {
    adder.add_edge(n+i, n+m+in_state[i], 1, 0);
  }
  
  // add states to sink
  for(int i=0; i<s; i++) {
    adder.add_edge(n+m+i, v_sink, max_per_state[i], 0);
  }

  // Run the algorithm
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost = boost::find_flow_cost(G);
  
  // compute flow
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
  }
  
  cout << s_flow  << " " << max_bid * s_flow - cost << endl;
  
  return 0;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
  return 0;
}