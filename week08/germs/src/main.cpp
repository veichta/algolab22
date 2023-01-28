#include<CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/Delaunay_triangulation_2.h>
#include<CGAL/Triangulation_vertex_base_with_info_2.h>
#include<CGAL/Triangulation_face_base_2.h>
#include<bits/stdc++.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt SQRT_K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef Triangulation::Edge_iterator                           Edge_iterator;
typedef K::Point_2                                             P;
typedef std::pair<P, Index>                                    IPoint;


using namespace std;


long floor_to_long(const SQRT_K::FT& x) {
  long a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}


void solve(int N) {
  int L, B, R, T; cin >> L >> B >> R >> T;
  
  vector<IPoint> germs(N);
  vector<SQRT_K::FT> death(N, LONG_MAX);
  
  // read points
  for (int i = 0; i < N; ++i) {
    long x, y; cin >> x >> y;
    germs[i] = make_pair(P(x, y), i);
    
    SQRT_K::FT dist = pow(x - L, 2);
    death[i] = min(death[i], dist);
    
    dist = pow(x - R, 2);
    death[i] = min(death[i], dist);
    
    dist = pow(y - T, 2);
    death[i] = min(death[i], dist);
    
    dist = pow(y - B, 2);
    death[i] = min(death[i], dist);
  }
  
  Triangulation t;
  t.insert(germs.begin(), germs.end());
  
  // output all edges
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    
    P p1 = e->first->vertex((e->second+1)%3)->point();
    P p2 = e->first->vertex((e->second+2)%3)->point();
    
    
    SQRT_K::FT r = CGAL::squared_distance(p1, p2) / 4;
    
    death[i1] = min(death[i1], r);
    death[i2] = min(death[i2], r);
  }
  
  sort(death.begin(), death.end());
  
  SQRT_K::FT first_death = CGAL::sqrt(CGAL::sqrt(death[0]) - 0.5);
  SQRT_K::FT half_death = CGAL::sqrt(CGAL::sqrt(death[N/2]) - 0.5);
  SQRT_K::FT last_death = CGAL::sqrt(CGAL::sqrt(death[N-1]) - 0.5);
  
  
  cout << -floor_to_long(-first_death) << " " 
       << -floor_to_long(-half_death) << " " 
       << -floor_to_long(-last_death) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int N; cin >> N;
  while (N) {solve(N); cin >> N;}
  return 0;
}