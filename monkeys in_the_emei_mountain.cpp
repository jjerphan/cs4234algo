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
	size_t N,M, cno = 1;
	while(cin >> N , N > 0){
		vector<ii> intervals, t_ape;
		set<int> cur;
		int vsum = 0, s = N*3, t = s + 1;
		Dinic mf(t + 1);
		cin >> M;
		for(int i = 0; i < N; i++){ //Read all monkeys and add to relevant structures
			int v, a, b;
			cin >> v >> a >> b;
			vsum += v;
			t_ape.push_back(ii(a, i));
			t_ape.push_back(ii(b, i));
			mf.AddEdge(s, i, v);
		}
		sort(t_ape.begin(), t_ape.end()); //Sort intervals of a/b
		ii p = t_ape[0];
		for(ii q : t_ape){ // For each start/stop time in order
			if(p.first < q.first){
				int delta = q.first - p.first, j = N + intervals.size();
				for(int id : cur)
					mf.AddEdge(id, j, delta);
				mf.AddEdge(j, t, delta*M);
				intervals.push_back(ii(p.first, q.first));
				p = q;
			}
			if(cur.find(q.second) == cur.end())
				cur.insert(q.second);
			else
				cur.erase(q.second);
		}
		cout << "Case " << cno++ << ": ";
		if(mf.GetMaxFlow(s,t) == vsum){ // Possible
			cout << "Yes" << endl;
			vector<vector<ii>> allocs(N);
			for(int i = 0; i < intervals.size(); i++){ // Go through all intervals
				vector<ii> prio;
				for(Edge e : mf.G[N+i])
					if(e.flow < 0)
						prio.push_back(ii(-e.flow, e.to)); // Add all negative flows (flow to interval) to piority set
				for(int j = intervals[i].first; j < intervals[i].second; j++){
					sort(prio.begin(), prio.end(), pair_desc); //will simply sort it each time (should be close to O(n), as we will only have to swap few edges)
					for(int m = 0; m < min(M, prio.size()); m++){
						int id = prio[m].second;
						if(allocs[id].empty() || allocs[id].back().second < j)
							allocs[id].push_back(ii(j, j+1));
						else
							allocs[id].back().second++;
						if(--prio[m].first <= 0)
							prio.erase(prio.begin() + m--);
					}
				}
			}
			for(int i = 0; i < N; i++){
				cout << allocs[i].size();
				int sum = 0;
				for(ii p : allocs[i]){
					cout << " (" << p.first << "," << p.second << ")";
				}
				cout << endl;
			}
		} else { //impossible
			cout << "No" << endl;
		}
	}
}
