#include<iostream>
#include<vector>

using namespace std;


int f(const vector<int> &vals, vector<vector<int>> &mem, int left, int right) {
  if (left > right) {
    return 0;
  } else if (mem[left][right] > -1) { 
    // return value if it was already calculated
    return mem[left][right];
  }
  
  int left_guarantee = min(
    f(vals, mem, left+2, right), 
    f(vals, mem, left+1, right-1)
  ) + vals[left];
  
  int right_guarantee = min(
    f(vals, mem, left+1, right-1), 
    f(vals, mem, left, right-2)
  ) + vals[right];
  
  int res = max(left_guarantee, right_guarantee);
  mem[left][right] = res;
  
  return res;
}

int solve(int n) {
  // read in values
  
  vector<int> vals;
  
  for (int i=0; i<n; i++) {
    int val;
    cin >> val;
    
    
    vals.push_back(val);
  }
  
  int left = 0;
  int right = n-1;
  
  vector<vector<int>> mem(n, vector<int> (n, -1));
  
  int res = f(vals, mem, left, right);
  
  cout << res << endl;
  
  return 0;
}


int main() {
  int T;
  cin >> T;
  
  for (int t = 0; t < T; t++) {
    int n;
    cin >> n;
    
    solve(n);
  }
  
  return 0;
}