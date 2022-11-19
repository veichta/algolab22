#include<iostream>
#include<vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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

void make_edge(edge_adder &adder, vector<vector<int>> &board, int n, int u, pair<int, int> v) {
  if ((v.first >= 0) and (v.first < n)) {
    if ((v.second >= 0) and (v.second < n)) {
      if (board[v.first][v.second]) {
        adder.add_edge(u, v.first * n + v.second, 1);
      }
    }
  }
}

void solve() {
  int n; cin >> n;
  
  // read in board
  int n_present = 0;
  vector<vector<int>> board(n, vector<int>(n, 0));
  for (int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      cin >> board[i][j];

      if (board[i][j])
        n_present++;
    }
  }
  
  // init
  graph G(n*n);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // create graph
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      int idx = i*n + j;
      
      if (not board[i][j]) {
        continue;
      }
  
      if ((i+j) % 2) {
        // add source to black tiles
        adder.add_edge(v_source, idx, 1);

        // add attacked neighbours
        make_edge(adder, board, n, idx, make_pair(i-1, j-2));
        make_edge(adder, board, n, idx, make_pair(i-1, j+2));
        
        make_edge(adder, board, n, idx, make_pair(i+1, j-2));
        make_edge(adder, board, n, idx, make_pair(i+1, j+2));
        
        make_edge(adder, board, n, idx, make_pair(i-2, j-1));
        make_edge(adder, board, n, idx, make_pair(i-2, j+1));
        
        make_edge(adder, board, n, idx, make_pair(i+2, j-1));
        make_edge(adder, board, n, idx, make_pair(i+2, j+1));
        
      } else {
        // add white tiles to sink
        adder.add_edge(idx, v_sink, 1);
      }
    }
  }
  
  // compute max IS
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  long maxIS = n_present - flow;
  
  cout << maxIS << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
  return 0;
}