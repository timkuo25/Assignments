/*
DSAP 2019 Spring Hw3-1, Catalan Number
ver 1.0
Author: tim_ezemnng
Last Modified: 4/5/2019

Note:
*/

#include <iostream>
using namespace std;

long long cat(int num)
{
	if (num == 0 or num == 1) return 1;
	else
	{
		long long result = 0;
		for (int k = 0; k < num; k++)
			result += ( cat(k) * cat(num - 1 - k) );
		return result;
	}
}

int main()
{
	int num;
	cin >> num;
	cout << cat(num);
	
	return 0;
}
