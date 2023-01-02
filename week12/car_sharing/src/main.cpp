// Algoscore: C C C C C

// Carsharing solution using MinCostMaxFlow 
// without negative edges (SSPNW algorithm)
// and coordinate compression per station
// Score: 100

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <unordered_set>
#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef long long int in;
using namespace boost;
using namespace std;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor,
                property <edge_weight_t, long> > > > > Graph;

typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type       EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;
typedef graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator


struct EdgeAdder {
    EdgeAdder(Graph & G, EdgeCapacityMap &capacity, EdgeWeightMap &weight, ReverseEdgeMap &rev_edge) 
        : G(G), capacity(capacity), weight(weight), rev_edge(rev_edge) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, reverseE;
        tie(e, tuples::ignore) = add_edge(u, v, G);
        tie(reverseE, tuples::ignore) = add_edge(v, u, G);
        capacity[e] = c;
        weight[e] = w;
        capacity[reverseE] = 0;
        weight[reverseE] = -w;
        rev_edge[e] = reverseE; 
        rev_edge[reverseE] = e; 
    }
    Graph &G;
    EdgeCapacityMap &capacity;
    EdgeWeightMap &weight;
    ReverseEdgeMap  &rev_edge;
};

struct Booking {
    in s,t,d,a,p;
};

void testcase() {
    // Read the input
    in N,S; cin >> N >> S;
    // cout << N << " " << M << " " << S << endl;
    assert(1<=N && N<=10000); // bookings
    assert(2<=S && S<=10);   // states
    vector<in> L(S);
    const in MAXL = 100;
    for(in i=0; i<S; i++) {
        cin >> L[i];
        assert(0<=L[i] && L[i]<=100);
    }
    const in MAXT = 100000;
    const in MAXP = 100;
    vector<Booking> B; // bookings

    // Map for time coordinate compression
    vector<set<in>> times(S);
    for(in s=0; s<S; s++) {
        times[s].insert(0);
        times[s].insert(MAXT);
    }
    vector<map<in,in>> M(S);

    for(in i=0; i<N; i++) {
        in s,t,d,a,p;
        cin >> s >> t >> d >> a >> p;
        assert(1<=s && s<=S);
        assert(1<=t && t<=S);
        assert(0<=d && d<a);
        assert(a<=MAXT);
        assert(1<=p && p<=100);
        s--; t--;
        times[s].insert(d);
        times[t].insert(a);
        B.push_back({s,t,d,a,p});
    }

    vector<in> prefix_sum_of_lengths(S+1);
    for(in s=0; s<S; s++) {
        in i=0;
        for(auto &t : times[s]) {
            M[s][t]=i;
            i++;
        }
        prefix_sum_of_lengths[s+1] = prefix_sum_of_lengths[s] + M[s].size();
    }

    // Build the graph
    in T = prefix_sum_of_lengths.back(); // number of timesteps
    in v_source = T;
    in v_target = T+1;
    Graph G(T+2);
    EdgeCapacityMap capacity = get(edge_capacity, G);
    EdgeWeightMap weight = get(edge_weight, G);
    ReverseEdgeMap rev_edge = get(edge_reverse, G);
    ResCapacityMap res_capacity = get(edge_residual_capacity, G);
    EdgeAdder ea(G, capacity, weight, rev_edge);
    
    // Add the edges
    for(in i=0; i<S; i++) {
        // source to first station node: buy at most one site, no cost
        ea.addEdge(v_source,prefix_sum_of_lengths[i],L[i],0);
        // last station node to sink
        ea.addEdge(prefix_sum_of_lengths[i+1]-1, v_target, MAXL*S,0);
        // along each station
        in it = -1;
        in lastt = 0;
        for (auto& t : times[i]) {
            if(it != -1) {
                ea.addEdge(prefix_sum_of_lengths[i] + it, 
                    prefix_sum_of_lengths[i] + it + 1, 
                    MAXL*S, MAXP*(t-lastt));
            }
            it++;
            lastt = t;
        }
    }

    // add booking edges
    for(in i=0; i<N; i++) {
        ea.addEdge(prefix_sum_of_lengths[B[i].s] + M[B[i].s][B[i].d], 
            prefix_sum_of_lengths[B[i].t] + M[B[i].t][B[i].a],
            1, 
            ((B[i].a-B[i].d)*MAXP)-B[i].p);
    }

    // Run the algorithm
    successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    in flow = 0;
    // Iterate over all edges leaving the source
    OutEdgeIt e, eend;
    for(tie(e, eend) = out_edges(vertex(v_source,G), G); e != eend; ++e) {
        flow += capacity[*e] - res_capacity[*e];
    }    
    in cost = MAXP*MAXT*flow-find_flow_cost(G);
    cout << cost << endl;
}
int main() {
    in T; cin >> T;
    while(T--) testcase();
}
