#include <bits/stdc++.h>


using namespace std;
typedef long long LL;

struct Edge {
  int u, v;
  LL cap, flow;
  Edge() {}
  Edge(int u, int v, LL cap): u(u), v(v), cap(cap), flow(0) {}
};

struct Dinic {
  int N;
  vector<Edge> E;
  vector<vector<int>> g;
  vector<int> d, pt;

  Dinic(int N): N(N), E(0), g(N), d(N), pt(N) {}

  void AddEdge(int u, int v, LL cap) {
    if (u != v) {
      E.emplace_back(u, v, cap);
      g[u].emplace_back(E.size() - 1);
      E.emplace_back(v, u, 0);
      g[v].emplace_back(E.size() - 1);
    }
  }

  bool BFS(int S, int T) {
    queue<int> q({S});
    fill(d.begin(), d.end(), N + 1);
    d[S] = 0;
    while(!q.empty()) {
      int u = q.front(); q.pop();
      if (u == T) break;
      for (int k: g[u]) {
        Edge &e = E[k];
        if (e.flow < e.cap && d[e.v] > d[e.u] + 1) {
          d[e.v] = d[e.u] + 1;
          q.emplace(e.v);
        }
      }
    }
    return d[T] != N + 1;
  }

  LL DFS(int u, int T, LL flow = -1) {
    if (u == T || flow == 0) return flow;
    for (int &i = pt[u]; i < g[u].size(); ++i) {
      Edge &e = E[g[u][i]];
      Edge &oe = E[g[u][i]^1];
      if (d[e.v] == d[e.u] + 1) {
        LL amt = e.cap - e.flow;
        if (flow != -1 && amt > flow) amt = flow;
        if (LL pushed = DFS(e.v, T, amt)) {
          e.flow += pushed;
          oe.flow -= pushed;
          return pushed;
        }
      }
    }
    return 0;
  }

  LL MaxFlow(int S, int T) {
    LL total = 0;
    while (BFS(S, T)) {
      fill(pt.begin(), pt.end(), 0);
      while (LL flow = DFS(S, T))
        total += flow;
    }
    return total;
  }
};


struct flight {
    int day;
    int from;
    int to;
    int cap;
};

bool operator <(const flight& x, const flight& y) {
    return std::tie(x.day, x.from, x.to, x.cap) <
    std::tie(y.day, y.from, y.to, y.cap);
}

int C, D, F;

int index_city_day(int c, int d) {
    return d + c * D;
}

void resolve_case() {
    scanf("%d %d %d", &C, &D, &F);

    // Getting flights info
    unordered_set<int > prices;
    int price;

    vector<pair<int, flight >> fs(F);
    for (int i = 0; i < F; i++) {
      scanf("%d %d %d", &fs[i].second.from,
                        &fs[i].second.to,
                        &fs[i].second.cap);

      // 1 to 0 indexing
      fs[i].second.from--;
      fs[i].second.to--;
      scanf("%d", &price);
      prices.insert(price);
      fs[i].first = price;
      scanf("%d", &fs[i].second.day);
    }

    // Sorting prince for binary search
    sort(fs.begin(), fs.end());
    vector<int > pr(prices.begin(), prices.end());

    // Constructing the basic graph taht will be completed every time
    Dinic basic_graph(1 + C * D);
    // last day, last city
    int target = C * D - 1;
    int source = target + 1;
    int people;

    int tot_people = 0;
    for (int c = 0 ; c < C ; c++) {
        cin >> people;
        tot_people += people;
        basic_graph.AddEdge(source, index_city_day(c, 0), people);
        for (int d = 0 ; d < D - 1 ; d++) {
            // People can stay in the same city over time
            basic_graph.AddEdge(index_city_day(c, d),
            index_city_day(c, d+1),
            100000);
        }
    }

    // Binary search on the price
    int high = pr.size() - 1;
    int low = 0;
    int res;
    while (low < high) {
        Dinic graph = basic_graph;
        int mid = (high + low) / 2;
        price = pr[mid];
        // Construct graph
        for (int i = 0; i < fs.size() ; i++) {
            int price_flight = fs[i].first;
            if (price_flight > price) {
                break;
            }
            int index_from = index_city_day(fs[i].second.from,
                                            fs[i].second.day);
            int index_to = index_city_day(fs[i].second.to,
                                          fs[i].second.day + 1);
            graph.AddEdge(index_from, index_to, fs[i].second.cap);
        }

        res = graph.MaxFlow(source, target);

        if (res < tot_people) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    if (res != tot_people) {
        cout << "IMPOSSIBLE\n";
    } else {
        cout << pr[low] << "\n";
    }
}

int main() {
  int T;
  cin >> T;
  for (int _t = 1 ; _t < T + 1; _t++) {
      cout << "Case #" << _t << ": ";
      resolve_case();
  }
  return 0;
}
