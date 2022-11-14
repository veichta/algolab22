#include<iostream>

#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/push_relabel_max_flow.hpp>

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


void solve() {
  int n, m, k, c; cin >> n >> m >> k >> c;
  
  // stupid edge case
  if (n==0 or m==0) {
    cout << 0 << endl;
    return;
  }
  
  int num_vertices = 2 * n * m; // intersections
  num_vertices += 2 * (n-1) * m + 2 * (m-1) * n; // hallways

  graph G(num_vertices);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  for (int i=0; i<k; i++) {
    int x, y; cin >> x >> y;
    int curr_in = 2 * (y*n + x);
    adder.add_edge(v_source, curr_in, 1);
  }
  
  // create connected graph
  for (int i=0; i<m; i++) {
    for (int j=0; j<n; j++) {
      // current vertex
      int curr_in = 2 * (i*n + j);
      int curr_out = curr_in + 1;

      // right vertex
      int vertex_right_in = 2 * (i*n + j + 1);
      int vertex_right_out = vertex_right_in + 1;

      // vertex down
      int vertex_down_in = 2 * ((i+1)*n + j);
      int vertex_down_out = vertex_down_in + 1;

      // edge to the right
      int edge_right_in = curr_in + 2*n*m + 2 * i * (n-1);
      int edge_right_out = edge_right_in + 1;

      // edge down
      int edge_down_in = edge_right_in + 2 * (n-1);
      int edge_down_out = edge_down_in + 1;
      

      // vertex capacity
      adder.add_edge(curr_in, curr_out, c);
      
      if (i==0) {
        adder.add_edge(curr_out, v_sink, 1);
      }
      if (j==0) {
        adder.add_edge(curr_out, v_sink, 1);
      }
      
      if (j != n-1) {
        // right edge capacity
        adder.add_edge(edge_right_in, edge_right_out, 1);
        
        // curr to right hallway
        adder.add_edge(curr_out, edge_right_in, 1);
        adder.add_edge(edge_right_out, curr_in, 1);
        
        // right hallway to right
        adder.add_edge(edge_right_out, vertex_right_in, 1);
        adder.add_edge(vertex_right_out, edge_right_in, 1);

      } else {
        // add to sink
        adder.add_edge(curr_out, v_sink, 1);
      }
      
      if (i != m-1) {
        // down edge capacity
        adder.add_edge(edge_down_in, edge_down_out, 1);
        
        // curr to down hallway
        adder.add_edge(curr_out, edge_down_in, 1);
        adder.add_edge(edge_down_out, curr_in, 1);
        
        // down hallway to down
        adder.add_edge(edge_down_out, vertex_down_in, 1);
        adder.add_edge(vertex_down_out, edge_down_in, 1);

      } else {
        // add to sink
        adder.add_edge(curr_out, v_sink, 1);
      }
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  cout << flow << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
  return 0;
}