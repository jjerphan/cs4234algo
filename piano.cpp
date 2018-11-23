//https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2108
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

typedef pair<int,int> ii;

bool pair_desc(ii p1, ii p2){return p1.first > p2.first;}

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	int N, P;
	cin >> T;
	int nb_days = 100;

	while(T--) {
		cin >> N >> P;
		P /= 2; // lifters work in pairs
		Dinic mf(nb_days + N + 2);
		int s = nb_days + N, t = nb_days + N + 1;
		int first_day = nb_days, last_day = 0;
		int beg, end;

		// Linking pianos
		for(int i = 0 ; i < N ; i ++) {
			cin >> beg >> end;
			mf.AddEdge(s, i, 1);
			first_day = min(first_day, beg);
			last_day = max(last_day, end);
			for (int j = beg; j < end + 1 ; j++)
				mf.AddEdge(i, j + N - 1, 1);
		}

		// Week days to sink
		for (int i = first_day; i < last_day + 1 ; i++) {
			if((i % 7) != 0 & (i % 6) != 0) {
				mf.AddEdge(i + N - 1, t, P);
			}
		}

		int max_flow = mf.GetMaxFlow(s,t);
		if (max_flow == N) {
			cout << "fine" << endl;
			continue;
		}

		// If not possible, add week-ends
		for (int i = first_day; i < last_day + 1 ; i++) {
			if((i % 7) == 0 || (i % 6) == 0) {
				mf.AddEdge(i + N - 1, t, P);
			}
		}

		// Adding the new flow
		max_flow += mf.GetMaxFlow(s,t);
		if (max_flow == N) {
			cout << "weekend work" << endl;
		} else {
			cout << "serious trouble" << endl;
		}

	}

	return 0;
}
