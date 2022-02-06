/*
    References: Discussed with TAs
*/

#include <iostream>
#include <vector>
#include <map>

using namespace std;

int find_set(vector<int> &d_s, int element){
    int set_num = element;
    int next = d_s[set_num];

    while(set_num != next){
        d_s[set_num] = d_s[next];
        set_num = next;
        next = d_s[set_num];
    }
    return set_num;
}


int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, q;
    cin >> n >> q;

    map<int, int> m;
    vector<int> set_pos;
	vector<long long> set_len;
    vector<int> d_s; //disjoint set
    vector<int> cat_set;
	
	for (int i = 0; i < n + 1; i++){
		set_pos.push_back(-1);
        set_len.push_back(-1);
		d_s.push_back(-1);
        cat_set.push_back(-1);
	}
    
    for (int i = 1; i <= n; i++){
        int pos;
        cin >> pos;

        auto it = m.find(pos);
        if (it == m.end()){
            m[pos] = i;
            set_pos[i] = pos;
            set_len[i] = 1;
            d_s[i] = i;
            cat_set[i] = i;
        }
        else{
            set_len[m[pos]]++;
            d_s[i] = m[pos];
            cat_set[i] = m[pos];
        }

    }
    
    long long result = 0;
    for (auto x : set_len){
        if (x >= 2) result += x * (x - 1) / 2;
    }

    for (int i = 0; i < q; i++){
        int mode, n1, n2;
        cin >> mode >> n1 >> n2;

        if (mode == 2){
            int cat_num = n1, to = n2;
            int set_num = find_set(d_s, cat_set[cat_num]);


            if (set_len[set_num] >= 2) result -= set_len[set_num] * (set_len[set_num] - 1) / 2;
            set_len[set_num]--;
            if (set_len[set_num] >= 2) result += set_len[set_num] * (set_len[set_num] - 1) / 2;
        

            auto it = m.find(to);
            if (it == m.end()){
                m[to] = d_s.size();
                set_pos.push_back(to);
                set_len.push_back(0);
                d_s.push_back(d_s.size());

                it = m.find(to);
            }

            cat_set[cat_num] = it->second;

            if (set_len[it->second] >= 2) result -= set_len[it->second] * (set_len[it->second] - 1) / 2;
            set_len[it->second]++;
            if (set_len[it->second] >= 2) result += set_len[it->second] * (set_len[it->second] - 1) / 2;

        }

        else if (mode == 1){
            int center = n1, rad = n2;
            int cnt = 0;
            bool new_set = false;
            bool erase_cur = true;
            
            auto it = m.find(center);
            if(it == m.end()){
                m[center] = d_s.size();
                set_pos.push_back(center);
                set_len.push_back(0);
                d_s.push_back(d_s.size());

                it = m.find(center);
            }
            else erase_cur = false;
            
            
            auto it2 = it, it3 = it;
            if (set_len[it->second] >= 2) result -= set_len[it->second] * (set_len[it->second] - 1) / 2;
            
            while (1){
                if (it == m.begin()) break;
                it--;
                if (it->first < center - rad){
                	it++;
					break;	
				}
                erase_cur = false;
                
                cnt += set_len[it->second];
                if (set_len[it->second] >= 2) result -= set_len[it->second] * (set_len[it->second] - 1) / 2;
                set_len[it->second] = 0;
                
                d_s[it->second] = it3->second;
            }
            if (it != it3){
            	m.erase(it, it3);
			} 

            while (1){
                if (it2 == prev(m.end())) break;
                it2++;
                if (it2->first > center + rad){
					it2--;
					break;
				}
				erase_cur = false;

                cnt += set_len[it2->second];
                if (set_len[it2->second] >= 2) result -= set_len[it2->second] * (set_len[it2->second] - 1) / 2;
                set_len[it2->second] = 0;
                
                d_s[it2->second] = it3->second;
            }
            if (it2 != it3) {
            	m.erase(next(it3), next(it2));
			}
			
			if (erase_cur) m.erase(center);
        	else{
        		set_len[it3->second] += cnt;
            	if (set_len[it3->second] >= 2) result += set_len[it3->second] * (set_len[it3->second] - 1) / 2;	
			}

        }
        cout << result << "\n";
    }

    return 0;
}
