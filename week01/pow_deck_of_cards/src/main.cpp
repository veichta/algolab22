#include<iostream>
#include<vector>

using namespace std;



int solve_n2() {
  int n;
  cin >> n;
  
  int k;
  cin >> k;
  
  vector<int> sums;
  
  // read vals
  int val;
  cin >> val;
  sums.push_back(val);
  for (int i = 1; i < n; i++) {
    int val;
    cin >> val;
    val += sums[i-1];
    sums.push_back(val);
  }
  
  // solve
  int low = 0;
  int high = 0;
  
  int closest = abs(k-sums[0]);
  for (int i=0; i < n; i++) {
    for (int j=i; j < n; j++) {
      int sum_low = sums[i];
      int sum_high = sums[j];
      
      if (abs(k-(sum_high)) < closest) {
        int diff = abs(k-(sum_high));
        closest = diff;
        
        low = 0;
        high = j;
      
      } else if (abs(k-(sum_high-sum_low)) < closest) {
        // check closest
        int diff = abs(k-(sum_high-sum_low));
        closest = diff;
        
        low = i+1;
        high = j;
      }
    } 
  }
  
  cout << low << " " << high << endl;
  return 0;
}

int main() {
  int T;
  cin >> T;
  
  
  for (int t=0; t < T; t++) {
    solve_n2();
  }
  
  return 0;
}