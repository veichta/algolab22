#include<bits/stdc++.h>

using namespace std;

long n, m, D, T; 


bool check(const vector<vector<long>> &M, const long gulps_d) {
  vector<pair<long, long>> L1;
  vector<pair<long, long>> L2;
  
  // create subsets for both partitions
  int mid = n / 2;
  for (int s = 0; s < 1<<mid; ++s) { // Iterate through all subsets in L1
    long sum_distance = 0;
    long sum_time = 0;
    for (int i = 0; i < mid; ++i) {
      if (s & 1<<i) { // If i-th element in subset
        sum_distance += M[i][0] + gulps_d;
        sum_time += M[i][1];
      } 
    }
    L1.push_back(make_pair(sum_time, sum_distance));
  }
  

  for (int s = 0; s < 1<<(n-mid); ++s) { // Iterate through all subsets in L2
    long sum_distance = 0;
    long sum_time = 0;
    for (int i = 0; i < (n-mid); ++i) {
      if (s & 1<<i) { // If i-th element in subset
        sum_distance += M[mid+i][0] + gulps_d;
        sum_time += M[mid+i][1];
      } 
    }
    L2.push_back(make_pair(sum_time, sum_distance));
  }
  
  // sort L2 for time and make distance monotonically increasing afterwards
  sort(L2.begin(), L2.end());
  
  long max_d = 0;
  for (size_t i=0; i<L2.size(); i++) {
    max_d = max(L2[i].second, max_d);
    L2[i].second = max_d;
  }
  
  // check if exists item1 and item2 such that asterix arrives before the time is up
  for (auto item1 : L1) {
    
    // bin search max time allowed
    int low = 0;
    int high = L2.size();
    
    while (low < high) {
      int mid = (low + high) / 2;
      auto item2 = L2[mid];
      
      if (item1.first + item2.first >= T) {
        high = mid;
      } else {
        low = mid+1;
      }
    }
    
		// can just check since item2.second stores max dist until time t_i
    if ((low > 0) and (item1.second + L2[low-1].second >= D)) {
      return true;
    }
  }
  
  return false;
}

void solve() {
  cin >> n >> m >> D >> T;
  
  vector<vector<long>> M(n, vector<long>(2, 0));
  for (int i=0; i<n; i++) {
    long d, t; cin >> d >> t;
    M[i] = {d, t};
  }
  
  vector<long> gulps(m, 0);
  for (int i=0; i<m; i++) {
    long val; cin >> val;
    gulps[i] = val;
  }
  
  if (check(M, 0)) {
    cout << 0 << endl;
    return;
  }
  
  // bin search over number of gulps
  int low = 0;
  int high = m;
  
  while (low < high) {
    int mid = (low + high) / 2;
    if (check(M, gulps[mid])) {
      high = mid;
    } else {
      low = mid+1;
    }
  }
  
  if (high == m) {
    cout << "Panoramix captured" << endl;
  } else {
    cout << low + 1 << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--){ solve(); }
  return 0;
}