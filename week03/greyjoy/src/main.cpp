#include<iostream>
#include<vector>

using namespace std;


long solve() {
  long res = 0;
  long n, k, w; cin >> n >> k >> w;
  
  vector<long> weights(n, 0);
  for (long i=0; i<n; i++) {
    long val; cin >> val;
    weights[i] = val;
  }
  
  vector<vector<long>> paths(w, vector<long>(1, weights[0]));
  
  for (long i=0; i<w; i++) {
    long left=0;
    long l; cin >> l;
    long island; cin >> island; // always 0
    
    for (long j=1; j<l; j++) {
      cin >> island;
      long s = weights[island] + paths[i][j-1];
      paths[i].push_back(s);
      
      while((left < j) && (paths[i][j] - paths[i][left] > k)) {left++;}
      
      if ((paths[i][j] - paths[i][left]) == k) {
        res = max(res, j - left);
      }
    }
  }
  
  k -= weights[0];
  if (k < 0) {
    return res;
  } else if (k == 0) {
    return max(res, (long) 1);
  }
  
  vector<long> max_ind(k+1, -1);
  max_ind[0] = 0;
  
  for (long w_idx=0; w_idx<w; w_idx++) {
    vector<pair<int, int>> updates;
    
    for (unsigned long i=1; i<paths[w_idx].size(); i++) {
      long s = paths[w_idx][i] - weights[0];
      
      if (s <= k) {
        if (max_ind[k-s] != -1) {
          res = max(res, 1 + max_ind[k-s] + (long) i);
        }
        updates.push_back(make_pair(s, max((long) i, max_ind[s])));
      } else {break;}
    }
    
    for (unsigned int i=0; i<updates.size(); i++) {
      max_ind[updates[i].first] = updates[i].second;
    }
  }
  return res;
}

int main() {
  ios_base::sync_with_stdio(false);
  long T; cin >> T;
  
  while (T--) { cout << solve() << endl; }
  
  return 0;
}