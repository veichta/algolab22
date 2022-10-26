#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>

using namespace std;

void init_dp(vector<vector<vector<long>>> &dp, const vector<pair<long, long>> &type_a) {
  int n = (int) type_a.size();
  int H = (int) dp[0][0].size()-1;
  
  for(int h=0; h <= H; h++) {
    for(int i=1; i <= n; i++) {
      dp[i][1][h] = dp[i-1][1][h];
      if ((type_a[i-1].second >= h) && (dp[i][1][h] < type_a[i - 1].first)) {
        dp[i][1][h] = type_a[i-1].first;
      }
    }
  }
  
  for(int j = 2; j <= n; j++) {
    for(int h = 0; h <= H; h++) {
      for(int i = j; i <= n; i++) {
          int prev_h = max(0, h - ((int) type_a[i-1].second));
          dp[i][j][h] = max(dp[i-1][j][h], dp[i-1][j-1][prev_h] + type_a[i-1].first);
      }
    }
  }
}

void solve() {
  // read input
  long n, m; cin >> n >> m;
  long a, b; cin >> a >> b;
  long H, P, W; cin >> P >> H >> W;

  vector<pair<long, long>> type_a;
  for (long i=0; i <n; i++) {
    long pi, hi; cin >> pi >> hi;
    type_a.push_back(make_pair(pi, hi));
  }
  
  vector<long> type_b;
  for (long i=0; i <m; i++) {
    long wi; cin >> wi;
    type_b.push_back(wi);
  }
  
  sort(type_b.begin(), type_b.end());
  
  // dp[i][j][h] describes the maximum amount of power one can get with the
  // first i potions, using exactly j of them to have at least happiness h
  vector<vector<vector<long>>> dp(n+1, vector<vector<long>>(n+1, vector<long>(H+1, LONG_MIN)));
  init_dp(dp, type_a);
  
  long curr_w=0;
  for (long i=0; i<m; i++) {
    curr_w += type_b[m-i-1];
    
    if (curr_w >= W) {
      for (int j=1; j<=n; j++) {
        if (curr_w-a*j < W) {
          break;
        }
        // P + b * (i+1) for numerical stability
        if (dp[n][j][H] >= P + b * (i+1)) {
          cout << (i+1) + j << endl;
          return;
        }
      }  
    }
  }

  cout << -1 << endl;
}

int main() {
  long T; cin >> T;
  while(T--) { solve(); }
  return 0;
}