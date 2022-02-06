/*
    References: Discussed with TAs
*/
#include "ada-hw4-p3.h"
#include <unordered_map>
#include <algorithm>
using namespace std;

// feel free to use global variables
vector<int> contest;
vector<int> valid;
vector<int> conceal;


unordered_map<int, bool> valid_map;
unordered_map<int, int> together;

int cur;


vector<int> init(int N) {
    for (int i = 0; i < N; i++)
        contest.push_back(i);

    cur = 0;
    valid.push_back(0);
    valid_map[0] = true;

    for (int i = 0; i < N - 1; i++){
        if (!compare(cur, i + 1)){
            conceal.push_back(cur);
            conceal.push_back(i + 1);
            valid.pop_back();

            valid_map[cur] = false;
            valid_map[i + 1] = false;

            together[cur] = i + 1;
            together[i + 1] = cur;

            if (valid.empty()) {
                i++;
                cur = i + 1;
                valid.push_back(i + 1);
                valid_map[i + 1] = true;
            }
            else cur = valid.back();
        }
        else{
            cur = i + 1;
            valid.push_back(i + 1);
            valid_map[i + 1] = true;
        }
    }

    return conceal;
}

vector<int> insert(int p, int id) {
    contest.insert(contest.begin() + p, id);
    valid_map[id] = true;

    int front = -1, back = -1;
    for (int i = p + 1; i < contest.size(); i++){
        if (valid_map[contest[i]]){
            front = contest[i];
            break;
        }
    }
    if (front != -1){
        if (!compare(id, front)){
            conceal.push_back(id);
            conceal.push_back(front);
            valid_map[id] = false;
            valid_map[front] = false;
            together[id] = front;
            together[front] = id;

            return conceal;
        }    
    }

    for (int i = p - 1; i >= 0; i--){
        if (valid_map[contest[i]]){
            back = contest[i];
            break;
        }
    }
    if (back != -1){
        if (!compare(back, id)){
            conceal.push_back(id);
            conceal.push_back(back);
            valid_map[id] = false;
            valid_map[back] = false;
            together[id] = back;
            together[back] = id;

            return conceal;
        }    
    }

    return conceal;
}

vector<int> remove(int p) {
    int id = contest[p];
    contest.erase(contest.begin() + p);

    if (valid_map[id]) return conceal;

    auto it = find(conceal.begin(), conceal.end(), id);
    conceal.erase(it);


    int id2 = together[id];
    int p2;
    for (int i = 0; i < contest.size(); i++){
        if (contest[i] == id2) p2 = i;
    }

    int front = -1, back = -1;
    for (int i = p2 + 1; i < contest.size(); i++){
        if (valid_map[contest[i]]){
            front = contest[i];
            break;
        }
    }
    if (front != -1){
        if (!compare(id2, front)){
            conceal.push_back(front);
            valid_map[front] = false;
            together[id2] = front;
            together[front] = id2;

            return conceal;
        }
    }

    for (int i = p2 - 1; i >= 0; i--){
        if (valid_map[contest[i]]){
            back = contest[i];
            break;
        }
    }
    if (back != -1){
        if (!compare(back, id2)){
            conceal.push_back(back);
            valid_map[back] = false;
            together[id2] = back;
            together[back] = id2;

            return conceal;
        }
    }

    it = find(conceal.begin(), conceal.end(), id2);
    conceal.erase(it);
    valid_map[id2] = true;

    return conceal;
}
