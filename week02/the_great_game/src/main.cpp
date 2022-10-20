#include<iostream>
#include<vector>
#include<climits>

using namespace std;

void solve() {
  int n, m; cin >> n >> m;
  int r, b; cin >> r >> b;
  r--; b--;
  vector<vector<int>> transition(n, vector<int>(0));
  
  for (int i=0; i<m; i++) {
    int u, v; cin >> u >> v;
    transition[u-1].push_back(v-1);
  }
  
  vector<int> max_steps(n, -1);
  vector<int> min_steps(n, INT_MAX);
  
  max_steps[n-1] = 0; min_steps[n-1] = 0;
  for (int u=n-2; u>=0; u--) {
    for (unsigned int j=0; j<transition[i].size(); j++) {
      int v = transition[i][j];
      
      // calc best and worst moves
      max_steps[u] = max(max_steps[u], min_steps[v] + 1);
      min_steps[u] = min(min_steps[u], max_steps[v] + 1);
    }
  }
  
  if (min_steps[r] < min_steps[b]) {
    // red wins if it requires less steps
    cout << 0 << endl;
  } else if (min_steps[b] < min_steps[r]) {
    // black wins if it requires less steps
    cout << 1 << endl;
  } else if (min_steps[b]%2 == 1) {
    // red wins if the steps are euqal, but the game takes an odd number of steps
    cout << 0 << endl;
  } else {
    // black wins if the steps are euqal, but the game takes an even number of steps
    cout << 1 << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  
  for (int t=0; t<T; t++) {
    solve();
  }
  return 0;
}