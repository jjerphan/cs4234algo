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

struct slide {
	int x_max, x_min, y_max, y_min;

	bool in_slide(int x, int y) {
		return x_min <= x && x <= x_max && y_min < y && y < y_max;
	}
};

slide slides[10000];

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	int N, P;
	int c = 0;
	while(1) {
		cin >> N;
		if (N == 0) {
			return 0;
		}
		cout << "Heap " << ++c << endl;
		Dinic dinic(N*2 + 2);
		int s = 2 * N;
		int t = s + 1;

		list<ii > all_edges;
		for (int n = 0 ; n < N; n++) {
			cin >> slides[n].x_min;
			cin >> slides[n].x_max;
			cin >> slides[n].y_min;
			cin >> slides[n].y_max;
			all_edges.push(ii(s, n));
			all_edges.push(ii(n+N, t));
			dinic.AddEdge(s, n, 1);
			dinic.AddEdge(n+N, t, 1);
		}

		int x,y;
		for (int n = 0; n < N ; n++) {
			cin >> x >> y;
			for (int i = 0 ; i < N; i++) {
				if(slides[i].in_slide(x, y)) {
					all_edges.push(ii(i, n + N))
					dinic.AddEdge(i, n + N, 1);
				}
			}
		}

		if (dinic.GetMaxFlow(s,t) != N) {
			cout << "none" << endl;
			continue;
		}


		list<ii > matched_edges;
		for(int m = N ; m >= 0 ; m--) {
			for(int i = 0; i < N ; i++) {
				for(Edge & e : dinic.G[i]) {
					if(e.flow == 1) {
						matched_edges.push_back(ii(i,e.to));
					}
				}
			}
		}

		unordered_set<ii > removed;
		while(!matched_edges.empty())) {
			removed.insert(matched_edges.front);
			matched_edges.pop_front();
			Dinic dinic(N*2 + 2);

		}


		for(int i = 0 ; i < edges.size(); i++) {
			char slide = 'A' + edges[i].first;
			int number = edges[i].second + 1 - N;
			cout << "(" << slide << "," << number << ") ";
		}



		cout << endl <<endl;
	}

	return 0;
}
