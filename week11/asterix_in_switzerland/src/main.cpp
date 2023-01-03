#include<iostream>
#include<vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

using namespace std;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int n, m; cin >> n >> m;
  
  graph G(n);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  long money_supply = 0;
  vector<int> balances(n, 0);
  for (int i=0; i<n; i++) {
    int b; cin >> b;
    balances[i] = b;

    if (b < 0) { 
      adder.add_edge(i, v_sink, -b);
    } else {
      adder.add_edge(v_source, i, b);
      money_supply += b;
    }
  }
  
  for (int i=0; i<m; i++) {
    int u, v, d; cin >> u >> v >> d;
    adder.add_edge(u, v, d);
  }
  
  // if supply is larger than debt, then there is a solution
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if (flow < money_supply) {
    cout << "yes" << endl;
  } else {
    cout << "no" << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
}