#include<bits/stdc++.h>

#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2    P;

using namespace std;

int N, M; 

void read_legs(vector<vector<P>> &legs) {
  int x, y; 
  P start, end;
  
  cin >> x >> y;
  start = P(x, y);
  
  for (int i=0; i<M-1; i++) {
    cin >> x >> y;
    end = P(x, y);
  
    legs[i] = {start, end};
  
    start = end;
  }
}


bool inside(const P &point, const vector<vector<P>> &lines) {
  // point is inside of the triangle iff it is on the side same side as the other
  // lines or it is collinear with a line
  
  for (int i=0; i<3; i++) {
    P on_correct_side = lines[(i+1) % 3][0];
    
    // try to minimize this check as it takes the most time for some reason
    auto correct = CGAL::orientation(lines[i][0], lines[i][1], on_correct_side);
    auto orientation = CGAL::orientation(lines[i][0], lines[i][1], point);

    if ( (orientation != correct) and (orientation != 0) ) {
      return false;
    }
  }
  return true;
}


void find_legs_in_triangle(vector<vector<int>> &triangles, const vector<vector<P>> &legs) {
  int x1, y1; 
  int x2, y2; 
  for (int i=0; i<N; i++) {
    // read lines
    vector<vector<P>> lines(3, vector<P>());
    for (int l=0; l<3; l++) {
      cin >> x1 >> y1;
      cin >> x2 >> y2;
      lines[l] = {P(x1, y1), P(x2, y2)};
    }
    
    // check for each leg if it is inside
    for (int leg=0; leg<M; leg++) {
      P start = legs[leg][0];
      P end = legs[leg][1];
      
      if (inside(start, lines) and inside(end, lines)) {
        triangles[i].push_back(leg);
      }
    }
  }
}


void solve() {
  cin >> M >> N;
  
  vector<vector<P>> legs(M, {P(0, 0), P(0, 0)});
  read_legs(legs);
  
  vector<vector<int>> triangles(N, vector<int>());
  find_legs_in_triangle(triangles, legs);
  
  int res = N;
  int n_zeros = M;
  
  // use deque to represent the window
  deque<int> q = {};
  
  // keep track which legs are covered how often
  vector<int> counts(M, 0);
  
  // sliding window over triangles
  for (int i=0; i<N; i++) {
    vector<int> curr_t = triangles[i];
    
    // increment count for all legs inside the triangle
    for (int leg : curr_t) {
      counts[leg]++;
      if (counts[leg] == 1) {n_zeros--;}
    }
    
    q.push_back(i);
    
    // check if all legs covered and remove beginning until not all covered
    while (n_zeros == 0) {
      res = min(res, int(q.size()));
      
      vector<int> pop_t = triangles[q.front()];
      q.pop_front();
      
      for (int leg : pop_t) {
        counts[leg]--;
        if (counts[leg] == 0) { n_zeros++; }
      }
    }
  }
  
  cout << res << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
  return 0;
}