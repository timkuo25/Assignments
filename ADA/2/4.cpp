/*
References: Discussed with TAs
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

class Pair{
    public:
        long long val;
        int left;
        int right;
        Pair(long long val, int left, int right): val(val), left(left), right(right){}
};

class Node{
    public:
        long long val;
        int left;
        int right;
        Node(long long val, int left, int right): val(val), left(left), right(right){}
};

class Cmp{
    public:
        bool operator() (Pair p1, Pair p2){
            return p1.val > p2.val;
        }
};


int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, flag;
    
    cin >> t >> flag;
    
    for (int T = 0; T < t; T++){
        int n;
        long long cur, tmp, sum1 = 0, sum2 = 0;
        vector<long long> v1, v2, ans;
        vector<Node> node_vec1, node_vec2;
        priority_queue<Pair, vector<Pair>, Cmp> q1, q2;
        vector<bool> available1, available2;

        cin >> n;
        cin >> cur;
        v1.push_back(cur);

        for (int i = 1; i < n; i++){
            cin >> tmp;
            if (cur * tmp < 0){
                cur = tmp;
                v1.push_back(cur);
            }
            else{
                if (tmp > cur){
                    cur = tmp;
                    v1.pop_back();
                    v1.push_back(cur);
                }
            }
        }

        for (int i = 0; i < v1.size(); i++){
            sum1 += v1[i];
        }

        for (int i = 0; i < n - v1.size(); i++){
            ans.push_back(0);
        }
        ans.push_back(sum1);

        if (v1.size() == 1){
            while (ans.size() != 0){
                cout << ans[ans.size() - 1] << (ans.size() == 1 ? "\n" : " ");
                ans.pop_back();
            }
            continue;
        }

        if (v1.back() <= v1[0]){
            for (int i = 0; i < v1.size() - 1; i++){
                v2.push_back(v1[i]);
            }
        }
        else{
            for (int i = 1; i < v1.size(); i++){
                v2.push_back(v1[i]);
            }
        }

        for (int i = 0; i < v2.size(); i++){
            sum2 += v2[i];
        }
        ans.push_back(sum2);

        if (v2.size() == 1){
            while (ans.size() != 0){
                cout << ans[ans.size() - 1] << (ans.size() == 1 ? "\n" : " ");
                ans.pop_back();
            }
            continue;
        }


        int head1 = 0, head2 = 0;
        int tail1 = v1.size() - 1, tail2 = v2.size() - 1;
        for (int i = 0; i < v2.size(); i++){
            available1.push_back(true);
            available2.push_back(true);
        }
        available1.push_back(true);

        for (int i = 0; i < v1.size(); i++){
            if (i == 0){
                Pair p(v1[i] + v1[i + 1], i, i + 1);
                q1.push(p);
                Node node(v1[i], v1.size() - 1, i + 1);
                node_vec1.push_back(node);
            }

            else if (i == v1.size() - 1){
                Pair p(v1[i] + v1[head1], i, head1);
                q1.push(p);
                Node node(v1[i], i - 1, head1);
                node_vec1.push_back(node);
            }

            else{
                Pair p(v1[i] + v1[i + 1], i, i + 1);
                q1.push(p);
                Node node(v1[i], i - 1, i + 1);
                node_vec1.push_back(node);
            }
        }

        for (int i = 0; i < v2.size(); i++){
            if (i == 0){
                Pair p(v2[i] + v2[i + 1], i, i + 1);
                q2.push(p);
                Node node(v2[i], v2.size() - 1, i + 1);
                node_vec2.push_back(node);
            }

            else if (i == v2.size() - 1){
                Pair p(v2[i] + v2[head1], i, head2);
                q2.push(p);
                Node node(v2[i], i - 1, head2);
                node_vec2.push_back(node);
            }

            else{
                Pair p(v2[i] + v2[i + 1], i, i + 1);
                q2.push(p);
                Node node(v2[i], i - 1, i + 1);
                node_vec2.push_back(node);
            }
        }

        
        int round = 0;
        while(1){
            Pair tmp = q1.top();
            sum1 -= tmp.val;
            ans.push_back(sum1);
            q1.pop();

            available1[tmp.left] = false;
            available1[tmp.right] = false;

            Pair v_push(node_vec1[node_vec1[tmp.left].left].val + node_vec1[node_vec1[tmp.right].right].val, node_vec1[tmp.left].left, node_vec1[tmp.right].right);
            q1.push(v_push);

            if (tmp.right == tail1 || tmp.left == tail1) tail1 = node_vec1[tmp.left].left;
            if (tmp.right == head1 || tmp.left == head1) head1 = node_vec1[tmp.right].right;
            node_vec1[node_vec1[tmp.left].left].right = node_vec1[tmp.right].right;
            node_vec1[node_vec1[tmp.right].right].left = node_vec1[tmp.left].left;
            
            while(!available1[q1.top().left] || !available1[q1.top().right]){
                q1.pop();
                if (q1.empty()) break;
            }

            round++;
            if (round == v2.size() - 1) break;

            Pair tmp2 = q2.top();
            sum2 -= tmp2.val;
            ans.push_back(sum2);
            q2.pop();

            available2[tmp2.left] = false;
            available2[tmp2.right] = false;

            Pair v_push2(node_vec2[node_vec2[tmp2.left].left].val + node_vec2[node_vec2[tmp2.right].right].val, node_vec2[tmp2.left].left, node_vec2[tmp2.right].right);
            q2.push(v_push2);

            if (tmp2.right == tail2 || tmp2.left == tail2) tail2 = node_vec2[tmp2.left].left;
            if (tmp2.right == head2 || tmp2.left == head2) head2 = node_vec2[tmp2.right].right;
            node_vec2[node_vec2[tmp2.left].left].right = node_vec2[tmp2.right].right;
            node_vec2[node_vec2[tmp2.right].right].left = node_vec2[tmp2.left].left;
            
            while(!available2[q2.top().left] || !available2[q2.top().right]){
                q2.pop();
                if (q2.empty()) break;
            }
            round++;
            if (round == v2.size() - 1) break;
        }
        
        while (ans.size() != 0){
            cout << ans[ans.size() - 1] << (ans.size() == 1 ? "\n" : " ");
            ans.pop_back();
        }
    }

    return 0;
}