///3
#include<iostream>
#include<vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;


typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;



bool check_lp(const vector<vector<int>> stalls, int a, int p) {
  int n = stalls.size();
  Program lp (CGAL::SMALLER, true, 0, true, 1);

  // constraints  
  const int H1 = 0; 
  const int H2 = 1;
  const int H3 = 2;
  
  for (int i=0; i<n; i++) {
    int k_prime = stalls[i][2] + (a * a);
    int l_prime = stalls[i][3] + (p * p);
    int m_prime = stalls[i][4] + (a * p);
    lp.set_a(H1, i, -k_prime); lp.set_a(H2, i, -l_prime); lp.set_a(H3, i, -m_prime);
    lp.set_b(i, stalls[i][1] - stalls[i][0]);
  }

  // objective function
  lp.set_c(H1, 1);
  lp.set_c(H2, 1);
  lp.set_c(H3, 1);

  // solve the program
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  // std::cout << s; 
  
  return s.is_optimal();
}


void solve() {
  int n; cin >> n;
  vector<vector<int>> stalls(n, vector<int>(5, 0));
  for (int i=0; i<n; i++) {
    int f, c, k, l, m; cin >> f >> c >> k >> l >> m;
    stalls[i][0] = f;
    stalls[i][1] = c;
    stalls[i][2] = k;
    stalls[i][3] = l;
    stalls[i][4] = m;
  }
  
  vector<int> A(25, 0);
  for (int i=1; i<=24; i++) {
    int ai; cin >> ai;
    A[i] = ai + A[i-1];
  }
  
  vector<int> P(25, 0);
  for (int i=1; i<=24; i++) {
    int pi; cin >> pi;
    P[i] = pi + P[i-1];
  }
  
  int res = 45;
  for (int i=0; i<=4; i++) {
    for (int j=0; j<=4; j++) {
      bool success = check_lp(stalls, A[i], P[j]);
      if (success) {
        res = min(res, i + j);
      }
    }
  }
  
  if (res < 45) {
    cout << res << endl;
  } else {
    cout << "Impossible!" << endl;
  }
  
}


int main() {
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  
  while (T--) { solve(); }
  return 0;
}