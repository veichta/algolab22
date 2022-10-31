#include<iostream>
#include<vector>
#include<climits>

using namespace std;

void solve() {
  long n, m, x, k; cin >> n >> m >> x >> k;
  
  // init canals
  vector<vector<pair<long, long>>> canals(n, vector<pair<long, long>>(0));
  for (long i=0; i<m; i++) {
    long u, v, r; cin >> u >> v >> r;
    canals[u].push_back(make_pair(v, r));
  }
  
  // init dp
  long res = LONG_MAX;
  vector<vector<long>> dp(k+1, vector<long>(n, -1));
  dp[0][0] = 0;
  
  // write ahead each round
  for (long j=0; j<k; j++) {
    for(long source=0; source<n; source++) {
      if (dp[j][source] != -1) {
        long m = canals[source].size();
        
        for (long i=0; i<m; i++) {
          long next = canals[source][i].first;
          long reward = canals[source][i].second;
          
          if (canals[next].size() == 0) { // next is a sink -> move to 0 in no steps
            dp[j+1][0] = max(dp[j+1][0], dp[j][source] + reward);
            // dp[j+1][next] = max(dp[j+1][next], dp[j][source] + reward);
            
            if (dp[j+1][0] >= x)
              res = min(res, (long) j+1);
              
          } else {
            dp[j+1][next] = max(dp[j+1][next], dp[j][source] + reward);
            
            if (dp[j+1][next] >= x)
              res = min(res, (long) j+1);
          }
        }
      }
    }
  }
  
  if (res < LONG_MAX) {
    cout << res << endl;
  } else {
    cout << "Impossible" << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  long T; cin >> T;
  while(T--) { solve(); }
  return 0;
}