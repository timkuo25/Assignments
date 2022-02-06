/*
    References: Discussed with a TA
*/

#ifndef __YP_CLIQUE_GLPK_HEADER
#define __YP_CLIQUE_GLPK_HEADER
#include<glpk.h>
#include<vector>
#include<utility>
#include <iostream>
#include <algorithm>
#include <bitset> 
using namespace std;

namespace ypglpk{
  using std::vector,std::pair;
  bool output=0;
  constexpr double INF=1e300;
  // set whether to output verbose information about the solver (for debugging)
  void set_output(bool _output){
    output=_output;
  }
  // for interal usage. please do not call it unless you are really sure about what you are doing
  void set_constraints(glp_prob *P,const vector<vector<double>> &A,const vector<double> &b,const vector<double> &c){
    int n=c.size(),m=b.size();
    glp_set_obj_dir(P,GLP_MAX);
    glp_add_rows(P,m);
    glp_add_cols(P,n);
    for(int i=0;i<m;i++) glp_set_row_bnds(P,i+1,GLP_UP,0,b[i]);
    for(int i=0;i<n;i++) glp_set_col_bnds(P,i+1,GLP_FR,0,0);
    for(int i=0;i<n;i++) glp_set_obj_coef(P,i+1,c[i]);
    vector<int> ia(1),ja(1); vector<double> ar(1);
    for(int i=0;i<m;i++){
      for(int j=0;j<n;j++){
        if(A[i][j]==0) continue;
        ia.push_back(i+1); ja.push_back(j+1);
        ar.push_back(A[i][j]);
      }
    }
    glp_load_matrix(P,(int)ia.size()-1,ia.data(),ja.data(),ar.data());
  }
  // maximize cx subject to Ax<=b. return pair(cy,y) where y is the optimal x. if the constraints are infeasible or unbounded, return pair(-INF,{}).
  pair<double,vector<double>> linear_programming(const vector<vector<double>> &A,const vector<double> &b,const vector<double> &c){
    int n=c.size();
    glp_term_out(output?GLP_ON:GLP_OFF);
    glp_prob *lp=glp_create_prob();
    set_constraints(lp,A,b,c);
    glp_smcp parm;
    glp_init_smcp(&parm);
    parm.presolve = GLP_ON;
    if(glp_simplex(lp,&parm)){
      glp_delete_prob(lp);
      return {-INF,{}};
    }
    vector<double> ans(n);
    for(int i=0;i<n;i++) ans[i]=glp_get_col_prim(lp,i+1);
    double opt=glp_get_obj_val(lp);
    glp_delete_prob(lp);
    return {opt,ans};
  }
  // maximize cx subject to Ax<=b and xi is an integer if isint[i]. return pair(cy,y) where y is the optimal x. if the constraints are infeasible or unbounded, return pair(-INF,{}).
  pair<double,vector<double>> mixed_integer_linear_programming(const vector<vector<double>> &A,const vector<double> &b,const vector<double> &c,const vector<bool> &isint){
    int n=c.size();
    glp_term_out(output?GLP_ON:GLP_OFF);
    glp_prob *milp=glp_create_prob();
    set_constraints(milp,A,b,c);
    for(int i=0;i<n;i++){
      if(isint[i]) glp_set_col_kind(milp,i+1,GLP_BV);
    }
    glp_iocp parm;
    glp_init_iocp(&parm);
    parm.presolve = GLP_ON;
    if(glp_intopt(milp,&parm)){
      glp_delete_prob(milp);
      return {-INF,{}};
    }
    vector<double> ans(n);
    for(int i=0;i<n;i++) ans[i]=glp_mip_col_val(milp,i+1);
    double opt=glp_mip_obj_val(milp);
    glp_delete_prob(milp);
    return {opt,ans};
  }
}
#endif

void BK(vector<vector<int>> &result, bitset<80> R, bitset<80> P, bitset<80> X, vector<vector<int>> &g){
   bitset<80> empty;
    if (P == empty && X == empty){
        vector<int> sub_result;
        for (int i = 0; i < 80; i++){
            if (R[i]) sub_result.push_back(i);
        }
        result.push_back(sub_result);
	    return;
    }

    bitset<80> P_sub_N = P;

    int pivot;

    for (int i = 0; i < g.size(); i++){
      if (P[i] || X[i]){
        pivot = i;
        break;
      }
    }

    for (auto x: g[pivot]){
      P_sub_N[x] = 0;
    }

    for (int i = 0; i < g.size(); i++){
        if (P_sub_N[i]){
            bitset<80> uni_R = R, intersect_P, intersect_X, set_v, g_v;

            for (auto x: g[i]){
                g_v[x] = 1;
            }

            uni_R[i] = 1;
            intersect_P = P & g_v;
            intersect_X = X & g_v; 

            BK(result, uni_R, intersect_P, intersect_X, g);
            P[i] = 0;
            X[i] = 1;
        }
    }
}

int main(){
        // ios_base::sync_with_stdio(false);
        // cin.tie(nullptr);
        int cases = getchar() - '0';
        
        int thisChar = getchar();
        while (thisChar != '\n'){
          cases = cases * 10 + thisChar - '0';
          thisChar = getchar(); 
        }



        for (int cs = 0; cs < cases; cs++){
                int n_nodes, n_edges;
                n_nodes = getchar() - '0';

                thisChar = getchar();
                while (thisChar != ' '){
                  n_nodes = n_nodes * 10 + thisChar - '0';
                  thisChar = getchar(); 
                }

                n_edges = getchar() - '0';
                
                thisChar = getchar();
                while (thisChar != '\n'){
                  n_edges = n_edges * 10 + thisChar - '0';
                  thisChar = getchar(); 
                }

                vector<vector<int>> g(n_nodes);
                int from, to;

                for (int i = 0; i < n_edges; i++){
                  from = getchar() - '0';

                  thisChar = getchar();
                  while (thisChar != ' '){
                    from = from * 10 + thisChar - '0';
                    thisChar = getchar(); 
                  }

                  to = getchar() - '0';
                  
                  thisChar = getchar();
                  while (thisChar != '\n'){
                    to = to * 10 + thisChar - '0';
                    thisChar = getchar(); 
                  }
                  g[from].push_back(to);
                  g[to].push_back(from);
                }

                vector<vector<int>> result;
                bitset<80> R0, V0, X0;

                for (int i = 0; i < n_nodes; i++){
                    V0[i] = 1;
                }

                BK(result, R0, V0, X0, g);

    int n = result.size() , m = n_nodes;

    vector<vector<double>> A(m, vector<double>(n, 0));
    vector<double> b(m, 0),c(n, 0);

    for (int i = 0; i < n; i++){
        c[i] = -1;
    }

    for (int i = 0; i < m; i++){
      b[i] = -1;
    }

    for (int i = 0; i < result.size(); i++){
      for (auto x: result[i]){
        A[x][i] = -1;
      }
    }

                vector<bool> isint(n, 1);

                pair<double, vector<double>> res = ypglpk::mixed_integer_linear_programming(A,b,c,isint);

		cout << -res.first << "\n";

		vector<bool> picked(n_nodes, 0);
		vector<int> cout_list;
		for (int i = 0; i < result.size(); i++){
			if (res.second[i]){
				for (auto x: result[i]){
					if (!picked[x]){
						cout_list.push_back(x);
						picked[x] = 1;		
					}
				}
				cout << cout_list.size() << " ";
				for (int i = 0; i < cout_list.size(); i++){
					cout << cout_list[i] << (i == cout_list.size() - 1 ? "\n" : " ");
				}
				cout_list = vector<int>(0);
			}
		}
		}



        return 0;
}

