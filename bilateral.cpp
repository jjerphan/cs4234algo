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

class index_mapping {
private:
    map<int, int> node_index;
    map<int, int> index_node;
    unordered_set<int > node;
    int N;

public:

    explicit index_mapping(int N=0): N(N) {}

    void add_node(int i) {
        if(node.find(i) == node.end()) {
            node_index[i] = node.size() + N;
            index_node[node.size() + N] = i;
            node.insert(i);
        }
    }

    int get_index(int node) {
        return node_index[node];
    }

    int get_node(int index) {
        return index_node[index];
    }

    int size() { return node.size();}

};

index_mapping mapp;

void print_mvc(Dinic& dinic, int s, int t) {

	unordered_set<int > U;
	for(Edge & e: dinic.G[s] ) {
		if (e.flow == 0) {
			U.push_back(e.to)
		}
	}

	// Todo : find MVC using K = (L \ Z) U (R inter Z)

}

int main() {
	ios_base::sync_with_stdio(false);
	int M;
	int N = 2000;
	cin >> M;
	int s = N;
	int t = s + 1;
	int friend_ = 1009;

	Dinic without_dinic(N + 2);
	Dinic with_dinic(N + 2);

	unordered_set<int > as;
	unordered_set<int > bs;
	for(int i = 0; i < M ; i++) {
		int a, b;
		cin >> a >> b;
		as.insert(a);
		bs.insert(b);
		mapp.add_node(a);
		mapp.add_node(b);
		if (a!= friend_) {
			without_dinic.AddEdge(mapp.get_index(a), mapp.get_index(b), 1);
		}
		// cout << "Link " << a << " " << b << endl;
		with_dinic.AddEdge(mapp.get_index(a), mapp.get_index(b), 1);
	}

	for(const int a: as) {
		with_dinic.AddEdge(s, mapp.get_index(a), 1);
		without_dinic.AddEdge(s, mapp.get_index(a), 1);
	}

	for(const int b: bs) {
		with_dinic.AddEdge(mapp.get_index(b), t, 1);
		without_dinic.AddEdge(mapp.get_index(b), t, 1);
	}

	int with = with_dinic.GetMaxFlow(s, t);
	int without = without_dinic.GetMaxFlow(s, t);

	cout << with << endl;
	cout << without << endl;

	if (without < with) {
		cout << with << endl;
		print_mvc(with_dinic,s,t);
	} else {
		cout << without << endl;
		print_mvc(without_dinic,s,t);
	}

	return 0;
}
