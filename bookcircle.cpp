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

int main(){
	int B, G;
	map<string, vector<int >> book_owners;
	string name;
	cin >> B >> G;
	int N = B + G;
	Dinic dinic(N + 2);
	int s = N;
	int t = N + 1;

	int books;
	for(int i = 0; i < B; i++) {
		cin >> name;
		cin >> books;
		dinic.AddEdge(s, i, 1);
		for(int b = 0; b < books ; b++) {
			cin >> name;
			book_owners[name].push_back(i);
		}
	}

	for(int i = 0; i < G; i++) {
		cin >> name;
		cin >> books;
		dinic.AddEdge(i + B, t, 1);
		for(int b = 0; b < books ; b++) {
			cin >> name;
			book_owners[name].push_back(i+B);
		}
	}

	for (pair<string, vector<int > > p: book_owners) {
		vector<int > & links = p.second;
		if(links.size() == 2) {
			dinic.AddEdge(links[0], links[1], 1);
		}
	}
	cout << dinic.GetMaxFlow(s,t) << endl;
	return 0;
}
