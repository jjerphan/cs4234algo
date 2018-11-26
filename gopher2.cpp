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

typedef pair<int, int > ii;


struct pos {
    float x, y;
};

pos gophers[100];
pos holes[100];


int case_n = 0;
int main() {
	int n, m, s, v;
	while(cin >> n) {
	    cin >> m >> s >> v;

	 	for (int i = 0; i < n; i++) {
	        cin >> gophers[i].x;
	        cin >> gophers[i].y;
	    }

	    for (int i = 0; i < m; i++) {
	        cin >> holes[i].x;
	        cin >> holes[i].y;
	    }
		Dinic solver(n + m + 2);
		int source = n + m;
		int target = source + 1;

	    // Computing second needed per pair
	    for (int robot = 0; robot < n; robot++) {
	        for (int hole = 0; hole < m; hole++) {
	            float diff_x = gophers[robot].x - holes[hole].x;
	            float diff_y = gophers[robot].y - holes[hole].y;
	            float dist = sqrt(diff_x * diff_x + diff_y * diff_y);
	            float sec_needed = dist / v;
				if (sec_needed <= s) {
		            solver.AddEdge(robot, hole + n, 1);
				}
	        }
	    }

	    for (int i = 0; i < n; i++) {
	        solver.AddEdge(source, i, 1);
	    }


	    for (int i = 0; i < m; i++) {
	        solver.AddEdge(i + n, target, 1);
	    }

	    cout <<  n - solver.GetMaxFlow(source, target) << endl;
	}
    return 0;
}
