//https://open.kattis.com/problems/risk
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

struct Region {
	int a;
	vector<int> neighbors;
	bool at_enemy;
};


int main(){
	ios_base::sync_with_stdio(false);
	int TC;
	cin >> TC;
	while(TC--){
		int N, nfront = 0;
		cin >> N;
		vector<Region> regions(N);
		for(int i = 0; i < N; i++)
			cin >> regions[i].a;
		for(int i = 0; i < N; i++){
			string s;
			cin >> s;
			for(int j = 0 ; j < N; j++){
				if(!regions[i].a == 0 && s[j] == 'Y'){
					if(regions[j].a != 0) {
						regions[i].neighbors.push_back(j);
					} else if(!regions[i].at_enemy){
						regions[i].at_enemy = true;
						nfront++;
					}
				}
			}
		}
		int i = 0, j = N * 100, best = 1, source = N*2, sink = source + 1;
		while(i <= j){
			int m = (i+j)/2;
			Dinic graph(sink + 1);
			for(int k = 0; k < N; k++){
				if(regions[k].a == 0) continue;
				if(regions[k].at_enemy)
					graph.AddEdge(k, sink, m - 1);
				graph.AddEdge(source, k, regions[k].a - 1);
				graph.AddEdge(k, k + N, regions[k].a);
				for(int neighbor : regions[k].neighbors)
					graph.AddEdge(k + N, neighbor, 1337);
			}
			if(graph.GetMaxFlow(source, sink) == nfront * (m-1)){
				i = m + 1;
				best = m;
			} else {
				j = m - 1;
			}
		}
		cout << best << endl;
	}
}