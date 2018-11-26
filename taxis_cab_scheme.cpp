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

struct ride {
    int begin;
    int end;
    int beg_x;
    int beg_y;
    int end_x;
    int end_y;
};

void resolve_case() {
    int N;

    cin >> N;

    vector<ride > rides(N);

    Dinic solver(2 + 2 * N);
    int source = 0;
    int target = 1 + 2 * N;

    // Computing info of rides
    char osef;  // buffer for ":"
    int hour, min;
    int time_spent;
    for (int n = 0 ; n < N ; n++) {
        cin >> hour;
        cin >> osef;
        cin >> min;
        cin >> rides[n].beg_x;
        cin >> rides[n].beg_y;
        cin >> rides[n].end_x;
        cin >> rides[n].end_y;

        // Time of the ride
        time_spent =  abs(rides[n].beg_x - rides[n].end_x);
        time_spent += abs(rides[n].beg_y - rides[n].end_y);

        rides[n].begin = hour * 60 + min;
        rides[n].end = rides[n].begin + time_spent;

        solver.AddEdge(source, n + 1, 1);
        solver.AddEdge(n + 1 + N, target, 1);
    }

    // Linking rides if it's possible
    for (int i = 0; i < N ; i++) {
        for (int j = 0 ; j < N ; j++) {
            if (i != j) {
                // Transit time
                time_spent =  abs(rides[j].beg_x - rides[i].end_x);
                time_spent += abs(rides[j].beg_y - rides[i].end_y);

                if (rides[i].end + time_spent < rides[j].begin) {
                    solver.AddEdge(1 + i, 1 + j + N, 1);
                }
            }
        }
    }

    cout << N - solver.GetMaxFlow(source, target) << "\n";
}

int main() {
    int T;
    cin >> T;

    for (int t = 0 ; t < T; t++) {
        resolve_case();
    }


    return 0;
}
