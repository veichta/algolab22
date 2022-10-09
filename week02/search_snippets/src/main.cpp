#include<iostream>
#include<vector>
#include<algorithm>
#include <climits>

using namespace std;

bool check_seq(vector<int> &seq) {
  //check if all words are in the sequence
  for (unsigned int i=0; i<seq.size(); i++) {
    if (seq[i] <= 0) {
      return false;
    }
  }
  return true;
}

int solve(int n) {
  vector<int> word_counts(n, 0);
  
  // read wordcounts
  for (int i=0; i<n; i++) {
    int count; cin >> count;
    word_counts[i] = count;
  }
  
  // read word positions into vector of pairs
  int max_pos = 0;
  vector<int> first_occurance(n, INT_MAX);
  vector<pair<int, int>> word_positions;
  
  for (int word_idx=0; word_idx<n; word_idx++) {
    for (int pos_idx=0; pos_idx<word_counts[word_idx]; pos_idx++) {
      int pos; cin >> pos;
      max_pos = max(max_pos, pos);
      word_positions.push_back(make_pair(pos, word_idx));
      
      // keep track of first occurances per word
      first_occurance[word_idx] = min(first_occurance[word_idx], pos);
    }
  }
  
  // last first occurance
  int res = n;
  for (int i=0; i<n; i++) {
    res = max(res, first_occurance[i]);
  }

  // sort words on position
  sort(word_positions.begin(), word_positions.end());
  
  // solve
  unsigned int l = 0;
  vector<int> in_seq(n, 0);
  
  // move right to the last first occurance
  int r_init = 0;
  while (word_positions[r_init].first < res) {
    in_seq[word_positions[r_init].second]++;
    r_init++;
  }
  
  for (unsigned int r=r_init; r<word_positions.size(); r++) {
    in_seq[word_positions[r].second]++;

    // check if sequence contains all words
    if (in_seq[word_positions[r].second] == 1) {
      
      // try to increase left as much as possible
      while (l < r) {
        res = min(res, word_positions[r].first - word_positions[l].first + 1);

        in_seq[word_positions[l].second]--;
        l++;
        
        if (in_seq[word_positions[l-1].second] <= 0) { break; }
      }
    }
  }
  return res;
}

int main() {
  int T; cin >> T;
  
  for (int t=0; t<T; t++) {
    int n; cin >> n;
    
    int res = solve(n);
    cout << res << endl;
  }
  return 0;
}