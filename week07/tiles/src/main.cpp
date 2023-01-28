#include<bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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

using namespace std;

int H, W;

void solve() {
  cin >> W >> H;
  
  graph G(H * W);
  edge_adder adder(G);

  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  int n_empty = 0;
  for(int h=0; h<H; h++) {
    string row; cin >> row;
    for (int w=0; w<W; w++) {
      bool is_white = (w+h) % 2;
      if (row[w] == '.') {
        n_empty++;
        if (is_white) {
          // add source to tile
          adder.add_edge(v_source, h*W + w, 1);

          // add flow to possible neighbours
          if (w > 0) { // left
            adder.add_edge(h*W + w, h*W + w-1, 1);
          }
          if (w < W-1) { // right
            adder.add_edge(h*W + w, h*W + w+1, 1);
          }
          if (h > 0) { // up
            adder.add_edge(h*W + w, (h-1)*W + w, 1);
          }
          if (h < H - 1) { // down
            adder.add_edge(h*W + w, (h+1)*W + w, 1);
          }
        } else {
          // add tile to sink
          adder.add_edge(h*W + w, v_sink, 1);
        }
      }
    }
  }
  
  if (n_empty & 1) {
    cout << "no" << endl;
    return;
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if ((flow == n_empty/2)) {
    cout << "yes" << endl;
  } else {
    cout << "no" << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) solve();
  return 0;
}