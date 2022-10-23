///1
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

void solve() {
  int n; cin >> n;
  
  vector<pair<int, int>> ships;
  for(int i=0; i<n; i++) {
    int l, p; cin >> l >> p;
    ships.push_back(make_pair(p, l));
  }
  
  // sort by rings
  sort(ships.begin(), ships.end());
  
  // take first ship
  int count = 1;
  int right = ships[0].first;
  int end_ships = ships[0].first;
  
  for(int i=1; i<n; i++) {
    int ring = ships[i].first;
    int ship_length = ships[i].second;
    
    int right_end = max(end_ships + ship_length, ring);
    
    if (right_end < right) { // take smaller ship
      right = right_end;
    } else if (ring >= right) { // take new ship
      count++;
      end_ships = right;
      right = max(end_ships + ship_length, ring);
    }
  }
  cout << count << endl;
}

int main() {
  int T; cin >> T;
  while (T--) { solve(); }
  return 0;
}