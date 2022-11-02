#include<iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;


typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void solve1(int a, int b) { 
  // maximize & round down
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  const int X = 0; 
  const int Y = 1;
  
  // constraints
  // x + y  <= 4
  lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);  
  // 4x + 2y <= ab
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b); 
  // -x + y <= 1
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);  
  
  // objective function: -(by - ax)
  lp.set_c(Y, -b); lp.set_c(X, a);

  // solve
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if (s.is_infeasible() or s.is_unbounded()) {
    cout << "no" << endl;
  } else {
    double res = CGAL::to_double(s.objective_value());
    cout << floor(-res) << endl;
  }
}

void solve2(int a, int b) {
  // minimize & round up
  Program lp (CGAL::SMALLER, false, 0, true, 0);
  
  const int X = 0; 
  const int Y = 1;
  const int Z = 2;
  
  // constraints
  //  -x - y  <= 4
  lp.set_a(X, 0,  -1); lp.set_a(Y, 0, -1); lp.set_b(0, 4);                      
  // -4x - 2y - z <= ab
  lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b); 
  // x - 2y <= 1
  lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1);                        
  
  // objective: ax + by + z
  lp.set_c(Y, b); lp.set_c(X, a); lp.set_c(Z, 1);                               


  // solve
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if (s.is_infeasible() or s.is_unbounded()) {
    cout << "no" << endl;
  } else {
    double res = CGAL::to_double(s.objective_value());
    cout << ceil(res) << endl;
  }
}


int main() {
  int task; cin >> task;
  
  while(task != 0) {
    int a, b; cin >> a >> b;

    if (task == 1) {
      solve1(a, b);
    } else {
      solve2(a, b);
    }

    cin >> task;
  }
  
  return 0;
}