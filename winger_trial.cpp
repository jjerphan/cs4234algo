#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> ii;
typedef vector<int> vi;
typedef vector<vi> vvi;

class MaxFlow {
 private:
    class Edge {
     public:
        int cap;
        int orig_cap;
        int flow;
        Edge():cap(0), orig_cap(0), flow(0) {}

        explicit Edge(int _cap): cap(_cap), orig_cap(_cap), flow(0) {}

        void reset() { cap = orig_cap ; return;}
    };

    int N;
    map<ii, Edge> edges;
    vvi neighs;

    int source;
    int target;


    void push_flow(int u, int v, int flow) {
        edges[make_pair(u, v)].flow += flow;
        edges[make_pair(u, v)].cap -= flow;

        edges[make_pair(v, u)].flow -= flow;
        edges[make_pair(v, u)].cap += flow;
    }

    // Used for Ford Fulkerson
    int DFS(int u, int bottleneck, vector<bool>& visited) {
        visited[u] = true;

        // If we get here, there is an augmenting path and we return the
        // value of the flow that is possible to make
        if (u == target) {
            return bottleneck;
        }

        for (int &v: neighs[u]) {
            if (!visited[v]) {
                int cap_edge = edges[make_pair(u, v)].cap;

                // If we can advance, we try to get a new flow
                if (cap_edge > 0) {
                    int new_flow = DFS(v, min(bottleneck, cap_edge), visited);

                    if (new_flow !=0) {
                        push_flow(u, v, new_flow);
                        return new_flow;
                    }
                }
            }
        }
        // If we get here, there is no augmenting path from u
        return 0;
    }

 public:
    explicit MaxFlow(int _N):N(_N), neighs(_N, vi()),
    source(0), target(_N - 1) { }

    MaxFlow& add_edge(int u, int v, int cap) {
        neighs[u].push_back(v);
        neighs[v].push_back(u);

        edges[make_pair(u, v)] = Edge(cap);
        edges[make_pair(v, u)] = Edge(0);

        return *this;
    }

    void print_internals() {
        cout << "Edge: Flow\n";
        for(pair<const ii, Edge> & e: edges) {
            cout << e.first.first << " → " << e.first.second
            << ": " << e.second.cap << "\n";
        }
        cout << "\n";
    }

    int ford_fulkerson() {
        int max_f = 0;
        int source = 0;
        while (1) {
            vector<bool> visited(N, false);
            int new_flow = DFS(source, INT_MAX, visited);

            if (new_flow == 0) break;

            max_f += new_flow;
        }

        return max_f;
    }

    int edmonds_karp() {
        int max_f = 0;

        class State {
         public:
            vi path;
            vector<bool> visited;
            int bottleneck;

            explicit State(int N) : visited(N, false) { }
        };

        bool improvement_possible = true;
        while (improvement_possible) {
            improvement_possible = false;
            list<State > queue;

            State st(N);
            st.path.push_back(source);
            st.bottleneck = INT_MAX;
            queue.push_back(st);

            // BFS
            while (!queue.empty()) {
                State state = queue.front();
                queue.pop_front();

                int u = state.path.back();
                state.visited[u] = true;

                // At the end, we update the edges
                if (u == target) {
                    for (int i = 0 ; i < state.path.size()-1 ; i ++) {
                        push_flow(state.path[i], state.path[i+1],
                        state.bottleneck);
                    }
                    max_f += state.bottleneck;
                    improvement_possible = true;
                    break;
                }

                // For each new unvisited neighs, we try to find an
                // augmenting path
                for (int &v: neighs[u]) {
                    if (!state.visited[v]) {
                        State new_state = state;
                        int cap_edge = edges[make_pair(u, v)].cap;
                        if (cap_edge > 0) {
                            new_state.path.push_back(v);
                            new_state.bottleneck = min(state.bottleneck,
                            cap_edge);
                            queue.push_back(new_state);
                        }
                    }
                }
            }
        }
        return max_f;
    }

    int push_relabel(int _source, int _target) {

        source = _source;
        target = _target;

        // Excess on vertices
        vi x(N, 0);
        // Heights of vertices
        vi h(N, 0);

        h[source] = N;

        set<int> unbalanced;

        // Quick start by pushing as much as possible from the source
        for (int &v : neighs[source]) {
            int cap_edge = edges[make_pair(source, v)].cap;
            if (cap_edge > 0) {
                push_flow(source, v, cap_edge);
                x[v] = cap_edge;

                // Target isn't to balance
                if (v != target)
                    unbalanced.insert(v);
            }
        }

        while (!unbalanced.empty()) {
            set<int> new_unbalanced;
            for (const int &u : unbalanced) {
                bool can_push = false;

                // Push
                for (int &v : neighs[u]) {
                    int cap_edge = edges[make_pair(u, v)].cap;
                    if (h[u] > h[v] && cap_edge > 0) {
                        can_push = true;
                        int bottleneck = min(x[u], cap_edge);
                        push_flow(u, v, bottleneck);
                        x[u] -= bottleneck;
                        x[v] += bottleneck;

                        // Source and target aren't to balance
                        if (v != source && v != target) {
                            new_unbalanced.insert(v);
                        }
                    }
                }

                // Relabel
                if (!can_push) {
                    h[u]++; // simple update strategy, can be improved
                }
                if (x[u] > 0) {
                    new_unbalanced.insert(u);
                }
            }
            unbalanced = new_unbalanced;
        }

        return x[target];
    }

    // Reset the graph to rerun the algorithm
    void reset() {
        for (pair<const ii, Edge>& e: edges) {
            e.second.reset();
        }
    }
};


int L, W, N, d;

int xs[1000];
int ys[1000];

int main() {
    int cas = 0;
    while (1) {
        cas++;
        cin >> L >> W >> N >> d;

        if (L + W + N + d == 0) {
            return 0;
        }
        cout << "Case " << cas << ": ";

        for (int i = 0; i < N; i++) {
            cin >> xs[i] >> ys[i];
        }

        MaxFlow solver(N+2);
        int source = N;
        int target = N + 1;

        for (int i = 0; i < N ; i++) {
            if (ys[i] <= d) {
                solver.add_edge(i, target, 1);
            }
            if (abs(W - ys[i]) <= d) {
                solver.add_edge(source, i, 1);
            }

            for (int j = i+1 ; j < N ; j++) {
                int diff_x = xs[i] - xs[j];
                int diff_y = ys[i] - ys[j];
                int dist = sqrt(diff_x * diff_x + diff_y * diff_y);
                // cout << i << " " << j << " d : " << dist << "\n";
                if (dist <= 2 * d) {
                    // Connecting from top to bottom
                    // Order : Top to bottom, if equals then left to right
                    if (diff_y > 0) {
                        if (ys[i] < ys[j]) {
                            solver.add_edge(i, j, 1);
                        } else {
                            solver.add_edge(j, i, 1);
                        }
                    } else {  // if on same height, compare using length
                        if (xs[i] < xs[j]) {
                            solver.add_edge(i, j, 1);
                        } else {
                            solver.add_edge(j, i, 1);
                        }
                    }
                }
            }
        }

        cout << solver.push_relabel(source, target) << "\n";

    }
    return 0;
}
