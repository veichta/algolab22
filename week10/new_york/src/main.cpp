// Approach: 
// - similar to San Francisco but keep track of min, max and where the path started
// problem: O(n * m) runtime is not enough, should get down to O(n + m) ?

#include<iostream>
#include<vector>

using namespace std;

void solve() {
  int n, m, k; cin >> n >> m >> k;
  
  vector<int> temp(n, 0);
  for (int i=0; i<n; i++) {
    int t; cin >> t;
    temp[i] = t;
  }
  
  vector<vector<int>> paths(n, vector<int>(0, 0));
  for (int i=0; i<n-1; i++) {
    int u, v; cin >> u >> v;
    paths[u].push_back(v);
  }
  
  vector<vector<vector<int>>> dp(2, vector<vector<int>>(0, vector<int>(0, 0)));

  // init
  for(int i=0; i<n; i++) {
    dp[1 % 2].push_back({i, temp[i], temp[i], i});
  }
  
  for (int l=1; l<m; l++) {
    dp[(l+1) % 2].clear();
    for (unsigned int i = 0; i < dp[l % 2].size(); i++) {
      int u = dp[l % 2][i][0];
      for (int v : paths[u]) {
        int low = dp[l % 2][i][1]; low = min(low, temp[v]);
        int high = dp[l % 2][i][2]; high = max(high, temp[v]);
        
        if (high - low <= k) {
          dp[(l+1) % 2].push_back({v, low, high, dp[l % 2][i][3]});
        }
      }
    }
  }
  
  vector<bool> res(n, false);
  for (unsigned int i = 0; i < dp[m % 2].size(); i++) {
    res[dp[m % 2][i][3]] = true;
  }
  
  bool abort_mission = true;
  for (int i=0; i<n; i++) {
    if (res[i]) {
      cout << i << " ";
      abort_mission = false;
    }
  }
  
  if (abort_mission) {
    cout << "Abort mission";
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
}