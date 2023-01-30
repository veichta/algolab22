#include<bits/stdc++.h>

using namespace std;

long N;

void init_states(vector<vector<long>> &state, const vector<vector<vector<long>>> &edges, const vector<long> &gold, const long u) {
  // use dfs and store in state the total count of nodes below as well
  // as the time to get pass though all of them
  state[u][1] = 1;

  // go over neighbours
  for (long i = 0; i<long(edges[u].size()); i++) {
    long v = edges[u][i][0];
    long l = edges[u][i][1];
    
    init_states(state, edges, gold, v);
    
    // update state
    state[u][0] += 2 * l + state[v][0];
    state[u][1] += state[v][1];
  }
}

long get_gold(const vector<vector<long>> &state, const vector<vector<vector<long>>> &edges, const vector<long> &gold, const long u, long t) {
  long total = gold[u] - t;
  
  // sort neighbours by increasing count/time
  vector<vector<long>> neighbours;
  for (long i = 0; i<long(edges[u].size()); i++) {
    long v = edges[u][i][0];
    long l = edges[u][i][1];
    neighbours.push_back({state[v][0] + 2*l, state[v][1], i});
  }
  
  sort(neighbours.begin(), neighbours.end(), 
    [](const vector<long>& v1, const vector<long>& v2) -> bool {
        return v1[0] * v2[1] < v1[1] * v2[0];
    }
  );
  
  // take gold in increasing order of time / money
  for (long i = 0; i<long(neighbours.size()); i++) {
    long v = edges[u][neighbours[i][2]][0];
    long l = edges[u][neighbours[i][2]][1];

    t += l;
    long g = get_gold(state, edges, gold, v, t);
    total += g;
    t += l + state[v][0];
  }
  
  return total;
}

void solve() {
  cin >> N;

  vector<long> gold(N+1, 0);
  for (long i=1; i<=N; i++) {
    long g; cin >> g;
    gold[i] = g;
  }

  vector<vector<vector<long>>> tunnels(N+1);
  for (long i=0; i<N; i++) {
    long u, v, l; cin >> u >> v >> l;
    tunnels[u].push_back({v, l});
  }
  
  vector<vector<long>> states(N+1, vector<long>(2, 0));
  init_states(states, tunnels, gold, 0);
  
  long total = get_gold(states, tunnels, gold, long(0), long(0));
  cout << total << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) {solve();}
}