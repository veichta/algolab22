#include<iostream>
#include<vector>

#include<CGAL/QP_models.h>
#include<CGAL/QP_functions.h>
#include<CGAL/Gmpq.h>

using namespace std;


typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


double floor_to_double(const CGAL::Quotient<ET> &x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

long solve() {
  // init
  long n, m, h, w; cin >> n >> m >> h >> w;
  
  // init nails
  vector<pair<long, long>> nails;
  for (long i=0; i<n; i++) {
    long x, y; cin >> x >> y;
    nails.push_back(make_pair(x, y));
  }
  
  // init posters
  vector<pair<long, long>> posters;
  for (long j=0; j<m; j++) {
    long x, y; cin >> x >> y;
    posters.push_back(make_pair(x, y));
    
  }
  
  // find nearest poster for each nail
  vector<long> nearest(n, 0);
  for (int i=0; i<n; i++) {
    long closest = LONG_MAX;
    for (int j=0; j<m; j++) {
      long distance = max(
        abs(posters[j].first - nails[i].first) * h,
        abs(posters[j].second - nails[i].second) * w
      );
      
      if (distance < closest) {
        nearest[i] = j;
        closest=distance;
      }
    }
  }
  
  // LP
  Program lp (CGAL::SMALLER, true, 1, false, 0);
  
  // objective: -sum(a_i * (2w + 2h))
  for (long i=0; i<n; i++) {
    lp.set_c(i, -2*(w + h));
  }
  
  // constraints
  long cons_id = 0;
  for (long i=0; i<n; i++) {
    // constraints nails
    for (long j=0; j<n; j++) {
      if (i != j) {
        long x1 = nails[i].first;
        long y1 = nails[i].second;
        long x2 = nails[j].first;
        long y2 = nails[j].second;
        
        // |x1 - x2| / w >= |y1 - y2| / h <=> |x1 - x2| * h >= |y1 - y2| * w
        if (abs(x1-x2) * h >= abs(y1-y2) * w) {
          // intersection if x overlap
          long rhs = 2 * abs(x1-x2);
          lp.set_a(i, cons_id, w); lp.set_a(j, cons_id, w); lp.set_b(cons_id, rhs);
        } else {
          // intersection if y overlap
          long rhs = 2 * abs(y1-y2);
          lp.set_a(i, cons_id, h); lp.set_a(j, cons_id, h); lp.set_b(cons_id, rhs);
        }
        cons_id++;
      }
    }
    
    if (m == 0) continue;
    // constraint existing posters
    long x1 = nails[i].first;
    long y1 = nails[i].second;
    long x2 = posters[nearest[i]].first;
    long y2 = posters[nearest[i]].second;
    
    if (abs(x1-x2) * h >= abs(y1-y2) * w) {
      // longersection if x overlap
      long rhs = 2 * abs(x1-x2) - w;
      // lp.set_a(i, cons_id, w); lp.set_b(cons_id, rhs);
      lp.set_u(i, true, ET(rhs, w));
    } else {
      // intersection if y overlap
      long rhs = 2 * abs(y1-y2) - h;
      // lp.set_a(i, cons_id, h); lp.set_b(cons_id, rhs);
      lp.set_u(i, true, ET(rhs, h));
    }
    cons_id++;
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));

  if (s.is_optimal()) {
    return -floor_to_double(s.objective_value());
  } else {
    cout << s;
  }

  return -1;
}

int main() {
  ios_base::sync_with_stdio(false);
  long T; cin >> T;
  while (T--){ cout << solve() << endl; }
  return 0;
}