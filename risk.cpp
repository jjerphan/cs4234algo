https://open.kattis.com/problems/risk
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

string &findStartLetter(vector<string> words, char letter){
	for(string &word : words)
		if(word.at(0) == letter)
			return word;
	return words[0];
}

int armies[105];

int main(){
	ios_base::sync_with_stdio(false);
	int T, R;
	cin >> T;

	while(T--) {
		cin >> R;
		vector<vector<int > > neighs(R);
		unordered_set<int > enemy_regions;
		for(int r = 0 ; r < R; r ++) {
			cin >> armies[r];
			if (armies[r] == 0) {
				enemy_regions.insert(r);
			}
		}

		char temp;
		for(int i = 0; i < N ; i++) {
			for(int j = 0; j < N ; j++) {
				cin >> temp;
				if(temp == Y) {
					neighs[i].push_back(j);
					neighs[j].push_back(i);
				}
			}
		}

		// Identify weakest
		int weakest = -1;
		int armies_weakest = 1000;
		for(const int en_reg: enemy_regions) {
			for(const int in_danger: neighs[en_reg]) {
				if (armies[in_danger] > 0 && armies[in_danger] < armies_weakest) {
					weakest = in_danger;
					armies_weakest = armies[weakest];
				}
			}
		}

		Dinic mf(3 + neights[weakest].size());
		mf.AddEdge(s, i, 1);
		mf.AddEdge(n+i, t, 1);
		mf.GetMaxFlow(s, t);


		for (const int u: neights[weakest]) {


		}

	}
	return 0;
}
