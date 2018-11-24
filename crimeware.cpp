#include<cstdio>
#include<vector>
#include<queue>
#include<bits/stdc++.h>

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
/// End of Stanford ACM Dinic

int main()
{
  int P;
  std::cin >> P;
  for(int p = 0 ; p < P; p++) {
    // # streets, avenues, banks respectively
    LL S,A,B;
    std::cin >> S >> A >> B;

    LL width = S;
    LL height = A;
    LL offset = width * height;
    // We add vertex capacity of 1
    // source: first index, i.e. 0
    // v_in -1-> v_out
    // v_in in {1,..., offset}
    // v_out in {1+offset,...,offset*2}
    // hence
    //   v_in = i => v_out = i+offset
    //   v_out = i => v_in = i-offset
    // sink: last index, i.e. N-1
    LL N = 2 + 2 * offset;
    LL source = 0;
    LL sink = N-1;
    Dinic dinic(N);
    LL x, y;

    // Connecting outgoing avenues and streets to sink
    // and creating vertex capacities
    // Iterating on the grid {1... offset}
    for(int i = source + 1; i < 1 + offset; i++) {
      // Vertex Capacity
      dinic.AddEdge(i, i + offset, 1);

      // Computing the position on the grid
      int index_grid = i - 1;
      x = index_grid % width;
      y = index_grid / width;

      // If on one edge, we connect v_out to sink
      if (x == 0 || y == 0 || x == (width - 1) || y ==(height - 1)) {
        dinic.AddEdge(i + offset, sink, 1);
      }
    }

    // Adding the connection in the grid
    LL index_u, index_v;
    LL top_in, bottom_in, right_in, left_in;
    for(LL x = 0; x < S ; x++) {
      for(LL y = 0 ; y < A ; y++) {
        // + 1 for the source
        LL u_in = x + y * width +1 ;

        // Getting the in node of neighbours
        top_in = u_in + S;
        bottom_in = u_in - S;
        left_in = u_in - 1;
        right_in = u_in + 1;
        LL u_out = u_in + offset;
        if (y < height-1) {
          dinic.AddEdge(u_out, top_in, 1);
        }
        if (y > 0) {
          dinic.AddEdge(u_out, bottom_in, 1);
        }
        if (x > 0) {
          dinic.AddEdge(u_out, left_in, 1);
        }
        if (x < width -1) {
          dinic.AddEdge(u_out, right_in, 1);
        }
      }
    }

    // Connecting the source to the banks
    for(LL b = 0; b < B; b++) {
      cin >> x >> y;
      // 1 indexed to 0 indexed
      x--; y--;
      int index = x * width + y  +1 ;
      dinic.AddEdge(source, index, 1);
    }

    LL res = dinic.MaxFlow(source, sink);
    if (res == B) {
      std::cout << "possible\n";
    }
    else {
      std::cout << "not possible\n";
    }
  }
  return 0;
}

// END CUT
