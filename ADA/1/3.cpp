/*
References:
Discussed with TAs
*/

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

long long combine(vector<pair<int, int>> &points, int begin, int mid, int end){
    int n1 = mid - begin + 1;
    int n2 = end - mid;
    long long result = 0;

    if (n1 == 1 and n2 == 1) return 1;

    vector<pair<int, int>> i_y_l;
    vector<pair<int, int>> i_y_r;

    for (int i = begin; i < mid + 1; i++){
        pair<int, int> p(i, points[i].second);
        i_y_l.push_back(p);
    }

    for (int i = mid + 1; i < end + 1; i++){
        pair<int, int> p(i, points[i].second);
        i_y_r.push_back(p);
    }


    sort(i_y_l.begin(), i_y_l.end(),
        [](auto &left, auto &right) {
             return left.second < right.second;
        }
    );

    sort(i_y_r.begin(), i_y_r.end(),
        [](auto &left, auto &right) {
             return left.second < right.second;
        }
    );

    //bottom to top
    vector<pair<int, int>> cut;
    vector<pair<int, int>> candidate;
    vector<int> candidate_y;
    int right_cnt = 0;

    while (i_y_r[right_cnt].second < i_y_l[0].second and right_cnt < i_y_r.size()){
        if (candidate.size() == 0){
            candidate.push_back(points[i_y_r[right_cnt].first]);
            candidate_y.push_back(i_y_r[right_cnt].second);
        }
        else if(candidate.back().first > points[i_y_r[right_cnt].first].first){
            candidate.pop_back();
            candidate_y.pop_back();
            continue;
        }
        else{
            candidate.push_back(points[i_y_r[right_cnt].first]);
            candidate_y.push_back(i_y_r[right_cnt].second);
        }
        right_cnt++;
    }
    result += candidate.size();

    for (int i = 1; i < i_y_l.size(); i++){
        //handle left
        if (points[i_y_l[i - 1].first].first > points[i_y_l[i].first].first)
            cut.push_back(points[i_y_l[i - 1].first]);

        if (cut.size() > 0){
            while (cut.back().first < points[i_y_l[i].first].first and cut.size() != 0)
                cut.pop_back();
        }
    
        //handle right
        while (i_y_r[right_cnt].second < i_y_l[i].second and right_cnt < i_y_r.size()){
            if (candidate.size() == 0){
                candidate.push_back(points[i_y_r[right_cnt].first]);
                candidate_y.push_back(i_y_r[right_cnt].second);
            }
            else if(candidate.back().first > points[i_y_r[right_cnt].first].first){
                candidate.pop_back();
                candidate_y.pop_back();
                continue;
            }
            else{
                candidate.push_back(points[i_y_r[right_cnt].first]);
                candidate_y.push_back(i_y_r[right_cnt].second);
            }
            right_cnt++;
        }

        //combine
        if (cut.size() == 0) result += candidate.size();
        //binary search
        else{
            int target = cut.back().second;
            auto lower = lower_bound(candidate_y.begin(), candidate_y.end(), target);
            result += candidate.size() - distance(candidate_y.begin(), lower);
        }
    }
    
    //top to bottom
    cut = vector<pair<int, int>>();
    candidate = vector<pair<int, int>>();
    candidate_y = vector<int>();
    right_cnt = i_y_r.size() - 1;

    while (i_y_r[right_cnt].second > i_y_l[i_y_l.size() - 1].second and right_cnt >= 0){
        if (candidate.size() == 0){
            candidate.push_back(points[i_y_r[right_cnt].first]);
            candidate_y.push_back(-i_y_r[right_cnt].second);
        }
        else if(candidate.back().first > points[i_y_r[right_cnt].first].first){
            candidate.pop_back();
            candidate_y.pop_back();
            continue;
        }
        else{
            candidate.push_back(points[i_y_r[right_cnt].first]);
            candidate_y.push_back(-i_y_r[right_cnt].second);
        }
        right_cnt--;
    }
    result += candidate.size();

    for (int i = i_y_l.size() - 2; i >= 0; i--){
        //handle left
        if (points[i_y_l[i + 1].first].first > points[i_y_l[i].first].first)
            cut.push_back(points[i_y_l[i + 1].first]);
        if (cut.size() > 0){
            while (cut.back().first < points[i_y_l[i].first].first and cut.size() != 0)
                cut.pop_back();
        }

        //handle right
        while (i_y_r[right_cnt].second > i_y_l[i].second and right_cnt >= 0){
            if (candidate.size() == 0){
                candidate.push_back(points[i_y_r[right_cnt].first]);
                candidate_y.push_back(-i_y_r[right_cnt].second);
            }
            else if(candidate.back().first > points[i_y_r[right_cnt].first].first){
                candidate.pop_back();
                candidate_y.pop_back();
                continue;
            }
            else{
                candidate.push_back(points[i_y_r[right_cnt].first]);
                candidate_y.push_back(-i_y_r[right_cnt].second);
            }
            right_cnt--;
        }

        //combine
        if (cut.size() == 0) result += candidate.size();
        //binary search
        else{
            int target = -cut.back().second;
            auto lower = lower_bound(candidate_y.begin(), candidate_y.end(), target);
            result += candidate.size() - distance(candidate_y.begin(), lower);
        }
    }
    

    return result;
}

long long dc(vector<pair<int, int>> &points, int begin, int end){
    if (begin == end) return 0;

    int mid = begin + (end - begin) / 2;
    return dc(points, begin, mid) + dc(points, mid + 1, end) + combine(points, begin, mid, end);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, tmp1, tmp2;
    vector<pair<int, int>> points;
    cin >> n;

    for (int i = 0; i < n; i++){
        cin >> tmp1 >> tmp2;
        pair<int, int> p(tmp1, tmp2);
        points.push_back(p);
    }

    sort(points.begin(), points.end(),
        [](auto &left, auto &right) {
             return left.first < right.first;
        }
    );

    long long result = dc(points, 0, points.size() - 1);
    cout << result;

    return 0;
}