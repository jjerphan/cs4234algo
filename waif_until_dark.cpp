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

    int push_relabel() {
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


int main() {
    int n_children, n_toys, n_categ;
    scanf("%d", &n_children);
    scanf("%d", &n_toys);
    scanf("%d", &n_categ);

    int N = n_children + n_toys + n_categ + 2 ;

    MaxFlow solver(N);
    int source = 0;
    int target = N - 1;

    // Offset: children numbered starting at 1
    // Offet: categories starting at 0
    int offset_toy = n_children;
    int offset_categ = offset_toy + n_toys + 1 ;

    int nb_toy_child = 0;
    int toy = 0;

    // Source to children, children to toys
    for(int child = 1 ; child < n_children + 1; child++) {
        solver.add_edge(source, child, 1);
        scanf("%d", &nb_toy_child);
        for(int i = 0; i < nb_toy_child; i ++) {
            scanf("%d", &toy);
            solver.add_edge(child, toy + offset_toy, 1);
        }
    }

    int nb_toy_categ = 0;
    int cap = 0;

    // Toys assigend to a category
    unordered_set<int > toy_assigned;

    // Toys to categories
    for (int categ = 0; categ < n_categ ; categ++) {
        scanf("%d", &nb_toy_categ);
        for (int i = 0 ; i < nb_toy_categ ; i ++) {
            scanf("%d", &toy);
            toy_assigned.insert(toy);
            solver.add_edge(offset_toy + toy, offset_categ + categ, 1);
        }
        scanf("%d", &cap);
        solver.add_edge(offset_categ + categ, target, cap);
    }

    // Remaining toys to the target directly
    for (int toy = 1; toy < n_toys + 1; toy++) {
        if(toy_assigned.find(toy) == toy_assigned.end()) {
            solver.add_edge(toy + offset_toy, target, 1);
        }
    }

    cout << solver.push_relabel() << "\n";

    return 0;
}
