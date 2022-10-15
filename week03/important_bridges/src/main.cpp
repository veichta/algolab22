/// std
#include<iostream>
#include<vector>
#include<algorithm>

// BGL includes 
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/biconnected_components.hpp>


// BGL Graph definitions
// =====================
namespace boost{
  struct edge_component_t{
    enum{num = 555};
    typedef edge_property_tag kind;
  } edge_component;
}

typedef boost::adjacency_list<
  boost::vecS, boost::vecS, 
  boost::undirectedS,
  boost::no_property,
  boost::property<boost::edge_component_t, size_t>
> graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator    edge_it;

using namespace std;

// Solution
// =====================
void find_bridges(graph &G, int n) {
  boost::property_map<graph, boost::edge_component_t>::type component = boost::get(boost::edge_component, G);
  
  size_t num_comps = biconnected_components(G, component);

  // get size of components: ei is a bridge <=> ei isolated component
  vector<int> comp_size(num_comps, 0);
  boost::graph_traits<graph>::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
    comp_size[component[*ei]]++;
  }
  
  // find bridges which are singel 
  vector<pair<int, int>> bridges;
  for (boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
    if (comp_size[component[*ei]] == 1) {
      // store edges in specified output
      int u = min(source(*ei, G), target(*ei, G));
      int v = max(source(*ei, G), target(*ei, G));
      bridges.push_back(make_pair(u, v));
    }
  }

  // sort edges for output
  sort(bridges.begin(), bridges.end());
  cout << bridges.size() << endl;
  for(unsigned int i=0; i<bridges.size(); i++) {
    cout << bridges[i].first << " " << bridges[i].second << endl;
  }
}

void create_graph(graph &G, int m) {
  for (int i=0; i<m; i++) {
    int u; int v; cin >> u >> v;
    boost::add_edge(u, v, G);
  }
}

void solve(int n, int m) {
  graph G(n);
  create_graph(G, m);
  find_bridges(G, n);
}

int main() {
  int T; cin >> T;
  
  for (int t=0; t<T; t++) {
    int n; int m; cin >> n >> m;
    solve(n, m);
  }
  return 0;
}