#include <iostream>

using namespace std;

int sum(int n) {
  int s = 0;
  for (int i=0; i < n; i++) {
    int val;
    cin >> val;
    
    s += val;
  }
  
  cout << s << endl;
  return s;
}

int main() {
  int T;
  cin >> T;
  
  for (int t=0; t < T; t++) {
    int n;

    cin >> n;
    sum(n);
  }
  
  
  return 0;
}

