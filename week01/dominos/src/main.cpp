#include <iostream>

using namespace std;

int solve() {
  int n;
  cin >> n;
  
  int right=1;
  for (int i=0; i < n; i++) {
    int val;
    cin >> val;

    if (i < right) {
      right = max(right, i+val);
    }
    
  }
  
  int res = min(right, n);
  cout << res << endl;
  
  return res;
}

int main() {
  int T;
  cin >> T;
  
  for (int t=0; t < T; t++) {
    solve();
  }
  
  
  return 0;
}