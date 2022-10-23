#include<iostream>
#include<vector>
#include<set>
#include<algorithm>

using namespace std;


void solve() {
  int n, m; cin >> n >> m;
  
  int strongest_friend = 0;
  vector<int> friends(n, 0);
  for (int i=0; i<n; i++) {
    cin >> friends[i];
    strongest_friend = max(strongest_friend, friends[i]);
  }
  
  int heaviest_box = 0;
  multiset<int> boxes;
  for (int i=0; i<m; i++) {
    int val; cin >> val;
    boxes.insert(val);
    
    heaviest_box = max(heaviest_box, val);
  }
  
  sort(friends.begin(), friends.end());

  if (strongest_friend < heaviest_box) {
    cout << "impossible" << endl;
    return;
  }
  
  int turns = 0;
  while(boxes.size() > 0) {
    turns++;
    for (int i=n-1; i>=0; i--) {
      int f = friends[i];

      auto it = boxes.upper_bound(f);
      if (it != boxes.begin()) {
        it--;
        boxes.erase(it);
      } else {
        break;
      }
    }
  }
  
  cout << (turns * 3 - 1) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int T; cin >> T;
  while(T--) { solve(); }
  
  return 0;
}