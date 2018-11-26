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

struct Monkey { 
	int v,a,b;
	vector<ii> allocs;
};

struct Schedule {
	int v, l, id;
};

bool schedule_desc(Schedule p1, Schedule p2){return p1.v > p2.v;}

struct TestCase {
	int N, M;

	vector<Monkey> monkeys;
	vector<ii> intervals;

	int interval_index(int interval_id, int l){
		return monkey_index(N, 0) + M*interval_id + l;
	}

	int monkey_index(int monkey_id){
		return monkey_id;
	}
	
	int monkey_index(int monkey_id, int interval_id){
		return monkey_index(N) + 2*N*monkey_id + interval_id; 
	}

	TestCase(int N): N(N) {
		cin >> M;
		int source = interval_index(2*N, 0), sink = source + 1, vsum = 0;
		Dinic mf(sink + 1);
		monkeys.resize(N);
		vector<ii> monkeys_by_time;
		for(int i = 0; i < N; i++){
			Monkey &m = monkeys[i];
			cin >> m.v >> m.a >> m.b;
			vsum += m.v;
			monkeys_by_time.push_back(ii(m.a, i));
			monkeys_by_time.push_back(ii(m.b, i));
			mf.AddEdge(source, monkey_index(i), m.v);
		}
		sort(monkeys_by_time.begin(), monkeys_by_time.end());
		set<int> cur;
		ii p = monkeys_by_time[0];
		for(ii q : monkeys_by_time){ // For each start/stop time in order
			if(p.first < q.first){
				int delta = q.first - p.first, j = intervals.size();
				for(int id : cur) {
					mf.AddEdge(monkey_index(id), monkey_index(id, j), delta); // Add edges to intermediate monkey
				}
				for(int i = 0; i < M; i++){
					mf.AddEdge(interval_index(j, i), sink, delta); // Add edges to sink from interval_i_j
					for(int id : cur) 
						mf.AddEdge(monkey_index(id, j), interval_index(j, i), delta); //Add flow from each monkey to interval_i_j
				}
				intervals.push_back(ii(p.first, q.first));
				p = q;
			}
			if(cur.find(q.second) == cur.end())
				cur.insert(q.second);
			else
				cur.erase(q.second);
		}
		if(mf.GetMaxFlow(source, sink) == vsum){ // Possible
			cout << "Yes" << endl;
			for(int i = 0; i < intervals.size(); i++){
				ii &interval = intervals[i];
				vector<Schedule> schedules(M);
				for(int j = 0; j < M; j++)
					for(Edge &e : mf.G[interval_index(i, j)])
						if(e.flow < 0)
							schedules.push_back(Schedule{abs(e.flow), j, (e.to-i)/(2*N+1)});
				for(int a = interval.first; a < interval.second; a++){
					bitset<5> covered(false);
					sort(schedules.begin(), schedules.end(), schedule_desc);
					for(Schedule &scd : schedules){
						if(!covered[scd.l] && scd.v-- > 0){ //not currently drinking
							covered[scd.l] = true;
							monkeys[scd.id].v--;
							if(monkeys[scd.id].allocs.empty() || monkeys[scd.id].allocs.back().second != a)
								monkeys[scd.id].allocs.push_back(ii(a, a+1));
							else
								monkeys[scd.id].allocs.back().second++;
						}
					}
					
				}
			}
			for(int i = 0; i < N; i++){
				if(monkeys[i].v != 0) cout << "FAILED ALLOCATION: " << monkeys[i].v << endl;
				cout << monkeys[i].allocs.size();
				for(ii alloc : monkeys[i].allocs) cout << " " << "(" << alloc.first << "," << alloc.second << ")";
				cout << endl;
			}
		} else { //impossible
			cout << "No" << endl;
		}

	}
};

int main(){
	ios_base::sync_with_stdio(false);
	int N,M, cno = 1;
	while(cin >> N , N > 0){
		cout << "Case " << cno++ << ": ";
		TestCase tc{N};
	}
}