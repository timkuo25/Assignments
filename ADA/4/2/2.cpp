/*
    References: None
*/

#include "connect4.h"
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<char>> cur;

void place_disc(int col, bool player, vector<vector<char>> &v){
    int row = 0;
    while(1){
        if (v[row][col] == '-'){
            row++;
            if (row == 6) break;
        }
        else break;
    }
    
    if (player) v[row - 1][col] = 'X';
    else v[row - 1][col] = 'O';
}

void dec_vec(vector<int> &target, vector<vector<char>> &v){
    for (int i = 0; i < 7; i++){
        if (v[0][i] == '-'){
            target.push_back(i);
        }
    }
}

int check(vector<vector<char>> &v, bool player){
    int score = 0;
    char obj = player ? 'X' : 'O';

    //row
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 3; j++){
            int obj_cnt = 0, dash_cnt = 0;
            for (int k = 0; k < 4; k++){
                char cur = v[i][j + k];
                if (cur == obj) obj_cnt++;
                else if (cur == '-') dash_cnt++;
            }
            if (obj_cnt == 2 && dash_cnt == 2){
                score += 1.1;
            } 
            else if (obj_cnt == 3 && dash_cnt == 1) score += 5.5;
            else if (obj_cnt == 4) return 100000;
        }
    }

    //col
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 7; j++){
            int obj_cnt = 0, dash_cnt = 0;
            for (int k = 0; k < 4; k++){
                char cur = v[i + k][j];
                if (cur == obj) obj_cnt++;
                else if (cur == '-') dash_cnt++;
            }
            if (obj_cnt == 2 && dash_cnt == 2) score += 1;
            else if (obj_cnt == 3 && dash_cnt == 1) score += 5;
            else if (obj_cnt == 4) return 100000;
        }
    }

    for (int i = 3; i < 6; i++){
        for (int j = 0; j <= 3; j++){
            int obj_cnt = 0, dash_cnt = 0;
            for (int k = 0; k < 4; k++){
                char cur = v[i - k][j + k];
                if (cur == obj) obj_cnt++;
                else if (cur == '-') dash_cnt++;
            }
            if (obj_cnt == 2 && dash_cnt == 2) score += 1.2;
            else if (obj_cnt == 3 && dash_cnt == 1) score += 6;
            else if (obj_cnt == 4) return 100000;
        }
    }

    for (int i = 3; i < 6; i++){
        for (int j = 3; j < 7; j++){
            int obj_cnt = 0, dash_cnt = 0;
            for (int k = 0; k < 4; k++){
                char cur = v[i - k][j - k];
                if (cur == obj) obj_cnt++;
                else if (cur == '-') dash_cnt++;
            }
            if (obj_cnt == 2 && dash_cnt == 2) score += 1.2;
            else if (obj_cnt == 3 && dash_cnt == 1) score += 6;
            else if (obj_cnt == 4) return 100000;
        }
    }

    return score;
}


// The columns are numbered from 0 to 6
int decide(int yp_move) {
    if (yp_move == -1) {
        // A new game starts
        cur = {
                {'-', '-', '-', '-', '-', '-', '-'},
                {'-', '-', '-', '-', '-', '-', '-'},
                {'-', '-', '-', '-', '-', '-', '-'},
                {'-', '-', '-', '-', '-', '-', '-'},
                {'-', '-', '-', '-', '-', '-', '-'},
                {'-', '-', '-', 'X', '-', '-', '-'}
            };

        return 3;
    }
    else {
        // YP placed his piece at column `yp_move`
        place_disc(yp_move, 0, cur);

        vector<int> d_v;
        dec_vec(d_v, cur);

        vector<pair<int, int>> col_score;

        for (auto x : d_v){
            vector<vector<char>> tmp = cur;
            place_disc(x, 1, tmp);

            int score = check(tmp, 1);
            if (score == 100000) return x;
            vector<int> d_v2;
            dec_vec(d_v2, tmp);

            int best_O = 0;
            for (auto y : d_v2){
                vector<vector<char>> tmp2 = tmp;
                place_disc(y, 0, tmp2);

                int O_score = check(tmp2, 0);
                if (O_score == 100000){
                    best_O = 100000;
                    break;
                }
                best_O = max(O_score, best_O);
            }

            score -= best_O;

            pair<int, int> p(x, score);
            col_score.push_back(p);
        }

        sort(col_score.begin(), col_score.end(), [](auto &left, auto &right) {
            return left.second > right.second;
        });

        place_disc(col_score[0].first, 1, cur);
        return col_score[0].first;

    }
}
