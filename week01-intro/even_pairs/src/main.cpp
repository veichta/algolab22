///1
#include <iostream>

using namespace std;

int solve() {
  int n;
  cin >> n;
  
  int even = 0;
  int odd = 0;
  
  int current_sum = 0;
  for (int i=0; i < n; i++) {
    int bit;
    cin >> bit;
    
    current_sum += bit;
    
    if (current_sum % 2 == 0) {
      even++;
    } else {
      odd++;
    }
  }
  
  int res = even * (even-1) / 2 + odd * (odd-1) / 2 + even;
  
  cout << res << endl;
  
  return 0;
}

int main() {
  int T;
  cin >> T;
  
  for (int t = 0; t < T; t++) {
    solve();
  }
  
  return 0;
}