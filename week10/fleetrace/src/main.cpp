#include<iostream>

#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/cycle_canceling.hpp>
#include<boost/graph/push_relabel_max_flow.hpp>
#include<boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include<boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

using namespace std;

void solve() {
  int b, s, p; cin >> b >> s >> p;
  
  graph G(b+s);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_target = boost::add_vertex(G);
  
  // add source, if less boats -> add option to not take boat
  for (int i=0; i<b; i++) {
    adder.add_edge(v_source, i, 1, 0);
    if (b <= s)
      adder.add_edge(i, v_target, 1, 50);
  }
  
  for (int i=0; i<p; i++) {
    int bi, si, ci; cin >> bi >> si >> ci;
    adder.add_edge(bi, b+si, 1, 50-ci); // make sure costs are positive
  }
  
  // add sink, if less sailors -> add option to not take him/her
  for (int si=0; si<s; si++) {
    adder.add_edge(b+si, v_target, 1, 0);
    if (s < b)
      adder.add_edge(v_source, b+si, 1, 50);
  }
  
  // calculate min cost max flow
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  int cost = boost::find_flow_cost(G);
  
  cout << min(b, s) * 50 - cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
}