///
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;


long defuse(
  const vector<long> &bombs, 
  vector<bool> &defused,
  long n_defused,
  long bomb_idx
) {
  long n = bombs.size();
  
  if (bomb_idx >= n) {
    return n_defused;
  } else if (defused[bomb_idx]) {
    return n_defused;
  }
  
  long child1 = bomb_idx * 2 + 1;
  if (child1 < n) {
    n_defused = defuse(bombs, defused, n_defused, child1);
  }
  long child2 = bomb_idx * 2 + 2;
  if (child2 < n) {
    n_defused = defuse(bombs, defused, n_defused, child2);
  }
  
  n_defused++;
  defused[bomb_idx] = true;
  
  // cout << "Defusing " << bomb_idx << " at time " << n_defused << endl;
  return n_defused;
}

void solve() {
  long n; cin >> n;
  
  vector<long> bombs;
  vector<pair<long, long>> pi;
  for (long i=0; i<n; i++) {
    long val; cin >> val;
    bombs.push_back(val);
    pi.push_back(make_pair(val, i));
  }
  
  sort(pi.begin(), pi.end());
  
  // solve
  bool kabooom = false;
  vector<bool> defused(n, false);
  long n_defused = 0;
  for(long i=0; i<n; i++) {
    pair<long, long> current_bomb = pi[i];
    long bomb_idx = current_bomb.second;
    
    if (not defused[bomb_idx]) {
      // defuse bombs below
      long child1 = bomb_idx * 2 + 1;
      n_defused = defuse(bombs, defused, n_defused, child1);
      
      long child2 = bomb_idx * 2 + 2;
      n_defused = defuse(bombs, defused, n_defused, child2);
      
      // defuse bomb
      
      n_defused++;
      // cout << "Defusing " << bomb_idx << " at time " << n_defused << endl;
      if (n_defused > current_bomb.first) {
        kabooom = true;
      } else {
        defused[bomb_idx]=true;
      }
    }
  }
  
  if (kabooom) {
    cout << "no" << endl;
  } else {
    cout << "yes" << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  long T; cin >> T;
  while (T--) { solve(); }
  return 0;
}