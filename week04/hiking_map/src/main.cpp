///1
// std
#include<iostream>
#include<vector>


// cgal
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Triangle_2 T;

using namespace std;

// ################## INIT ##################
void read_path(vector<pair<P, P>> &path, int m) {
  int x; int y; cin >> x >> y;
  P u = P(x, y);
  
  for (int i=1; i<m; i++) {
    cin >> x >> y;
    P v = P(x, y);
    path.push_back(make_pair(u, v));
    
    u = v;
  }
}

void read_triangle(T &part) {
  int q0; int q1; int q2; int q3;
  P u; P v;
  
  cin >> q0 >> q1 >> q2 >> q3;
  u = P(q0, q1); v = P(q2, q3);
  L l1 = L(u, v);
  
  cin >> q0 >> q1 >> q2 >> q3;
  u = P(q0, q1); v = P(q2, q3);
  L l2 = L(u, v);
  
  cin >> q0 >> q1 >> q2 >> q3;
  u = P(q0, q1); v = P(q2, q3);
  L l3 = L(u, v);
  
  auto inter1 = CGAL::intersection(l1, l2);
  auto inter2 = CGAL::intersection(l1, l3);
  auto inter3 = CGAL::intersection(l2, l3);
  
  if (const P* op1 = boost::get<P>(&*inter1)) {
    if (const P* op2 = boost::get<P>(&*inter2)) {
      if (const P* op3 = boost::get<P>(&*inter3)) {
        part = T(*op1, *op2, *op3);
      }
    }
  }
}

void read_triangles(vector<T> &map_parts, int n) {
  for (int i=0; i<n; i++) {
    T part;
    read_triangle(part);
    map_parts.push_back(part);
  }
}

// ################## SOLUTION ##################
bool check_seg_in_tri(const T &triangle, pair<P, P> segment) {
  int in1 = triangle.bounded_side(segment.first);
  int in2 = triangle.bounded_side(segment.second);
  
  if ((in1 >= 0) & (in2 >= 0)) {
    return true;
  } else {
    return false;
  }
}

void solve(int n, int m) {
  // read inputs
  vector<pair<P, P>> path;
  read_path(path, m);
  
  vector<T> map_parts;
  read_triangles(map_parts, n);
  
  // init helper variables and pointer
  vector<int> counts(m-1, 0);
  
  bool all_inside=false;
  int count_inside = 0;
  
  int left = 0; int right=0;
  
  // ################## Init right pointer ##################
  while (!all_inside & (right < n)) {
    for(int seg_idx=0; seg_idx<m-1; seg_idx++) {
      if (check_seg_in_tri(map_parts[right], path[seg_idx])) {
        counts[seg_idx]++;
        if (counts[seg_idx] == 1) {
          count_inside++;
        }
      }
    }
    if (count_inside == m-1) {
      all_inside=true;
    }
    right++;
  }
  
  // move left pointer as far as possible
  int res = right;
  // cout << "R: " << right << " | res: " << res << endl;
  
  while(all_inside) {
    for(int seg_idx=0; seg_idx<m-1; seg_idx++) {
      if (check_seg_in_tri(map_parts[left], path[seg_idx])) {
        if (counts[seg_idx] == 1) {
          res = min(res, right-left);
          // cout << "L: " << left << " | res: " << res << endl;
          all_inside=false;
        }
        counts[seg_idx]--;
      }
    }
    left++;
  }
  
  // cout << "L: " << left << " | res: " << res << endl;

  // ################## find smallest sequence ##################
  // cout << "###################" << endl;
  while(right < n-1) {
    // move left pointer as far as possible
    while(all_inside) {
      // cout << "L: " << left-1 << " R: " << right << " | res: " << res << endl;
      for(int seg_idx=0; seg_idx<m-1; seg_idx++) {
        if (check_seg_in_tri(map_parts[left], path[seg_idx])) {
          if (counts[seg_idx] == 1) {
            res = min(res, right-left+1);
            // cout << "L: " << left << " R: " << right << " | res: " << res << endl;
            all_inside=false;
          }
          counts[seg_idx]--;
        }
      }
      left++;
    }
    
    right++;
    
    all_inside=true;
    for (int seg_idx=0; seg_idx<m-1; seg_idx++) {
      if (check_seg_in_tri(map_parts[right], path[seg_idx])) {
        counts[seg_idx]++;
      }
      if (counts[seg_idx] == 0) {
        all_inside=false;
      }
    }
  }
  cout << res << endl;
}

int main() {
  int T; cin >> T;
  
  for (int t=0; t<T; t++) {
    int m; int n; cin >> m >> n;
    
    solve(n, m);
  }
  return 0;
}