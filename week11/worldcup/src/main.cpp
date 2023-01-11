#include<iostream>
#include<vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int floor_to_double( CGAL::Quotient<ET> x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (int) a;
}

void solve() {
  int n, m, C; cin >> n >> m >> C;
  
  vector<vector<int>> warehouse(n, vector<int>(4, 0));
  for (int i=0; i<n; i++) {
    int x, y, s, a; cin >> x >> y >> s >> a;
    warehouse[i][0] = x; warehouse[i][1] = y;
    warehouse[i][2] = s; warehouse[i][3] = a;
  }
  
  vector<vector<int>> stadium(m, vector<int>(4, 0));
  for (int i=0; i<m; i++) {
    int x, y, d, u; cin >> x >> y >> d >> u;
    stadium[i][0] = x; stadium[i][1] = y;
    stadium[i][2] = d; stadium[i][3] = u;
  }
  
  vector<vector<int>> revenues(n, vector<int>(m, 0));
  for(int w=0; w<n; w++) {
    for (int s=0; s<m; s++) {
      int r_ws; cin >> r_ws;
      revenues[w][s] = r_ws;
    }
  }
  
  
  vector<set<int>> contour_w(n);
  vector<set<int>> contour_s(m);
  for (int c=0; c<C; c++) {
    int x, y, r; cin >> x >> y >> r;
    K::Point_2 pc(x, y);
    
    for (int w=0; w<n; w++) {
      K::Point_2 pw(warehouse[w][0], warehouse[w][1]);
      if (r*r >= CGAL::squared_distance(pw, pc)) {
        contour_w[w].insert(c);
      }
    }
    for (int s=0; s<m; s++) {
      K::Point_2 ps(stadium[s][0], stadium[s][1]);
      if (r*r >= CGAL::squared_distance(ps, pc)) {
        contour_s[s].insert(c);
      }
    }
  }

  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  // objective function
  for(int w=0; w<n; w++) {
    for (int s=0; s<m; s++) {
      
      vector<int> c_int;
      set_intersection(
        contour_w[w].begin(), contour_w[w].end(),
        contour_s[s].begin(), contour_s[s].end(),
        std::back_inserter(c_int)
      );
      
      int reward = revenues[w][s] * 100;
      reward -= (contour_w[w].size() + contour_s[s].size() - 2 * c_int.size());
      lp.set_c(w*m + s, -reward);
    }
  }
  
  // constraint
  int cons_count = 0;
  
  // warehouse supply => sum_s x_ws <= supply(w)
  for(int w=0; w<n; w++) {
    for (int s=0; s<m; s++) {
      lp.set_a(w*m + s, cons_count, 1);
    }
    
    lp.set_b(cons_count, warehouse[w][2]);
    cons_count++;
  }
  
  // max alc => sum_w x_ws * alk(w) <= 100 * max_alc(s)
  for (int s=0; s<m; s++) {
    for(int w=0; w<n; w++) {
      lp.set_a(w*m + s, cons_count, warehouse[w][3]);
    }
    
    lp.set_b(cons_count, 100*stadium[s][3]);
    cons_count++;
  }
  
  // supply = demand => sum_w x_ws <= demand(s) & -sum_w x_ws <= -demand(s)
  for (int s=0; s<m; s++) {
    for(int w=0; w<n; w++) {
      lp.set_a(w*m + s, cons_count, 1);
      lp.set_a(w*m + s, cons_count + 1, -1);
    }
    
    lp.set_b(cons_count, stadium[s][2]);
    lp.set_b(cons_count + 1, -stadium[s][2]);
    cons_count += 2;
  }

  // solve LP
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if (s.is_infeasible()) {
    cout << "RIOT!" << endl;
  } else {
    cout << floor_to_double(-s.objective_value() / 100) << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while (T--) { solve(); }
  return 0;
}