/*
References: Discussed with TAs
*/

#include <iostream>
#include <vector>
using namespace std;

long long cnt = 0;
double a, b, c;

void merge_z(long long *p, long long *z, int begin, int mid, int end){
    int n1 = mid - begin + 1;
    int n2 = end - mid;

    long long *p_left = new long long[n1], *p_right = new long long[n2];
    long long *z_left = new long long[n1], *z_right = new long long[n2];

    for (int i = 0; i < n1; i++){
        p_left[i] = p[begin + i];
        z_left[i] = z[begin + i];
    }
        
    for (int i = 0; i < n2; i++){
        p_right[i] = p[mid + 1 + i];
        z_right[i] = z[mid + 1 + i];
    }
    
    int left_index = 0, right_index = 0, index_merged = begin;

    while (left_index < n1 && right_index < n2){
        if (z_left[left_index] < z_right[right_index]) {
            p[index_merged] = p_left[left_index];
            z[index_merged] = z_left[left_index];
            left_index++;
        }
        else {
            p[index_merged] = p_right[right_index];
            z[index_merged] = z_right[right_index];
            right_index++;
        }
        index_merged++;
    }
    
    while (left_index < n1) {
        z[index_merged] = z_left[left_index];
        p[index_merged] = p_left[left_index];
        left_index++;
        index_merged++;
    }

    while (right_index < n2) {
        z[index_merged] = z_right[right_index];
        p[index_merged] = p_right[right_index];
        right_index++;
        index_merged++;
    }

    delete [] p_left;
    delete [] p_right;
    delete [] z_left;
    delete [] z_right;
}

void merge_sort_z(long long *p, long long *z, int begin, int end){
    if (begin == end) return;

    int mid = begin + (end - begin) / 2;
    merge_sort_z(p, z, begin, mid);
    merge_sort_z(p, z, mid + 1, end);
    merge_z(p, z, begin, mid, end);
}

void merge_p(long long *p, long long *z, int begin, int mid, int end){
    int n1 = mid - begin + 1;
    int n2 = end - mid;

    long long *p_left = new long long[n1], *p_right = new long long[n2];
    long long *z_left = new long long[n1], *z_right = new long long[n2];

    for (int i = 0; i < n1; i++){
        p_left[i] = p[begin + i];
        z_left[i] = z[begin + i];
    }
        
    for (int i = 0; i < n2; i++){
        p_right[i] = p[mid + 1 + i];
        z_right[i] = z[mid + 1 + i];
    }

    int j_start = 0;
    int max = -99999;
    for (int i = 0; i < n1; i++){
        for (int j = j_start; j < n2; j++){
            if (b * p_left[i] + c < a * p_right[j]){
                cnt += n2 - j;
                break;
            }
            j_start++;
        }
        if (z_left[i] > max) max = z_left[i];
    }

    vector<int> v1, v2;
    for (int i = 0; i < n1; i++){
        if (z_left[i] == max) v1.push_back(i);
    }
    for (int i = 0; i < n2; i++){
        if (z_right[i] == max) v2.push_back(i);
    }

    j_start = 0;
    for (int i = 0; i < v1.size(); i++){
        for (int j = j_start; j < v2.size(); j++){
            if (b * p_left[v1[i]] + c < a * p_right[v2[j]]){
                cnt -= (v2.size() - j);
                break;
            }
            j_start++;
        }
    }
    
    int left_index = 0, right_index = 0, index_merged = begin;

    while (left_index < n1 && right_index < n2){
        if (p_left[left_index] <= p_right[right_index]) {
            p[index_merged] = p_left[left_index];
            z[index_merged] = z_left[left_index];
            left_index++;
        }
        else {
            p[index_merged] = p_right[right_index];
            z[index_merged] = z_right[right_index];
            right_index++;
        }
        index_merged++;
    }
    
    while (left_index < n1) {
        p[index_merged] = p_left[left_index];
        z[index_merged] = z_left[left_index];
        left_index++;
        index_merged++;
    }

    while (right_index < n2) {
        p[index_merged] = p_right[right_index];
        z[index_merged] = z_right[right_index];
        right_index++;
        index_merged++;
    }

    delete[] p_left;
    delete[] p_right;
    delete[] z_left;
    delete[] z_right;
}

void merge_sort_p(long long *p, long long *z, int begin, int end){
    if (begin == end) return;

    int mid = begin + (end - begin) / 2;
    merge_sort_p(p, z, begin, mid);
    merge_sort_p(p, z, mid + 1, end);
    merge_p(p, z, begin, mid, end);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n >> a >> b >> c;
    long long *p = new long long[n];
    long long *z = new long long[n];
    
    for (int i = 0; i < n; i++){
        cin >> p[i];
        cin >> z[i];
    }

    merge_sort_z(p, z, 0, n - 1);
    merge_sort_p(p, z, 0, n - 1);

    cout << cnt;

    return 0;
}