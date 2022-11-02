#include<iostream>

#include<CGAL/QP_models.h>
#include<CGAL/QP_functions.h>
#include<CGAL/Gmpz.h>

using namespace std;


typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void solve(int n, int d) {
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  // constraints
  for (int i=0; i<n; i++) {
    int norm = 0;
    for(int j=1; j<=d; j++) {
      int aj; cin >> aj;
      norm += aj * aj;
      lp.set_a(j, i,  aj);
    }
    // + norm(a) * r
    norm = sqrt(norm);
    lp.set_a(0, i, norm);
    
    int b; cin >> b;
    lp.set_b(i, b);
  }
  
  // r >= 0
  lp.set_l(0, true, 0);
  
  // objective function: min -r
  lp.set_c(0, -1);
  
  // solve the program
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if (s.is_infeasible()) {
    cout << "none" << endl;
  } else if (s.is_unbounded()) {
    cout << "inf" << endl;
  } else {
    double r = CGAL::to_double(-s.objective_value());
    cout << floor(r) << endl;
  }
}

int main() {
  int n, d; cin >> n >> d;
  
  while(n != 0) {
    solve(n, d);
    cin >> n >> d;
  }
  return 0;
}