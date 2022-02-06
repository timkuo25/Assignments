/*
References: Discussed with TAs
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

class Job{
    public:
        int s;
        int e;
        int x;
        int p;
        int remain;
        Job(int s, int e, int x, int p): s(s), e(e), x(x), p(p), remain(x){}
};

bool cmp_p (Job *j1, Job *j2){
    if (j1->p == j2->p) return j1->e < j2->e;
    return j1->p > j2->p;
}

bool cmp_s (Job *j1, Job *j2){
    if (j1->s == j2->s) return j1->e < j2->e;
    return j1->s < j2->s;
}

class Cmp{
    public:
        bool operator() (Job *j1, Job *j2){
            return j1->e > j2->e;
        }
};


int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    long long last_ans = 0, final_ans = 0;
    cin >> n;
    vector<Job*> v_out;
    
    int latest = 0;
    for (int i = 0; i < n; i++){
        int s, e, x, p;
        cin >> s >> e >> x >> p;
        if (e > latest) latest = e;
        Job *job = new Job(s, e, x, p);
        v_out.push_back(job);
    }
    
    sort(v_out.begin(), v_out.end(), cmp_p);
    
    for (int i = 0; i < v_out.size(); i++){
        long long ans = 0;
        vector<Job*> v;
        priority_queue<Job*, vector<Job*>, Cmp> q;

        for (int j = 0; j < i + 1; j++){
            Job *tmp = new Job(v_out[j]->s, v_out[j]->e, v_out[j]->x, v_out[j]->p);
            v.push_back(tmp);
        }

        sort(v.begin(), v.end(), cmp_s);

        int job_index = 0;
        int cur = v[0]->s;
        bool last = false;

        while(1){
            while(v[job_index]->s == cur){
                q.push(v[job_index]);
                if (job_index == v.size() - 1){
                    last = true;
                    break;
                } 
                job_index++;
            }

            if (!last){
                int stop = v[job_index]->s;

                while(1){
                    int work = min(q.top()->remain, q.top()->e - cur + 1);
                    if (cur + work > stop) work = stop - cur;

                    ans += work;
                    cur += work;
                    q.top()->remain -= work;
                    if (q.top()->remain == 0 || cur > q.top()->e) q.pop();
                    if (q.empty()) break;
                    if (cur == stop) break;
                }

                if (cur < stop) cur = stop;
                continue;
            }

            while(!q.empty()){
                int work = min(q.top()->remain, q.top()->e - cur + 1);
                ans += work;
                cur += work;
                q.pop();
            }
            break;
        }

        final_ans += v_out[i]->p * (ans - last_ans);
        last_ans = ans;
    }

    cout << final_ans;
    
    return 0;
}