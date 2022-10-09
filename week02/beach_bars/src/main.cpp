#include<iostream>
#include<vector>
#include <algorithm>
#include <cassert>

using namespace std;

int solve(int n) {
  vector<int> locations;
  
  // read in values
  for (int i=0; i<n; i++) {
    int loc;
    cin >> loc;
    locations.push_back(loc);
  }
  
  // sort locations
  sort(locations.begin(), locations.end());
  
  // init pointers and variables
  int best=0; int left=0; int count=0; 
  int dist=0; int lowest_dist=0;
  vector<int> best_locations;
  
  for (int right=0; right<n; right++) {
    int diff = locations[right] - locations[left];
    
    count++;
    
    // increase left
    while (diff > 200) {
      left++; count--;
      diff = locations[right] - locations[left];
    }
    
    // check if new best
    dist = diff / 2;
    if (count > best) {
      best=count;
      
      // update best locations and distance
      best_locations.clear();
      best_locations.push_back(dist + locations[left]);

      // add second locations for odd diff
      if (diff%2==1) { 
        dist++;
        best_locations.push_back(dist + locations[left]);
      };
      
      lowest_dist=dist;
      
    } else if (count == best) {
      // add location(s) for odd/even points
      if ((diff%2==1) & (dist + 1 <= lowest_dist)) {
          // remove all previous locations if new smallest distance
          if (dist + 1 < lowest_dist) {
            lowest_dist = dist+1;
            best_locations.clear();
          }
          
          best_locations.push_back(dist + locations[left]);
          best_locations.push_back(dist + 1 + locations[left]); 
      } else {
        if (dist <= lowest_dist) {
          // remove all previous locations if new smallest distance
          if (dist < lowest_dist) {
            lowest_dist = dist;
            best_locations.clear();
          }
          
          best_locations.push_back(dist + locations[left]);
        }
      }
    }
    
  }

  cout << best << " " << lowest_dist << endl;
  
  for (unsigned int i=0; i<best_locations.size(); i++) {
    cout << best_locations[i] << " ";
  }
  cout << endl;
  
  return 0;
}

int main() {
  int T;
  cin >> T;
  
  for (int t=0; t<T; t++) {
    int n;
    cin >> n;
    
    solve(n);
  }
  
  return 0;
}