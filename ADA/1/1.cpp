/*
References:
1. https://openhome.cc/Gossip/AlgorithmGossip/HanoiTower.htm
2. https://github.com/Otrebus/timus/blob/master/1054.cpp
*/

#include <iostream>
#include <vector>
#define MOD 998244353
using namespace std;

vector<int> v;
bool violate = false;

long long steps(int n, int from, int tmp, int to){
    if (violate) return 0;
    if (n == 1){
        if (v[n - 1] == from) return 1;
        else if (v[n - 1] == to) return 0;
        else{
            violate = true;
            return 0;
        }
    }
    
    if (v[n - 1] == from) {
        long long num = 1;
        int times = (n - 1) / 31;
        int remain = (n - 1) % 31;

        for (int i = 0; i < times; i++){
            num <<= 31;
            num %= MOD;
        }
        num <<= remain;
        num %= MOD;

        return (num + steps(n - 1, from, to, tmp)) % MOD;
    }
    else if (v[n - 1] == to) return steps(n - 1, tmp, from, to);
    else{
        violate = true;
        return 0;
    }
    
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, tmp1, tmp2;

    cin >> n;
    for (int i = 0; i < n; i++)
        v.push_back(-1);
    
    for (int i = 0; i < 3; i++){
        cin >> tmp1;
        for (int j = 0; j < tmp1; j++){
            cin >> tmp2;
            v[tmp2 - 1] = i;
        }
    }
    
    long long result = steps(n, 0, 1, 2);
    cout << (violate ? -1 : result);
    
    return 0;
}