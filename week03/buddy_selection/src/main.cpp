// STL includes
#include<iostream>
#include<vector>
#include<algorithm>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  return matching_size;
}

int count_common(const vector<string> &ti, const vector<string> &tj) {
  int count = 0;
  int left = 0;
  
  int c = ti.size();
  for (int right=0; right<c; right++) {
    while((left < c) & (ti[right] > tj[left])) {
      left++;
      
      if (left >= c) {
        return count;
      }
    }
    
    if (ti[right] == tj[left]) {
      count++;
    }
  }
  return count;
}

graph init_graph(int n, int c, int f) {
  // read traits
  vector<vector<string>> traits(n, vector<string>(c, ""));
  for (int i=0; i<n; i++) {
    for (int j=0; j<c; j++) {
      string trait; cin >> trait;
      traits[i][j] = trait;
    }
  }
  
  // sort traits
  for (int i=0; i<n; i++) {
    sort(traits[i].begin(), traits[i].end());
  }
  
  graph G(n);
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      if ((i != j) & (i < j)) {
        int common_traits = count_common(traits[i], traits[j]);
        
        if (common_traits > f) {
          boost::add_edge(i, j, G);
        }
      }
    }
  }
  
  return G;
}

bool solve(int n, int c, int f) {
  graph G = init_graph(n, c, f);
  
  int matching_size = maximum_matching(G);

  if (matching_size * 2 == n) {
    return false;
  } else {
    return true;
  }
}

int main()
{
  int T; cin >> T;
  
  for (int t=0; t<T; t++) {
    int n; int c; int f; cin >> n >> c >> f;
    bool optimal = solve(n, c, f);
    
    if (optimal) {
      cout << "optimal" << endl;
    } else {
      cout << "not optimal" << endl;
    }
  }

  return 0;
}
