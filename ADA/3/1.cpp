/*
References:
    Discussed with TAs
    https://www.quora.com/How-can-you-minimize-the-diameter-of-a-tree-if-you-can-change-at-most-one-edge-of-the-tree
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int BFS(vector<vector<int>> &g, int s, vector<vector<int>> *check = nullptr, bool calc = false, int ignore = INT32_MIN){
    vector<bool> visited;
    vector<int> d;
    vector<int> pi;
    int result;

    for (int i = 0; i < g.size(); i++){
        visited.push_back(false);
        d.push_back(INT32_MIN);
        pi.push_back(INT32_MIN);
    }
    
    if (ignore != INT32_MIN){
        visited[ignore] = true;
    }


    visited[s] = true;
    d[s] = 0;
    queue<int> q;
    q.push(s);
    while(!q.empty()){
        int u = q.front();
        result = u;
        q.pop();
        for (int i = 0; i < g[u].size(); i++){
            int v = g[u][i];
            if (!visited[v]){
                visited[v] = true;
                d[v] = d[u] + 1;
                pi[v] = u;
                q.push(v);
            }
        }
    }

    // if (ignore == 3) cout << result << " ";
    if ((check != nullptr) || calc){
        int cur = result, next = pi[result], d = 0;
        while(next != INT32_MIN){
            vector<int> c;
            c.push_back(cur);
            c.push_back(next);
            if (!calc) check->push_back(c);
            cur = next;
            next = pi[cur];
            d++;
        }
        if (calc) return d;
    }

    return result;
}

int ceil(int n1, int n2){
    if (n1 % n2 == 0) return n1 / n2;
    return n1 / n2 + 1;
}

int calc_result(vector<vector<int>> &g, int ig_1, int ig_2){
    int side, d1, d2;
    side = BFS(g, ig_1, nullptr, false, ig_2);
    d1 = BFS(g, side, nullptr, true, ig_2);
    side = BFS(g, ig_2, nullptr, false, ig_1);
    d2 = BFS(g, side, nullptr, true, ig_1);

    return max({d1, d2, 1 + ceil(d1, 2) + ceil(d2, 2)});
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;

    vector<vector<int>> g;
    vector<vector<int>> check;

    for (int i = 0; i < n; i++){
        vector<int> v;
        g.push_back(v);
    }

    for (int i = 0; i < n - 1; i++){
        int from, to;
        cin >> from >> to;
        g[from - 1].push_back(to - 1);
        g[to - 1].push_back(from - 1);
    }

    int side = BFS(g, 0);
    int side_b = BFS(g, side, &check);

    int best_result = INT32_MAX;
    
    if (check.size() > 7){
        vector<vector<int>> slice;
        int tmp = check.size() / 2;
        for (int i = -1; i <= 1; i++){
            slice.push_back(check[tmp + i]);
        }
        for (int i = 0; i < slice.size(); i++){
            int result = calc_result(g, slice[i][0], slice[i][1]);
            if (result < best_result) best_result = result;
        }
    }
    else{
        for (int i = 0; i < check.size(); i++){
            int result = calc_result(g, check[i][0], check[i][1]);
            if (result < best_result) best_result = result;
        }
    }


    cout << best_result;

    return 0;
}