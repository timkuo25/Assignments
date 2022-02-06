#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <set>

using namespace std;

struct Node{
    int d;
    int f;
    int pi;
    string color;
};

void DFS_visit(vector<vector<int>>&g, vector<Node> &g_nodes, int u, int &time, vector<int> *v = nullptr){
    time++;
    g_nodes[u].d = time;
    g_nodes[u].color = "gray";

    if (v != nullptr) {
        v->push_back(u);
    }
    for (int i = 0; i < g[u].size(); i++){
        if (g_nodes[g[u][i]].color == "white"){
            g_nodes[g[u][i]].pi = u;
            if (v != nullptr) {
                DFS_visit(g, g_nodes, g[u][i], time, v);
            }
            else DFS_visit(g, g_nodes, g[u][i], time);
        }
    }
    g_nodes[u].color = "black";
    time++;
    g_nodes[u].f = time;
}

void DFS(vector<vector<int>> &g, vector<Node> &g_nodes, int &time){
    for (int i = 0; i < g_nodes.size(); i++){
        g_nodes[i].color = "white";
        g_nodes[i].pi = INT32_MIN;
    }
    time = 0;
    for (int i = 0; i < g_nodes.size(); i++){
        if (g_nodes[i].color == "white")
            DFS_visit(g, g_nodes, i, time);
    }
}

void find_SCC(vector<vector<int>> &g, vector<Node> &g_nodes, int &time, vector<int> &order, vector<vector<int>> &SCC){
    for (int i = 0; i < g_nodes.size(); i++){
        g_nodes[i].color = "white";
        g_nodes[i].pi = INT32_MIN;
    }
    time = 0;
    
    for (int x : order){
        if (g_nodes[x].color == "white"){
            vector<int> v;
            DFS_visit(g, g_nodes, x, time, &v);
            SCC.push_back(v);
        }
    }
}

void cal_G_T(vector<vector<int>> &g, vector<vector<int>> &g_T){
    for (int i = 0; i < g.size(); i++){
        vector<int> v;
        g_T.push_back(v);
    }
    for (int i = 0; i < g.size(); i++){
        for (int j = 0; j < g[i].size(); j++){
            int k = g[i][j];
            g_T[k].push_back(i);
        }
    }
}

void DFS_dag_visit(vector<vector<int>>&g, vector<Node> &g_nodes, int u, vector<bool> &chosen, vector<int> &in_deg, vector<int> &out_deg, int& to){
    g_nodes[u].color = "gray";

    for (int i = 0; i < g[u].size(); i++){
        if (g_nodes[g[u][i]].color == "white"){
            if (out_deg[g[u][i]] == 0){
                g_nodes[g[u][i]].color = "gray";
                if (chosen[g[u][i]]) continue;
                
                to = g[u][i];
                return;
            }
            else DFS_dag_visit(g, g_nodes, g[u][i], chosen, in_deg, out_deg, to);
        }
    }
    return;
}

void DFS_dag(vector<vector<int>>&g, vector<Node> &g_nodes, vector<int> &in_v, vector<bool> &chosen, 
                vector<int> &in_deg, vector<int> &out_deg, vector<pair<int, int>> &v){

    for (int i = 0; i < g_nodes.size(); i++){
        g_nodes[i].color = "white";
    }

    for (auto x : in_v){
        int to = -1;
        
        DFS_dag_visit(g, g_nodes, x, chosen, in_deg, out_deg, to);

        if (to != -1){
            chosen[x] = true;
            chosen[to] = true;
            pair<int, int> p(x, to);
            v.push_back(p);
        }

        else if (in_deg[x] == 0 && out_deg[x] == 0){
            chosen[x] = true;
            pair<int, int> p(x, x);
            v.push_back(p);
        }
    }
}


int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t, flag, n, m;

    cin >> t >> flag;

    for(int T = 0; T < t; T++){
        cin >> n >> m;

        vector<vector<int>> g, g_T;
        vector<Node> g_nodes, dag_nodes;

        for (int i = 0; i < n; i++){
            vector<int> v;
            g.push_back(v);
            Node n;
            g_nodes.push_back(n);
        }

        for (int i = 0; i < m; i++){
            int from, to;
            cin >> from >> to;
            g[from - 1].push_back(to - 1);
        }

        int time = 0;

        DFS(g, g_nodes, time);
        cal_G_T(g, g_T);

        vector<pair<int, int>> num_f;
        for (int i = 0; i < g_nodes.size(); i++){
            pair<int, int> p(i, g_nodes[i].f);
            num_f.push_back(p);
        }

        sort(num_f.begin(), num_f.end(), [](auto &left, auto &right) {
            return left.second > right.second;
        });  

        vector<int> order;
        for (int i = 0; i < num_f.size(); i++){
            order.push_back(num_f[i].first);
        }
        
        vector<vector<int>> SCC;
        find_SCC(g_T, g_nodes, time, order, SCC);

        unordered_map<int, int> um;
        vector<vector<int>> dag;

        for (int i = 0; i < SCC.size(); i++){
            vector<int> v;
            if (flag) dag.push_back(v);
            for (int j = 0; j < SCC[i].size(); j++)
                um[SCC[i][j]] = i;
        }

        vector<int> in_deg(SCC.size(), 0);
        vector<int> out_deg(SCC.size(), 0);
        vector<vector<bool>> matrix;

        for(int i = 0; i < SCC.size(); i++){
            vector<bool> v(SCC.size(), false);
            matrix.push_back(v);
        }

        for (int i = 0; i < g.size(); i++){
            for (int j = 0; j < g[i].size(); j++){
                int k = g[i][j];
                if (um[i] != um[k]){
                    out_deg[um[i]]++;
                    in_deg[um[k]]++;
                    if (flag){
                        if (!matrix[um[i]][um[k]]){
                            dag[um[i]].push_back(um[k]);
                            matrix[um[i]][um[k]] = true;
                            matrix[um[k]][um[i]] = true;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < SCC.size(); i++){
            Node n;
            dag_nodes.push_back(n);
        }

        int in = 0, out = 0;
        vector<int> in_v, out_v;

        for (int i = 0; i < SCC.size(); i++){
            if (in_deg[i] == 0){
                in_v.push_back(i);
                in++;
            } 
            if (out_deg[i] == 0){
                out_v.push_back(i);
                out++;
            }
        }

        if (SCC.size() == 1) cout << 0 << "\n";
        else{
            cout << max(in, out) << "\n";
            if (flag){
                vector<bool> chosen(SCC.size(), false);
                vector<pair<int, int>> v;

                // arbitrary in and out point
                int in = INT32_MIN, out = INT32_MIN;
                
                DFS_dag(dag, dag_nodes, in_v, chosen, in_deg, out_deg, v);

                vector<int> SCC_t;
                for (int i = 0; i < SCC.size(); i++){
                    SCC_t.push_back(SCC[i][0]);
                }


                // for (auto x : dag){
                //     for (auto y : x){
                //         cout << y << " ";
                //     }
                //     cout << "\n";
                // }
                // for (auto x : v){
                //     cout << x.first << " " << x.second << "\n";
                // }

                // for (auto x : v){
                //     cout << SCC[x.first][0] << " " << SCC[x.second][0] << "\n";
                // }

                if (v.size() > 0){
                    int head = SCC_t[v[0].first], tail = SCC_t[v[0].second];
                    in = head, out = tail;
                    for (int i = 1; i < v.size(); i++){
                        cout << tail + 1 << " " << SCC_t[v[i].first] + 1 << "\n";
                        tail = SCC_t[v[i].second];
                    }
                    cout << tail + 1 << " " << head + 1 << "\n";
                }

                vector<int> in_left, out_left;

                for (auto x: in_v){
                    if (!chosen[x]) in_left.push_back(x);
                }
                for (auto x: out_v){
                    if (!chosen[x]) out_left.push_back(x);
                }

                if (in == INT32_MIN) {
                    in = in_left[0];
                }
                if (out == INT32_MIN) {
                    out = out_left[0];
                }

                // for (auto x: in_v)

                // cout << in_left.size() << " " << out_left.size();

                int cnt = 0;

                while(cnt < min(out_left.size(), in_left.size())){
                    cout << SCC_t[out_left[cnt]] + 1 << " " << SCC_t[in_left[cnt]] + 1 << "\n";
                    cnt++;
                }

                if (cnt == in_left.size()){
                    while (cnt < out_left.size()){
                        cout << SCC_t[out_left[cnt]] + 1 << " " << in + 1 << "\n";
                        cnt++;
                    }
                }
                else{
                    while (cnt < in_left.size()){
                        cout << out + 1 << " " << SCC_t[in_left[cnt]] + 1 << "\n";
                        cnt++;
                    }
                }
            }
        }  
    }

    return 0;
}