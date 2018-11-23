//https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2108
#include <bits/stdc++.h>

using namespace std;

const int INF = 2000000000;
typedef long long ll;

struct Edge {
    int from, to, index;
    ll cap, flow;
    Edge(int from, int to, ll cap, ll flow, int index) :
        from(from), to(to), cap(cap), flow(flow), index(index) {}
};

struct Dinic {
    int N;
    vector<vector<Edge> > G;
    vector<Edge *> dad;
    vector<int> Q;

    Dinic(int N) : N(N), G(N), dad(N), Q(N) {}

    void AddEdge(int from, int to, ll cap) {
        G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
        if (from == to) G[from].back().index++;
        G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
    }

    ll BlockingFlow(int s, int t) {
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

        ll totflow = 0;
        for (int i = 0; i < G[t].size(); i++) {
            Edge *start = &G[G[t][i].to][G[t][i].index];
            ll amt = INF;
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

    ll GetMaxFlow(int s, int t) {
        ll totflow = 0;
        while (ll flow = BlockingFlow(s, t))
            totflow += flow;
        return totflow;
    }
};

typedef pair<int,int> ii;

bool pair_desc(ii p1, ii p2){return p1.first > p2.first;}


struct result {
    ll a;
    ll b;
    ll add;
    ll sub;
    ll mul;
};

result R[2505];

class index_mapping {
private:
    map<ll, int> ans_index;
    map<int, ll> index_ans;
    unordered_set<int > ans;
    int N;

public:

    explicit index_mapping(int N): N(N) {}

    void add_ans(int i) {
        if(ans.find(i) == ans.end()) {
            // cout << "inserting " << i << endl;
            ans_index[i] = ans.size() + N;
            index_ans[ans.size() + N] = i;
            ans.insert(i);
        }
    }

    int get_index(ll ans) {
        return ans_index[ans];
    }

    ll get_ans(int index) {
        return index_ans[index];
    }

    int size() { return ans.size();}

};

int main(){
    ios_base::sync_with_stdio(false);
    int N;
    cin >> N;

     // one pair and 3 operations, N times
    Dinic dinic(N * 4 +2);
    int s = N * 4;
    int t = s + 1;

    ll a, b;
    ll add, sub, mul;
    index_mapping mapp(N);
    for (int n = 0; n < N; n++) {
        cin >> a >> b;
        add = a + b;
        sub = a - b;
        mul = a * b;
        R[n].a = a;
        R[n].b = b;

        mapp.add_ans(R[n].add = add);
        mapp.add_ans(R[n].sub = sub);
        mapp.add_ans(R[n].mul = mul);

        // In this order to be find again then
        dinic.AddEdge(n, mapp.get_index(add), 1);
        dinic.AddEdge(n, mapp.get_index(sub), 1);
        dinic.AddEdge(n, mapp.get_index(mul), 1);
        dinic.AddEdge(s, n, 1);

    }

    // Answers to sink
    for(int i = 0; i < mapp.size(); i ++) {
        dinic.AddEdge(N + i, t, 1);
    }

    // Resolving MCBM
    if (dinic.GetMaxFlow(s,t) != N) {
        cout << "impossible\n";
        return 0;
    }

    // Inferring the matching from the pair of numbers
    for(int i = 0; i < N ; i++) {
        for(int j = 0 ; j < 3 ; j++) {
            const Edge & e = dinic.G[i][j];
            if(e.flow == 1) {
                cout << R[i].a;
                // See order above
                if(j == 0) {
                    cout << " + ";
                }
                if (j == 1) {
                    cout << " - ";
                }
                if (j == 2) {
                    cout << " * ";
                }
                cout << R[i].b << " = " << mapp.get_ans(e.to) << endl;
            }
        }
    }
    return 0;
}
