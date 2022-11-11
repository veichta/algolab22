#include<iostream>
#include<vector>

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
  // read input
  int h, w; cin >> h >> w;
  string target_word; cin >> target_word;
  
  vector<string> front_page(h, "");
  vector<string> back_page(h, "");
  
  for (int i=0; i<h; i++) {
    string word; cin >> word;
    front_page[i] = word;
  }
  
  for (int i=0; i<h; i++) {
    string word; cin >> word;
    back_page[i] = word;
  }
  
  // count pairs of front/back words
  vector<vector<int>> pair_counts(26, vector<int>(26, 0));
  for (int i=0; i<h; i++) {
    for(int j=0; j<w; j++) {
      int front_word = ((int) front_page[i][j]) - 65;
      int back_word = ((int) back_page[i][w - j - 1]) - 65;
      
      pair_counts[front_word][back_word]++;
    }
  }
  
  // define graph
  graph G(26*26 + 26);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  // add supply for each pair of words corresponding to the counts and connect
  // them with the words
  for (int i=0; i<26; i++) {
    for(int j=0; j<26; j++) {
      // source -> pair -> word
      
      int capacity = pair_counts[i][j];
      adder.add_edge(v_source, 26*i + j, capacity);
      
      adder.add_edge(26*i + j, 26*26+i, capacity);
      adder.add_edge(26*i + j, 26*26+j, capacity);
    }
  }
  
  // count occurance for each word
  vector<int> counts(26, 0);
  
  int word_length = target_word.size();
  for (int i=0; i<word_length; i++) {
    int word_idx = ((int) target_word[i]) - 65;
    counts[word_idx]++;
  }
  
  // add edge for each word with the corresponding count as capacity
  for (int i =0; i< 26; i++) {
    // word -> sink
    adder.add_edge(26*26 + i, v_sink, counts[i]);
  }
  
  // clculate flow
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  // output
  if (flow == word_length) {
    cout << "Yes" << endl;
  } else {
    cout << "No" << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) { solve(); }
  return 0;
}