/*
References: Discussed with TAs
*/

#include <iostream>
#include <algorithm>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, g1, g2;
    cin >> n;
    long long *a = new long long[n];
    long long **dp = new long long*[n];
    long long **gcd_dict = new long long*[n];

    for (int i = 0; i < n; i++){
        cin >> a[i];
        gcd_dict[i] = new long long[n];
        dp[i] = new long long[n];
        dp[i][i] = -1;        
    }

    // calc gcd
    for (int i = 0; i < n - 1; i++){
        for (int j = i + 1; j < n; j++){
            long long gcd_n = __gcd(a[i], a[j]);
            gcd_dict[i][j] = gcd_n;
        }
    }

    for (int i = 1; i < n; i++){
        g1 = gcd_dict[i - 1][i];
        dp[i - 1][i] = (g1 == 1) ? -1 : g1;
    }

    if (n == 2){
        cout << dp[0][1];
        return 0;
    }

    for (int i = 2; i < n; i++){
        dp[i - 2][i] = (gcd_dict[i - 2][i - 1] == 1 || gcd_dict[i - 1][i] == 1 || gcd_dict[i - 2][i] == 1) ? -1 : gcd_dict[i - 2][i - 1] + gcd_dict[i - 1][i];
    }

    for (int start = 3; start < n; start++){
        for (int o = start; o < n; o++){
            int L = o - start, R = o;
            long long num1 = -1, num2 = -1, num3 = -1;
            
            //num1
            for (int i = L + 1; i < R - 1; i++){
                long long cur_num1 = (dp[L][i] == -1 || dp[i + 1][R] == -1) ? -1 : dp[L][i] + dp[i + 1][R];
                if (cur_num1 > num1) num1 = cur_num1;
            }

            //num2
            g1 = gcd_dict[L][R];
            num2 = (g1 == 1 || dp[L + 1][R - 1] == -1) ? -1 : dp[L + 1][R - 1] + g1;

            //num3
            for (int i = L + 1; i < R; i++){
                long long cur_num3 = -1;
                if (i == L + 1){
                    cur_num3 = (gcd_dict[L][i] == 1 || gcd_dict[i][R] == 1 || gcd_dict[L][R] == 1 || dp[i + 1][R - 1] == -1) ? -1 : dp[i + 1][R - 1] + gcd_dict[L][i] + gcd_dict[i][R];
                }
                else if (i == R - 1){
                    cur_num3 = (gcd_dict[L][i] == 1 || gcd_dict[i][R] == 1 || gcd_dict[L][R] == 1 || dp[L + 1][i - 1] == -1) ? -1 : dp[L + 1][i - 1] + gcd_dict[L][i] + gcd_dict[i][R];
                }
                else{
                    cur_num3 = (gcd_dict[L][i] == 1 || gcd_dict[i][R] == 1 || gcd_dict[L][R] == 1 || dp[i + 1][R - 1] == -1 || dp[L + 1][i - 1] == -1) ? -1 : dp[i + 1][R - 1] + dp[L + 1][i - 1] + gcd_dict[L][i] + gcd_dict[i][R];
                }

                if (cur_num3 > num3) num3 = cur_num3;
            }
            
            long long max_num = num1;
            if (num2 > max_num) max_num = num2;
            if (num3 > max_num) max_num = num3; 
            dp[L][R] = max_num;
        }
    }

    cout << dp[0][n - 1];
    // draw dp
    // for (int i = 0; i < n; i++){
    //     for (int j = 0; j < n; j++){
    //         cout << dp[i][j] << "\t";
    //     }
    //     cout << "\n";
    // }
    

    return 0;
}