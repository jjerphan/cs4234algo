#include <bits/stdc++.h>

using namespace std;

const int INF = 2000000000;

struct Edge {
	int from, to, cap, flow, index;
	Edge(int from, int to, int cap, int flow, int index) :
		from(from), to(to), cap(cap), flow(flow), index(index) {}
};

struct Dinic {
	int N;
	vector<vector<Edge> > G;
	vector<Edge *> dad;
	vector<int> Q;

	Dinic(int N) : N(N), G(N), dad(N), Q(N) {}

	void AddEdge(int from, int to, int cap) {
		G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
		if (from == to) G[from].back().index++;
		G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
	}

	long long BlockingFlow(int s, int t) {
		fill(dad.begin(), dad.end(), (Edge *) NULL);
		dad[s] = &G[0][0] - 1;

		int head = 0, tail = 0;
		Q[tail++] = s;
		while (head < tail) {
			int x = Q[head++];
			for (int i = 0; i < G[x].size(); i++) {
	Edge &e = G[x][i];
	if (!dad[e.to] && e.cap - e.flow > 0) {
		dad[e.to] = &G[x][i];
		Q[tail++] = e.to;
	}
			}
		}
		if (!dad[t]) return 0;

		long long totflow = 0;
		for (int i = 0; i < G[t].size(); i++) {
			Edge *start = &G[G[t][i].to][G[t][i].index];
			int amt = INF;
			for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
	if (!e) { amt = 0; break; }
	amt = min(amt, e->cap - e->flow);
			}
			if (amt == 0) continue;
			for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
	e->flow += amt;
	G[e->to][e->index].flow -= amt;
			}
			totflow += amt;
		}
		return totflow;
	}

	long long GetMaxFlow(int s, int t) {
		long long totflow = 0;
		while (long long flow = BlockingFlow(s, t))
			totflow += flow;
		return totflow;
	}
};

class index_mapping {
private:
    map<string, int> letter_index;
    map<int, string> index_letter;
    unordered_set<string > letter;

public:

    void add_letter(string i) {
        if(letter.find(i) == letter.end()) {
            letter_index[i] = letter.size();
            index_letter[letter.size()] = i;
            letter.insert(i);
        }
    }

    int get_index(string letter) {
        return letter_index[letter];
    }

    string get_letter(int index) {
        return index_letter[index];
    }

    int size() { return letter.size();}


    int get_next_site_id() {
        string site;
        cin >> site;
        add_letter(site);
        return get_index(site);
    }

};


int main() {
    int s, r, f, t;
    cin >> s >> r >> f >> t;

    int N = s + 2 * t; // sites + transport (vertex cap)
    int source = N;
    int target = N + 1;
    Dinic solver(N+2);

    int site_int = 0;

    // To create id for site (maintain like between ids and letters)
    index_mapping mapp;

    // Source to raw sites
    for (int i = 0; i < r ; i++) {
        solver.AddEdge(source, mapp.get_next_site_id(), 1);
    }

    // Factories to sink
    for (int i = 0; i < f ; i++) {
        solver.AddEdge(mapp.get_next_site_id(), target, 1);
    }

    // Tranport company : vertex capacity and connection to sites
    int nb_state_accessible = 0;
    for (int i = 0 ; i < t ; i++) {

        int in = s + i;
        int out = in + t;

        // Vertex capacity
        solver.AddEdge(in, out, 1);
        cin >> nb_state_accessible;
        for (int j = 0 ; j < nb_state_accessible; j++) {
            site_int = mapp.get_next_site_id();
            // Linking both ways
            solver.AddEdge(site_int, in, 1);
            solver.AddEdge(out, site_int, 1);
        }
    }

    cout << solver.GetMaxFlow(source, target) << "\n";

    return 0;
}
