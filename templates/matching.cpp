#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> ii;
typedef vector<int> vi;
typedef vector<vi> vvi;


class MCBM {
 private:
    vector<int > matched;
    vvi neighs;
    int N;
    int L;

    void match(int u, int v) {
        matched[u] = v;
        matched[v] = u;
    }

    void unmatch(int u, int v) {
        matched[u] = -1;
        matched[v] = -1;
    }


 public:
    MCBM& add_edge(int u, int v) {
        neighs[u].push_back(v);
        neighs[v].push_back(u);
        return *this;
    }


    MCBM(int _N, int _L) : N(_N), L(_L), matched(N, -1), neighs(N, vi()) { }

    void print_internals() {
        for (int u = 0 ; u < N ; u++) {
            cout << u << " " << matched[u] << "\n";
        }
        cout << "\n";
    }

    int augmenting_path(int u, vector<bool >& visited) {
        // return 1 if  an augmenting path from L
        if (visited[u]) {
            return 0;
        }
        visited[u] = true;
        for (auto &v : neighs[u]) {
            if (matched[v] == -1 || augmenting_path(matched[v], visited)) {
                match(v, u);
                return 1;
            }
        }
        return 0;
    }

    unordered_set<int > greedy_random_matching() {
        int nb_match = 0;
        unordered_set<int > unmatched;
        for (int u = 0; u < L ; u++) {
            for(int & v : neighs[u]) {
                if (matched[v] == -1) {
                    match(u, v);
                    nb_match++;
                    break;
                }
            }
            if(matched[u] == -1) {
                unmatched.insert(u);
            }
        }
        return unmatched;
    }

    int perform_matching() {
        unordered_set<int > unmatched = greedy_random_matching();
        int card = L - unmatched.size();
        for (const int i : unmatched) {
            vector<bool > visited(N, false);
            card += augmenting_path(i, visited);
        }


        return card;
    }

    void reset() {
        matched = vector<int>(N, -1);
    }

};


int main() {
    int N = 16;
    int L = 7;
    MCBM solver(N, L);

    solver.add_edge(0, 8);
    solver.add_edge(0, 10);
    solver.add_edge(0, 15);
    solver.add_edge(1, 8);
    solver.add_edge(2, 8);
    solver.add_edge(2, 9);
    solver.add_edge(2, 11);
    solver.add_edge(3, 10);
    solver.add_edge(4, 10);
    solver.add_edge(4, 12);
    solver.add_edge(5, 11);
    solver.add_edge(5, 12);
    solver.add_edge(6, 11);
    solver.add_edge(6, 12);
    solver.add_edge(6, 13);
    solver.add_edge(7, 8);

    cout << "Size Bipartite Matching : " << solver.perform_matching() << "\n";
    solver.print_internals();

    return 0;
}
