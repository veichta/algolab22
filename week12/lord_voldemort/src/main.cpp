#include<bits/stdc++.h>

using namespace std;

long get_len(const pair<long, long> &ppl) {
  return get<1>(ppl);
}
long get_left(const pair<long, long> &ppl) {
  return get<0>(ppl);
}
long get_right(const pair<long, long> &ppl) {
  return get<0>(ppl) + get<1>(ppl) - 1;
}

void solve() {
  long n, m, k; cin >> n >> m >> k;
  
  vector<long> objects(n, 0);
  for (long i=0; i<n; i++) {
    cin >> objects[i];
  }
  
  // find all permissable intervals
  vector<pair<long, long>> intervals;
  long l = 0;
  long sum = 0;
  
  for (int r = 0; r<n; r++) {
    sum += objects[r];
    
    while (sum >= k and l <= r) {
      if (sum == k)
        intervals.push_back(make_pair(l, r-l+1)); // intervals are start, start + len
      
      sum -= objects[l];
      l++;
    }
  }
  
  long N = intervals.size(), M = m;
  
  if (N < M) {
    cout << "fail" << endl;
    return;
  }
  
  // init
  vector<vector<long>> dp(M, vector<long>(N, -1));
  for(long i=0; i<N; i++) {
    dp[0][i] = get_len(intervals[i]);
  }
  
  
  // dp
  for (long i=1; i<M; i++) {
    long idx_prev = 0;
    long max_pred = -1;
    
    for (int j=0; j<N; j++) {
      auto current = intervals[j];

      while (get_right(intervals[idx_prev]) < get_left(current)) {
        max_pred = max(max_pred, dp[i-1][idx_prev]);
        idx_prev++;
      }
      
      if (max_pred != -1)
        dp[i][j] = max_pred + get_len(current);
      }
  }
  
  long ret = -1;
  for (int i=0; i<N; i++) {
    ret = max(dp[M-1][i], ret);
  }
  
  if (ret == -1) {
    cout << "fail" << endl;
  } else {
    cout << ret << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  long T; cin >> T;
  while (T--) { solve(); }
}