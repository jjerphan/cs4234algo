//https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=26&page=show_problem&problem=2413
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

int main(){
	ios_base::sync_with_stdio(false);
	int N, n, k, cno = 1;
	string word;

	cin >> N;
	while(N--){
		cout << "Case #" << cno ++ << ":" << endl;
		cin >> n;
		vector<vector<string>> words(n);
		int s = n + 26, t = s + 1;
		Dinic mf(t + 1);
		for(int i = 0; i < n; i++){
			cin >> k;
			for(int j = 0; j < k; j++){
				cin >> word;
				transform(word.begin(), word.end(), word.begin(), ::tolower);
				word.at(0) = toupper(word.at(0));
				mf.AddEdge(i, n + word.at(0) - 'A', 1);
				words[i].push_back(word);
			}
			mf.AddEdge(s, i, 1);
			mf.AddEdge(n+i, t, 1);
		}
		mf.GetMaxFlow(s, t);
		vector<string> output(n);
		for(int i = 0; i < n; i++)
			for(Edge e : mf.G[i])
				if(e.flow > 0){
					output[i] = findStartLetter(words[i], 'A' + (e.to - n));
					break;
				}
		sort(output.begin(), output.end());
		for(string word : output)
			cout << word << endl;
	}
}
