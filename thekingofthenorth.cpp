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

int X, Y;

int id(int x, int y) {
	return y + Y * x;
}

int main() {

	// X : rows, Y : cols
	cin >> X >> Y;

	// Vertex capacity
	Dinic solver(2 * X * Y + 1);
	int border = 2 * X * Y;

	int cap;
	for(int x = 0 ; x < X ; x++) {
		for(int y = 0 ; y < Y ; y++) {
			cin >> cap;
			int in = id(x, y);
			int out = in + X * Y;

			solver.AddEdge(in, out, cap);
			if (x == 0 || y == 0 || x == X - 1 || y == Y - 1) {
				solver.AddEdge(out, border, INF);
			}

			if (x > 0) {
				solver.AddEdge(out, id(x - 1, y), INF);
			}

			if (x < X - 1) {
				solver.AddEdge(out, id(x + 1, y), INF);
			}

			if (y > 0) {
				solver.AddEdge(out, id(x, y - 1), INF);
			}

			if (y < Y - 1) {
				solver.AddEdge(out, id(x, y + 1), INF);
			}
		}
	}

	int a, b;
	cin >> a >> b;
	int castle = id(a, b);

	cout << solver.GetMaxFlow(castle, border) << endl;

    return 0;
}
