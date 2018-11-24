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

struct bid {
	int amount;
	int side;
	unordered_set<int > channels;
};

bid bids[1000000];

void parse_ints(vector<int> & ints) {
	string line;
	getline(cin, line);
	stringstream in( line );
	copy(istream_iterator<int, char>(in), istream_iterator<int, char>(),
	back_inserter(ints));
}

int main(){
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	int cas = 0;
	while(T--) {
		if(cas != 0) {
			cout << endl;
		}
		cout << "Case " << ++cas << ":\n";
		map<int, vector<int >> channels_bids;
		string line;
		int N = 0;
		int n_temp;
		int tot_amount = 0;
		// Parsing info, quite a boiler plate
		for(int side = 0; side < 2; side ++) {
			cin >> n_temp;
			getline(cin, line);
			for(int i = 0; i < n_temp ; i++) {
				int id = N + i;
				vector<int> ints;
				parse_ints(ints);
				bids[id].amount = ints[0];
				tot_amount += ints[0];
				bids[id].side = side;
				bids[id].channels.clear();
				for (int chan = 1; chan < ints.size(); chan++) {
					channels_bids[ints[chan]].push_back(id);
					bids[id].channels.insert(ints[chan]);
				}
			}
			N += n_temp;
		}
		Dinic dinic(N + 2);
		int s = N;
		int t = N + 1;

		for(int i = 0 ; i < N ; i++) {
			if(i < N - n_temp)
				dinic.AddEdge(s, i, bids[i].amount);
			else
				dinic.AddEdge(i, t, bids[i].amount);
		}

		// for(int i = 0; i < N ; i++) {
		// 	cout << "Bid " << i << endl;
		// 	cout << " - Side " << bids[i].side << endl;
		// 	cout << " - Amount " << bids[i].amount << endl;
		// 	cout << bids[i].channels.size() << " - Channels : ";
		// 	for (int chan : bids[i].channels) {
		// 		cout << chan << " ";
		// 	}
		// 	cout << endl;
		// }

		for (pair<int, vector<int > > p: channels_bids) {
			vector<int > & links = p.second;
			if(links.size() == 2) {
				dinic.AddEdge(links[0], links[1], INF);
			}
		}
		cout << tot_amount - dinic.GetMaxFlow(s,t) << endl;
	}

	return 0;
}
