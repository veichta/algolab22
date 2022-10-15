#include<iostream>
#include<vector>

using namespace std;

int solve(int n, int k) {
  // read vals
  vector<int> vals;
  int val; cin >> val;
  vals.push_back(val);
  for (int i = 1; i < n; i++) {
    int val; cin >> val;
    vals.push_back(val);
  }
  
  // init 
  int left = 0; int right = 0;
  int low = 0; int high = 0;
  
  int closest = abs(k-vals[0]);
  int current_sum=0;
  
  while (right < n) {
    current_sum += vals[right]; 
    
    // check for new best
    if (abs(current_sum - k) < closest) {
      closest = abs(current_sum -k);
      low = left;
      high = right;
    }

    // increase the left pointer if the difference is to big
    while ((current_sum - k >= closest) & (left <= right)) {
      current_sum -= vals[left];
      left++;

      // check again for new best in order to increase 
      // left as much as possible
      if (abs(current_sum - k) < closest) {
        closest = abs(current_sum -k);
        low = left;
        high = right;
      }
    }

    right++;
  }
  
  cout << low << " " << high << endl;
  return 0;
}

int main() {
  int T;
  cin >> T;
  
  
  for (int t=0; t < T; t++) {
    int n; int k; cin >> n >> k;
    solve(n, k);
  }
  
  return 0;
}