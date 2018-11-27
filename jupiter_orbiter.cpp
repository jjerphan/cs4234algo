//https://open.kattis.com/problems/jupiter?fbclid=IwAR31ejtuxahns1LW4YeQMb5IyhA4C1Loxzyc2FgPmzcOvU5p_pWZSzQYgM4
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

#define oo 987654321

struct Queue {
	int cap;
	vector<int> sensors;
};

int main() {
	ios_base::sync_with_stdio(false);
	int N, Q, S;
	cin >> N >> Q >> S;
	int source = N * Q * 2 + N, sink = source + 1;
	Dinic graph(sink + 1);
	vector<Queue> queues(Q);
	for(int i = 0; i < S; i++){
		int s;
		cin >> s;
		queues[--s].sensors.push_back(i);
	}
	for(Queue &q : queues)
		cin >> q.cap;
	vector<int> sensor_inputs(S);
	int offset_window = N*Q*2, input_sum = 0;
	for(int i = 0; i < N; i++){
		int d, s, offset_in = i*Q, offset_out = N*Q + offset_in;
		cin >> d;
		graph.AddEdge(offset_window + i, sink, d);
		for(int &input : sensor_inputs){
			cin >> input;
			input_sum += input;
		}
		for(int j = 0; j < Q; j++){
			Queue &q = queues[j];
			int sum = 0;
			for(int &s : q.sensors)
				sum += sensor_inputs[s];
			graph.AddEdge(source, offset_in + j , sum);
			graph.AddEdge(offset_in + j, offset_out + j, queues[j].cap);
			graph.AddEdge(offset_out + j, offset_window + i, oo);
			if(i < N - 1)
				graph.AddEdge(offset_out + j, offset_in + Q + j, oo);
		}
	}
	int flow =  graph.GetMaxFlow(source, sink);
	if(input_sum == flow)
		cout << "possible" << endl;
	else 
		cout << "impossible" << endl;
}