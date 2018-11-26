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

typedef pair<int, int > ii;


struct pos {
    float x, y;
};

pos robots[205];
pos holes[205];

int times[3] = {5, 10, 20};
float v_robot = 10.0;

int case_n = 0;
int main() {

    int n_robots;
    int n_holes;
    while (1) {
        case_n++;

        cin >> n_robots;
        if (n_robots == 0) {
            return 0;
        }

        cout << "Scenario " << case_n << "\n";
        for (int i = 0; i < n_robots; i++) {
            cin >> robots[i].x;
            cin >> robots[i].y;
        }

        cin >> n_holes;
        for (int i = 0; i < n_holes; i++) {
            cin >> holes[i].x;
            cin >> holes[i].y;
        }

        // Computing second needed per pair
        vector<pair<float, ii> > seconds_needed;
        for (int robot = 0; robot < n_robots; robot++) {
            for (int hole = 0; hole < n_holes; hole++) {
                ii r_h = make_pair(robot, hole + n_robots);
                float diff_x = robots[robot].x - holes[hole].x;
                float diff_y = robots[robot].y - holes[hole].y;
                float dist = sqrt(diff_x * diff_x + diff_y * diff_y);
                float sec_needed = dist / v_robot;
                seconds_needed.push_back(make_pair(sec_needed, r_h));
            }
        }

        // Sorting over the seconds
        sort(seconds_needed.begin(), seconds_needed.end());

        int source = n_robots + n_holes;
        int target = source + 1;
        int time;


        // Constructing the graph
        Dinic solver(2 + n_robots + n_holes);

        for (int i = 0; i < n_robots; i++) {
            solver.AddEdge(source, i, 1);
        }

        for (int i = 0; i < n_holes; i++) {
            solver.AddEdge(i + n_robots, target, 1);
        }

        int index_second = 0;

        // We just add the new link when possible
        // The Graph is reset to solve it again
        int res = 0;
        for (int t = 0; t < 3; t++) {
            // solver.reset();
            time = times[t];

            float sec;
            int r, h;
            while (index_second < seconds_needed.size()) {
                auto truc = seconds_needed[index_second];
                sec = truc.first;
                if (sec > time) {
                    break;
                }
                r = truc.second.first;
                h = truc.second.second;
                solver.AddEdge(r, h, 1);
                index_second++;
            }

            res += solver.GetMaxFlow(source, target);
            cout << "In " << time << " seconds " << res <<
            " robot(s) can escape\n";
        }
        cout << "\n";
    }

    return 0;
}
