/*
DSAP 2019 Spring HW6-2, queue waiting simulation 
ver 1.0
Author: tim_ezemnng
Last Modified: 5/1/2019

Note:
*/
#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	int n;
	double mu, lambda;
	
	cin >> n >> lambda >> mu;
	
	double now, total_wait = 0, oper;
	cin >> now >> oper;
	now = now + oper;
	
	for(int i = 0; i < n - 1; i++)
	{
		double arrive;
		cin >> arrive >> oper;
		
		if(arrive >= now) // no waiting
		{
			now = arrive + oper;
			continue;
		}
		else
		{
			total_wait += (now - arrive);
			now += oper;
			continue;
		}
	}
	
	double wp = total_wait / n;
	double wt = lambda / (mu * (mu - lambda));
	double per = (wp - wt) / wt;
	
	per *= 100;
	int ans = round(per);
	cout << ans;
	return 0;
}
