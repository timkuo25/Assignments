/*DSA HW5-2 Angry Birds Problem(Dynamic Programming)*/ 

#include <iostream>
#include <vector>
using namespace std;

bool in_p (double a, double b, double x, double y)
{
	double count = y - (a*x*x + b*x);
	if (count < 0.0001 and count > -0.0001)
		return true;
	return false;
}

int main()
{
	int task, num;
	vector<int> ans;
	cin >> task;
	
	for(int r = 0; r < task; r++)
	{
		cin >> num;
		double** loc = new double*[num];
		for(int i = 0; i < num; i++)
			loc[i] = new double[2];
			
		int** set = new int*[num];
		for(int i = 0; i < num; i++)
			set[i] = new int[num];
			
		for(int i = 0; i < num; i++)
			cin >> loc[i][0] >> loc[i][1];
		
		int posb = (1 << num); //2^n�زզX
		
		int* dp = new int[posb];
		
		double x1, x2, y1, y2;
		double a, b;
		
		for(int i = 0; i < num; i++)
		{
			for(int j = 0; j < num; j++)
			{
				x1 = loc[i][0];
				y1 = loc[i][1];
				x2 = loc[j][0];
				y2 = loc[j][1];
				
				if (x1 == x2)
				{
					set[i][j] = -1;
					continue;
				}
				
				a = (y1*x2 - y2*x1) / (x1*x2*(x1 - x2));
				b = (y1*x2*x2 - y2*x1*x1) / (x1*x2*(x2 - x1));					

				if (a >= 0)
				{
					set[i][j] = -1;
					continue;
				}
						
				int p = ((1 << i) + (1 << j));
				for(int k = 0; k < num; k++)
				{
					if(k == i or j == k)
						continue;
						
					if (in_p(a, b, loc[k][0], loc[k][1]))
						p += (1 << k);			
				}
				set[i][j] = p;
			}
		}
		
		for(int i = 0; i < posb; i++)
		{
			bool got1 = false, got2 = false, gotp = false;
			int count, best = 5000;
			for (int j = 0; j < num; j++)
			{
				if ((i & (1 << j)) == 0)
					continue;
				
				got1 = true;

				for(int k = j + 1; k < num; k++)
				{
					if((i & (1 << k)) == 0)
						continue;	
					
					got2 = true;
					
					if (set[j][k] == -1)
					{
						int p = (1 << j) + (1 << k);
						count = dp[i - p] + 2;
						if (count < best)
							best = count;
						continue;
					}
						
					gotp = true;
					
					count = dp[i & (~set[j][k])] + 1;
					if (count < best)
						best = count;			
				}				
				break;
			}
			
			if (got1 == false)
			{
			//	cout << i << " 1" << endl;
				dp[i] = 0;
			}
				
			else if ((got1 == true) and (got2 == false))
			{
			//	cout << i << " 2" << endl;
				dp[i] = 1;
			}
			
			else
			{			
			//	cout << i << " 4" << endl;
				dp[i] = best;
			}	

		}

	//	for (int i = 0; i < posb; i++)
	//		cout << dp[i] << " ";
			
		ans.push_back(dp[posb - 1]);
	
		delete [] dp;
			
		for(int i = 0; i < num; i++)
		{
			delete [] set[i];
			delete [] loc[i];
		}
		
		delete [] loc;
		delete [] set;
	}
	
	for (int i = 0; i < ans.size() - 1; i++)
		cout << ans[i] << endl;
	cout << ans[task - 1];
	return 0;
}
