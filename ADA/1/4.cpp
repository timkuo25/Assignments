/*
References:
Discussed with b05504066 (Lee Min Han)
*/

#include <iostream>
#include <climits>
#include <vector>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int cases, flag, n;
    cin >> cases >> flag;
    
    for (int i = 0; i < cases; i++){
        cin >> n;
        long long *sweet = new long long[n];
        vector<long long> alt;
        vector<int> pos;
        vector<long long> alt_s;
        vector<int> pos_s;

        for (int j = 0; j < n; j++){
            cin >> sweet[j];
        }

        long long neg = LLONG_MIN;
        int k = 0, index;
        bool no_positive = false;

        while (sweet[k] <= 0){
            if (sweet[k] > neg){
                neg = sweet[k];
                index = k;
            }
            if (k + 1 == n){
                no_positive = true;
                break;
            }
            k++;
        }

        if (no_positive){
            cout << neg << "\n";
            if (flag) cout << "1 " << index + 1 << "\n";
            delete [] sweet;
            continue;
        }

        long long cur = sweet[k];
        int cur_pos = k;
        for (int j = k + 1; j < n; j++){
            if (sweet[j] == 0){
                if (j == n - 1) continue;

                if (cur > 0){
                    alt.push_back(cur);
                    pos.push_back(cur_pos);
                }

                alt.push_back(0);
                pos.push_back(j);
                cur = 0;

                while(1){
                    if (j == n - 1) break;
                    j++;
                    if (sweet[j] > 0){
                        cur = sweet[j];
                        cur_pos = j;
                        break;
                    } 
                }
            }
            else if (cur * sweet[j] < 0){
                alt.push_back(cur);
                pos.push_back(cur_pos);
                cur = sweet[j];
                cur_pos = j;
            }
            else if (cur < sweet[j]){
                cur = sweet[j];
                cur_pos = j;
            }
        }
        if (cur > 0){
            alt.push_back(cur);
            pos.push_back(cur_pos);
        }
        if (alt.back() <= 0){
            alt.pop_back();
            pos.pop_back();
        }

        if (alt.size() == 1) {
            cout << alt[0] << "\n";
            if (flag) cout << 1 << " " << pos[0] + 1 << "\n";
            delete [] sweet;
            continue;
        }

        alt_s.push_back(alt[0]);
        pos_s.push_back(pos[0]);

        int mid = 1;
        int right = 2;

        while (right < alt.size()){
            if (alt_s.back() + alt[mid] > 0 and alt[right] + alt[mid] > 0){
                alt_s.push_back(alt[mid]);
                alt_s.push_back(alt[right]);
                pos_s.push_back(pos[mid]);
                pos_s.push_back(pos[right]);
                
                right += 2;
                mid = right - 1;
            }
            else if (alt_s.back() + alt[mid] <= alt[right] + alt[mid]){
                alt_s.pop_back();
                pos_s.pop_back();
                alt_s.push_back(alt[right]);
                pos_s.push_back(pos[right]);

                right += 2;
                mid = right - 1;
            }
            else{
                if (right + 1 == alt.size()) break;
                if (alt[right + 1] < alt[mid]){
                    right += 2;
                }
                else{
                    right += 2;
                    mid = right - 1;
                }
            }
        }

        long long sum = 0;
        for (int j = 0; j < alt_s.size(); j++){
            sum += alt_s[j];
        }

        cout << sum << "\n";
        if (flag){
            cout << pos_s.size() << " ";
            for (int j = 0; j < pos_s.size() - 1; j++){
                cout << pos_s[j] + 1 << " ";
            }
            cout << pos_s.back() + 1 << "\n";
        }

        delete [] sweet;
    }

    return 0;
}