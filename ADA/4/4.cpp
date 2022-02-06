#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

long long RiskValue(const vector<long long> &b) {
    long long value = 0;
    for (int i = 0; i < b.size(); ++i) value += b[i];
    for (int i = 1; i < b.size(); ++i) value -= abs(b[i] - b[i - 1]);
    for (int i = 2; i < b.size(); ++i)
        value += b[i - 1] * ((b[i - 2] & b[i - 1]) ^ (b[i - 1] | b[i]) ^ (b[i - 2] + b[i]));
    return value;
}

long long risk(const vector<long long> &b, int l, int r){
    vector<long long> tmp;
    for (int i = l; i <= r; i++){
        tmp.push_back(b[i]);
    }

    return RiskValue(tmp);
}

long long value(const vector<long long> &v, int n, int m){
    vector<vector<long long>> dp(n, vector<long long>(m, INT32_MAX));
    
    for (int i = 0; i < n; i++){
        dp[i][0] = risk(v, 0, i);
    }

    for (int i = 1; i < m; i++){
        for (int j = i; j < n; j++){
            long long best = INT64_MAX;
            
            for (int k = 0;; k++){
                if (j - k <= i - 1) break;
                long long result = dp[j - k - 1][i - 1] + risk(v, j - k, j);
                if (result < best) best = result;
            }

            dp[j][i] = best;
        }
    }

    for (int i = 0; i < dp.size(); i++){
        for (int j = 0; j < dp[i].size(); j++){
            cout << dp[i][j] << " ";
        }
        cout << "\n";
    }

    return dp[n - 1][m - 1];
}

int main(){
    // cout << "[Sample 1] RiskValue: " 
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<long long> v(n);
    for (int i = 0; i < n; i++)
        cin >> v[i];

    value(v, n, m);

    // cout << value(v, n, m);
    long long best = INT64_MAX;
    vector<int> start;

    for (int i = 0; i < n; i++){
        start.push_back(i);
    }

    
    srand(2);
    random_shuffle(start.begin(), start.end());

    // for (int j = 0; j < min(n, 3 * n / m); j++){
    //     int x = start[j];
    //     vector<long long> tmp(n);

    //     for (int i = 0; i < n; i++){
    //         int index = (x + i) % n;
    //         tmp[i] = v[index];
    //     }

    //     long long result = value(tmp, n, m);
    //     // cout << result << " ";
    //     if (result < best) best = result;
    // }

    // cout << best;
    return 0;
}