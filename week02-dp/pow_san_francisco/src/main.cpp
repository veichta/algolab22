#include<iostream>
#include<vector>

using namespace std;


int f(
  vector<vector<int>> &memo, 
  const vector<vector<pair<int, int>>> &c, 
  int u, int k
) {
  int res = 0;
  if (k == 0) {
    return res;
  } else {
    bool sink = true;
    for (unsigned int i=1; i < c[u].size(); i++) {
      int v = c[u][i].first;
      int r = c[u][i].second;
      
      if (r >= 0) {
        
        if (memo[v][k-1] >=0) {
          res = max(res, r + memo[v][k-1]);
          // cout << "Reading f(" << u << ", " << v <<") from memo" << endl;
          
        } else {
          int val = f(memo, c, v, k-1);
          memo[v][k-1] = val;
          res = max(res, r + val);
          
        }
        sink = false;
      }
    }
    
    if (sink) {
      res = f(memo, c, 0, k);
    }
    
    return res;
  }
}

int solve() {
  int n;
  cin >> n;
  int m;
  cin >> m;
  
  int x;
  cin >> x;
  
  int k;
  cin >> k;
  
  // init canals
  // vector<vector<int>> canals(n, vector<int> (m, -1));
  // for (int i=0; i<m; i++) {
  //   int u; int v; int r;
  //   cin >> u;
  //   cin >> v;
  //   cin >> r;
  //   canals[u][v] = max(canals[u][v], r);
  // }
  
  vector<vector<pair<int, int>>> canals(n, vector<pair<int, int>> (1, make_pair(-1, -1)));
  // cout << canals[0][0].first << endl;
  for (int i=0; i<m; i++) {
    int u; int v; int r;
    cin >> u;
    cin >> v;
    cin >> r;
    canals[u].push_back(make_pair(v, r));
  }
  
  // store all f(u, k')
  vector<vector<int>> memo(n, vector<int> (k, -1));
  // check for all i < k
  int sol = -1;
  for (int i=1; i <= k; i++) {
    int max_val = f(memo, canals, 0, i);
    
    //cout << "i: " << i << ", diff: "<< max_val-x << endl;

    if (max_val >= x) {
      sol = i;
      break;
    }
  }
  
  // print solution
  if (sol >= 0) {
    cout << sol << endl;
  } else {
    cout << "Impossible" << endl;
  }
  
  //cout << endl;
  return 0;
}


int main() {
  int T;
  cin >> T;
  
  for (int t=0; t<T; t++) {
    solve();
  }
  
  return 0;
}