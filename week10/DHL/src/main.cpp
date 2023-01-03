#include<iostream>
#include<vector>
#include<climits>

using namespace std;

int vec_sum(const vector<int> &v, int begin, int end) {
  int s = v[end - 1];
  if (begin > 0) { s -= v[begin - 1]; }
  return s;
}

int calc_min_cost(
  const vector<int> &A, 
  const vector<int> &B, 
  int a, int b, 
  vector<vector<int>> &memo,
  const vector<int> &SA, 
  const vector<int> &SB
) {
  // init
  int n = A.size();
  int best = INT_MAX;
  
  // check if one is the last element -> need to take the rest
  if (a == n-1 or b == n-1) {
    return (vec_sum(SA, a, n) - (n-a)) * (vec_sum(SB, b, n) - (n - b));
  }

  int rem_cost;
  
  // take one from a
  int ca = (A[a] - 1);
  for (int i=b+1; i<n; i++) {

    // check if already computed
    if (memo[a+1][i] < INT_MAX) {
      rem_cost = memo[a+1][i];
    } else {
      rem_cost = calc_min_cost(A, B, a + 1, i, memo, SA, SB);
    }
    
    // calculate cost
    int cb = (vec_sum(SB, b, i) - (i-b));
    best = min(best, ca*cb + rem_cost);
  }
  
  // take one from b
  int cb = (B[b] - 1);
  for (int i=a+1; i<n; i++) {

    // check if already computed
    if (memo[i][b + 1] < INT_MAX) {
      rem_cost = memo[i][b + 1];
    } else {
      rem_cost = calc_min_cost(A, B, i, b + 1, memo, SA, SB);
    }
    
    // calculate cost
    int ca = (vec_sum(SA, a, i) - (i-a));
    best = min(best, ca*cb + rem_cost);
  }
  
  // remeber the best value
  memo[a][b] = best;
  
  return best;
}

void solve() {
  int n; cin >> n;
  
  // stack A and cumsum A
  vector<int> A(n, 0);
  for (int i=0; i<n; i++) {
    int ai; cin >> ai;
    A[n-i-1] = ai;
  }
  
  vector<int> SA(n, 0);
  SA[0] = A[0];
  for(int i=1; i<n; i++) {
    SA[i] = A[i] + SA[i-1];
  }

  // stack B and cumsum B
  vector<int> B(n, 0);
  for (int i=0; i<n; i++) {
    int bi; cin >> bi;
    B[n-i-1] = bi;
  }
  
  vector<int> SB(n, 0);
  SB[0] = B[0];
  for(int i=1; i<n; i++) {
    SB[i] = B[i] + SB[i-1];
  }
  
  // memo
  vector<vector<int>> memo(n, vector<int>(n, INT_MAX));
  
  
  // solve
  cout << calc_min_cost(A, B, 0, 0, memo, SA, SB) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
}
