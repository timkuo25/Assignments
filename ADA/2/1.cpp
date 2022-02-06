/*
References: Discussed with TAs
*/

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;

    for (int T = 0; T < t; T++){
        int n, m, k;
        long long tmp;
        cin >> n >> m >> k;

        if (k == 1){
            vector<vector<long long>> matrix, dp, tri;
            vector<vector<vector<int>>> pos, tri_pos;

            for (int i = 0; i < n; i++){
                vector<long long> v;
                for (int j = 0; j < m; j++){
                    string s;
                    cin >> s;
                    if (s == "X") tmp = INT64_MIN;
                    else tmp = stoll(s);
                    v.push_back(tmp);
                }
                matrix.push_back(v);

                vector<long long> v1;
                for (int j = 0; j < m; j++){
                    v1.push_back(INT64_MIN);
                }
                dp.push_back(v1);
                tri.push_back(v1);

                vector<vector<int>> v2;
                for (int j = 0; j < m; j++){
                    vector<int> v3;
                    v3.push_back(-1);
                    v3.push_back(-1);
                    v2.push_back(v3);
                }
                pos.push_back(v2);
                tri_pos.push_back(v2);
            }

            dp[0][0] = 0;
            pos[0][0][0] = 0;
            pos[0][0][1] = 0;
            for (int i = 1; i < n; i++){
                if (matrix[i][0] == INT64_MIN) dp[i][0] = INT64_MIN;
                else if (dp[i - 1][0] == INT64_MIN){
                    dp[i][0] = INT64_MIN;
                }
                else {
                    dp[i][0] = dp[i - 1][0] + matrix[i][0];
                    pos[i][0][0] = i - 1;
                    pos[i][0][1] = 0;
                }
            }

            for (int i = 1; i < m; i++){
                if (matrix[0][i] == INT64_MIN) dp[0][i] = INT64_MIN;
                else if (dp[0][i - 1] == INT64_MIN){
                    dp[0][i] = INT64_MIN;
                }
                else {
                    dp[0][i] = dp[0][i - 1] + matrix[0][i];
                    pos[0][i][0] = 0;
                    pos[0][i][1] = i - 1;
                }
            }

            for (int i = 1; i < n; i++){
                for (int j = 1; j < m; j++){
                    long long up = dp[i - 1][j], left = dp[i][j - 1];

                    if (matrix[i][j] == INT64_MIN) dp[i][j] = INT64_MIN;
                    else if (up == INT64_MIN && left == INT64_MIN){
                        dp[i][j] = INT64_MIN;
                    }
                    else{
                        if (up >= left){
                            dp[i][j] = dp[i - 1][j] + matrix[i][j];
                            pos[i][j][0] = i - 1;
                            pos[i][j][1] = j;
                        }
                        else{
                            dp[i][j] = dp[i][j - 1] + matrix[i][j];
                            pos[i][j][0] = i;
                            pos[i][j][1] = j - 1;
                        }
                    }

                }
            }

            if (dp[n - 1][m - 1] == INT64_MIN) cout << "Impassable\n";
            else{
                cout << "Passable\n" << dp[n - 1][m - 1] << "\n";
                vector<vector<int>> print_pos;
                int cur_x = n - 1, cur_y = m - 1;

                vector<int> last_pos{cur_x, cur_y};
                print_pos.push_back(last_pos);
                
                while(1){
                    int x, y;
                    x = pos[cur_x][cur_y][0], y = pos[cur_x][cur_y][1];
                    print_pos.push_back(pos[cur_x][cur_y]);
                    cur_x = x, cur_y = y;
                    if (cur_x == 0 && cur_y == 0) break;
                }
                cout << print_pos.size() << "\n";
                
                while (print_pos.size() != 0){
                    cout << print_pos.back()[0] + 1 << " " << print_pos.back()[1] + 1 << "\n";
                    print_pos.pop_back();
                }
            }
            continue;
        }

        int num = k - 1;
        vector<int> log;
        log.push_back(k - 1);
        while(1){
            num = num * 11 / 16;
            log.push_back(num);
            if (num == 0) break;
        }

        int c = log.size();
        
        vector<vector<long long>> matrix;
        vector<vector<vector<long long>>> dp;

        for (int i = 0; i < n; i++){
            vector<long long> v;
            for (int j = 0; j < m; j++){
                string s;
                cin >> s;
                if (s == "X") tmp = INT64_MIN;
                else tmp = stoll(s);
                v.push_back(tmp);
            }
            matrix.push_back(v);

            vector<vector<long long>> v2;
            for (int j = 0; j < m; j++){
                vector<long long> v3;
                for (int l = 0; l < c; l++)
                    v3.push_back(INT64_MIN);
                v2.push_back(v3);
            }
            dp.push_back(v2);
        }
        
        for (int i = 0; i < c; i++){
            dp[0][0][i] = 0;
        }

        
        for (int i = 1; i < n; i++){
            long long up = dp[i - 1][0][c - 1];
            if (up == INT64_MIN || matrix[i][0] == INT64_MIN) dp[i][0][0] = INT64_MIN;
            else dp[i][0][0] = up + matrix[i][0];

            for (int K = 1; K < c; K++){
                int obj = i - (log[c - K - 1] - log[c - K]);
               
                long long up = (obj < 0) ? INT64_MIN : dp[obj][0][K - 1];
                long long here = dp[i][0][K - 1];
                dp[i][0][K] = max(up, here);
            }
        }

        
        for (int i = 1; i < m; i++){
            long long left = dp[0][i - 1][c - 1];
            if (left == INT64_MIN || matrix[0][i] == INT64_MIN) dp[0][i][0] = INT64_MIN;
            else dp[0][i][0] = left + matrix[0][i];

            for (int K = 1; K < c; K++){
                int obj = i - (log[c - K - 1] - log[c - K]);
                long long left = (obj < 0) ? INT64_MIN : dp[0][obj][K - 1];
                long long here = dp[0][i][K - 1];
                dp[0][i][K] = max(left, here);
            }
        }

        for (int i = 1; i < n; i++){
            for (int j = 1; j < m; j++){
                long long up = dp[i - 1][j][c - 1], left = dp[i][j - 1][c - 1];
                if ((up == INT64_MIN && left == INT64_MIN) || matrix[i][j] == INT64_MIN) dp[i][j][0] = INT64_MIN;
                else dp[i][j][0] = max(up, left) + matrix[i][j];

                for (int K = 1; K < c; K++){
                    int obj_up = i - (log[c - K - 1] - log[c - K]);
                    int obj_left = j - (log[c - K - 1] - log[c - K]);
                    long long up = (obj_up < 0) ? INT64_MIN : dp[obj_up][j][K - 1]; 
                    long long left = (obj_left < 0) ? INT64_MIN : dp[i][obj_left][K - 1];
                    long long here = dp[i][j][K - 1];                    
                    
                    dp[i][j][K] = max(up, max(left, here));
                }
            }
        }
        
        // draw matrix
        // for (int i = 0; i < n; i++){
        //     for (int j = 0; j < m; j++)
        //         cout << matrix[i][j] << "\t";
        //     cout << "\n";
        // }
        
        // draw dp
        // for (int i = 0; i < n; i++){
        //     for (int j = 0; j < m; j++)
        //         cout << dp[i][j][0] << "\t";
        //     cout << "\n";
        // }

        if (dp[n - 1][m - 1][0] == INT64_MIN) cout << "Impassable\n";
        else{
            cout << "Passable\n" << dp[n - 1][m - 1][0] << "\n";
            vector<int> pos_x;
            vector<int> pos_y;

            pos_x.push_back(n);
            pos_y.push_back(m);
            int cur_x = n - 1, cur_y = m - 1;

            while(!(cur_x == 0 &3& cur_y == 0)){
                int steps = k;
                bool found = false;
                for (int i = 0; i < steps; i++){
                    int x = cur_x - i - 1, y = cur_y;
                    if (x < 0 || y < 0) break;
                    long long cur = dp[x][y][0];

                    if (cur + matrix[cur_x][cur_y] == dp[cur_x][cur_y][0]){
                        pos_x.push_back(x + 1);
                        pos_y.push_back(y + 1);
                        cur_x = x;
                        cur_y = y;
                        found = true;
                        break;
                    }
                }
                if (found) continue;
                
                while(steps >= 0){
                    for (int i = 0; i < steps; i++){
                        int x = cur_x - i, y = cur_y - (k - steps + 1);
                        if (y < 0 || x < 0) continue;
                        long long cur = dp[x][y][0];

                        if (cur + matrix[cur_x][cur_y] == dp[cur_x][cur_y][0]){
                            pos_x.push_back(x + 1);
                            pos_y.push_back(y + 1);
                            cur_x = x;
                            cur_y = y;
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                    steps--;
                }
            }

            cout << pos_x.size() << "\n";
            while(1){
                cout << pos_x.back() << " " << pos_y.back() << "\n";
                pos_x.pop_back();
                pos_y.pop_back();
                if (pos_x.size() == 0) break;
            }
               
            continue;
        }
    }

    return 0;
}