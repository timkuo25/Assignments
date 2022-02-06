/*
    References: Discussed with TAs
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

struct Node{
    long long num;
    long long d;
    long long pi;
    long long edge_index;
    long long last_d;
};

struct Edge{
    long long from;
    long long to;
    long long weight;
    long long index;
};

struct Cmp {
    bool operator()(Node const& n1, Node const& n2){
        if (n1.d == n2.d) {
            if (n1.last_d == n2.last_d) return n1.num > n2.num;
            return n1.last_d > n2.last_d;
        }
        return n1.d > n2.d;
    }
};

long long find_set(vector<long long> &d_s, long long obj){
    long long set_num = obj;
    long long next = d_s[set_num];

    while(set_num != next){
        d_s[set_num] = d_s[next];
        set_num = next;
        next = d_s[set_num];
    }
    return set_num;
}

void kruskal(vector<vector<Edge>> &g, vector<Edge> &edges, vector<Edge> &result){
    vector<long long> d_s;
    for (int i = 0; i < g.size(); i++){
        d_s.push_back(i);
    }

    sort(edges.begin(), edges.end(), 
        [](const Edge &a, const Edge &b) {
            if (a.weight == b.weight) return a.index < b.index;
            return a.weight < b.weight;
    });


    for (auto x: edges){
        // cout << find_set(d_s, x.from) << " " << find_set(d_s, x.to) << "\n";
        if (find_set(d_s, x.from) != find_set(d_s, x.to)){
            result.push_back(x);
            //union
            if (find_set(d_s, x.to) > find_set(d_s, x.from)) d_s[find_set(d_s, x.to)] = d_s[find_set(d_s, x.from)];
            else d_s[find_set(d_s, x.from)] = d_s[find_set(d_s, x.to)];
        }
        // for (auto x : d_s){
        //     cout << x << " ";
        // }
        // cout << "\n";
    }
}

void dijkstra(vector<vector<Edge>> &g, vector<Node> &g_nodes, long long s){
    for (int i = 0; i < g_nodes.size(); i++){
        g_nodes[i].d = INT64_MAX;
        g_nodes[i].pi = INT64_MIN;
        g_nodes[i].edge_index = INT64_MIN;
        g_nodes[i].last_d = INT64_MAX;
    }
    g_nodes[s].d = 0;
    g_nodes[s].last_d = 0;

    vector<bool> visited(g_nodes.size(), false);
    priority_queue<Node, vector<Node>, Cmp> q;
    for (auto x: g_nodes){
        q.push(x);

        // cout << x.num << " " << x.d << "\n";
    }

    long long min_weight = INT64_MAX;
    while(!q.empty()){
        long long u = q.top().num;
        q.pop();
        if (visited[u]) continue;

        visited[u] = true;
        for (int i = 0; i < g[u].size(); i++){
            long long v = g[u][i].to;
            if (g_nodes[v].d > g_nodes[u].d + g[u][i].weight){
                Node n;
                n.num = v;
                n.d = g_nodes[u].d + g[u][i].weight;
                n.pi = u;
                n.edge_index = g[u][i].index;
                n.last_d = g[u][i].weight;
                q.push(n);

                g_nodes[v].d = g_nodes[u].d + g[u][i].weight;
                g_nodes[v].pi = u;
                g_nodes[v].edge_index = g[u][i].index;
                g_nodes[v].last_d = g[u][i].weight;
            }
            else if (g_nodes[v].d == g_nodes[u].d + g[u][i].weight){
                if (g_nodes[v].last_d > g[u][i].weight){
                    Node n;
                    n.num = v;
                    n.d = g_nodes[u].d + g[u][i].weight;
                    n.pi = u;
                    n.edge_index = g[u][i].index;
                    n.last_d = g[u][i].weight;
                    q.push(n);

                    g_nodes[v].d = g_nodes[u].d + g[u][i].weight;
                    g_nodes[v].pi = u;
                    g_nodes[v].edge_index = g[u][i].index;
                    g_nodes[v].last_d = g[u][i].weight;
                }
            }
        }
    }
}



int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<Edge>> g;
    vector<Edge> edges;

    for (int i = 0; i < n; i++){
        vector<Edge> tmp;
        g.push_back(tmp);
    }

    for (int i = 0; i < m; i++){
        long long from, to, weight;
        cin >> from >> to >> weight;

        if (from > to){
            long long tmp = to;
            to = from;
            from = tmp;
        }

        Edge e = {from - 1, to - 1, weight, i};
        Edge e2 = {to - 1, from - 1, weight, i};

        g[from - 1].push_back(e);
        g[to - 1].push_back(e2);
        edges.push_back(e);

    }

    vector<Edge> kruskal_result;
    kruskal(g, edges, kruskal_result);

    // cout << "\n";
    // for (auto x: edges){
    //     cout << x.from << " " << x.to << " " << x.weight << "\n";
    // }

    // cout << "\n";
    // for (auto x: kruskal_result){
    //     cout << x.from << " " << x.to << " " << x.weight << "\n";
    // }

    vector<Node> g_nodes;
    for (int i = 0; i < g.size(); i++){
        Node n;
        n.num = i;
        g_nodes.push_back(n);
    }
    dijkstra(g, g_nodes, 0);


    // for (auto x : g){
    //     for (auto y : x){
    //         cout << y.to << " ";
    //     }
    //     cout << "\n";
    // }
    // for (auto x: g_nodes){
    //     cout << x.edge_index << " ";
    // }
    // cout << "\n";
    // for (auto x: g_nodes){
    //     cout << x.d << " ";
    // }

    long long sum_MST = 0, sum_SPT = 0;
    for (auto x: g_nodes){
        sum_SPT += x.last_d;
    }

    for (auto x: kruskal_result){
        sum_MST += x.weight;
    }

    if (sum_MST == sum_SPT){
        cout << "Yes\n";
        // for (int i = 0; i < kruskal_result.size(); i++){
        //     cout << kruskal_result[i].index + 1 << (i == kruskal_result.size() - 1 ? "\n" : " ");
        // }
        for (int i = 1; i < g_nodes.size(); i++){
            cout << g_nodes[i].edge_index + 1 << " ";
        }
    }
    else cout << "No\n";

    return 0;
}