//https://open.kattis.com/problems/avoidingtheapocalypse
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

#define INF 1337

typedef pair<int, int> ii;

struct TestCase {
	int n, i, g, s;
	struct Road {int b, p, t;};
	set<int> medical;
	vector<vector<Road>> locations;
	
	TestCase(){
		//Note here i will convert index to start from zero!
		int m, r;
		cin >> n;
		cin >> i >> g >> s;
		i--; s++;
		cin >> m;
		for(int j = 0; j < m; j++){int x; cin >> x; medical.insert(x-1); }
		cin >> r;
		locations.resize(r);
		for(int j = 0; j < r; j++){
			int a;
			Road r;
			cin >> a >> r.b >> r.p >> r.t;
			r.b--;
			locations[--a].push_back(r);
		}
	}

	int index(ii p){ return p.first * s + p.second;}

	int maxflow(){
		int sink = n*s, flow = 0;
		ii source(i,0);
		Dinic mf(sink + 1);
		vector<bool> taken(n*s);
		vector<ii> Q;
		for(int m : medical)
			mf.AddEdge(index(ii(m, 0)), sink, INF);
		for(Road &r : locations[i]){
			if(r.t < s){
				if(medical.find(r.b) != medical.end()){
					flow += r.p * (s - r.t);
					continue;
				}
				Q.push_back(ii(r.b, r.t)); 
				taken[index(Q.back())] = true;
				for(int j = r.t; j < s; j++)
					mf.AddEdge(index(source), index(ii(r.b, j)), r.p);
			}
		}
		while(!Q.empty()){
			ii p = Q.back(); Q.pop_back();
			if(p.second < s - 1){
				ii next(p.first, p.second + 1);
				mf.AddEdge(index(p), index(next), INF);
				if(!taken[index(next)]) 
					Q.push_back(next);
			}
			for(Road r : locations[p.first]){
				ii q(r.b, r.t + p.second);
				if(!taken[index(q)] && q.second < s){
					taken[index(q)] = true;
					if(medical.find(q.first) == medical.end()){
						mf.AddEdge(index(p), index(q), r.p);
						Q.push_back(q);
					} else {
						mf.AddEdge(index(p), index(ii(r.b, 0)), r.p);
					} 	
				}
			}
		}
		return min(mf.GetMaxFlow(index(source), sink) + flow, (long long) g);
	}

};

int main(){
	int cno;
	cin >> cno;
	while(cno--){
		TestCase tc{};
		cout << tc.maxflow() << endl;
	}
}