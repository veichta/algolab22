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

void solve(int n, int m) {
  // read input
  vector<pair<int, int>> nutrition_bounds;
  for (int i=0; i<n; i++) {
    int a, b; cin >> a >> b;
    nutrition_bounds.push_back(make_pair(a, b));
  }
  
  vector<int> prices;
  vector<vector<int>> food_info(m, vector<int>(n, 0));
  
  for (int i=0; i<m; i++) {
    int p; cin >> p;
    prices.push_back(p);
    
    for (int j=0; j<n; j++) {
      int val; cin >> val;
      food_info[i][j] = val;
    }
  }
  
  // LP
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  
  // constraints
  // -N_i <= min_i
  // N_i <= max_i
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      // -N_i = sum_j(-"Nutrition i in product j" * x_j)
      lp.set_a(j, 2*i, -food_info[j][i]);  
      
      // N_i = sum_j("Nutrition i in product j" * x_j)
      lp.set_a(j, 2*i+1, food_info[j][i]); 
    }
    // -N_i <= -min_i
    lp.set_b(2*i, -nutrition_bounds[i].first);  
    // N_i <= max_i
    lp.set_b(2*i+1, nutrition_bounds[i].second); 
  }

  // objective function: min sum_i(x_i * price_i)
  for (int i=0; i<m; i++) {
    lp.set_c(i, prices[i]);
  }
  
  // solve
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if (s.is_optimal()) {
    cout << floor(CGAL::to_double(s.objective_value())) << endl;
  } else {
    cout << "No such diet." << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int n, m; cin >> n >> m;
  
  while (n != 0 and m != 0) {
    solve(n, m);
    cin >> n >> m;
  }
}