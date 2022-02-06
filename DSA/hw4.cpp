/*DSA HW4- How's Problem (Hash table)*/ 

#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <unordered_map>

using namespace std;


int main()
{
	int task, command;
	const long long x = 29, M = 1000000007;
	vector<long long> hi;
	deque<long long> o1, o2, o3;
	vector<int> ans;
	char add;
	string str1, str2;
	unordered_map<long long, int> record;

	hi.reserve(100500);
	hi.push_back(1);
	for (int i = 0; i < 100000; i++)
	{
		hi.push_back((hi[i] * 29) % M);
	}
	
	cin >> str1;
	cin >> task;
	
	
	for (int i = 0; i < str1.length(); i++)
		o1.push_back(str1[i]);
	
	if (o1.size() <= 20)
	{
		for (int i = 0; i < o1.size(); i++)
			o2.push_back(o1[i]);
	}
	
	else
	{
		for (int i = 0; i < 20; i++)
			o2.push_back(o1[o1.size() - 20 + i]);
		o3 = o2;
		
	/*	for (int i = 0; i < o2.size(); i++)
		{
			cout << o2[i] << " ";
		}
		cout << endl;
		
		for (int i = 0; i < o3.size(); i++)
		{
			cout << o3[i] << " ";
		}
		cout << endl;
	*/}

	long long value = 0;
	
	if (o1.size() <= 20)
	{
		for(int i = 0; i < o1.size(); i++)
		{
			value = (o1[i] - 96); //a, b
		//	cout << value << endl;
			record[value]++;
			
			for (int j = i + 1; j < o1.size(); j++)
			{
				value = (value * x + (o1[j] - 96)) % M; // ab, abc
			//	cout << value << endl;
				record[value]++;
			}
		}
	}
	
	else
	{
		
		for(int i = 0; i < 20; i++)
		{
			value = (o3[i] - 96); //a, b
		//	cout << value << endl;
			record[value]++;
			
			for (int j = i + 1; j < 20; j++)
			{
				value = (value * x + (o3[j] - 96)) % M; // ab, abc
			//	cout << value << endl;
				record[value]++;
			}
		}
		int head = o1.size() - 21;

		
		while(head > -1)
		{
			o3.push_front(o1[head]);
			o3.pop_back();
			value = (o3[0] - 96);
			record[value]++;
			
			for(int i = 1; i < 20; i++)
			{
				value = (value * x + o3[i] - 96) % M;
			//	cout << value << endl;
				record[value]++;				
			}
			head -= 1;
		}
					
	}

	for (int i = 0; i < task; i++)
	{
		cin >> command;

		if(command == 1)
		{
			cin >> add;
			
			if (o1.size() < 20)
			{
				o1.push_front(add);
				o2.push_front(add);

				value = o1[0] - 96;
				record[value]++;// a
				for(int i = 1; i < o1.size(); i++)
				{
					value = (value * x + o1[i] - 96) % M;
				//	cout << value;
					record[value]++; // ab, abc, ....,abcdefg 
				}
			//	cout << full << endl;
			}
			
			else
			{
				o1.push_front(add);

				value = o1[0] - 96;
				record[value]++;
				for(int i = 1; i < 20; i++)
				{
					value = (value * x + o1[i] - 96) % M;
				//	cout << value;
					record[value]++; // ab, abc, ....,abcdefg 
				}
			}
		}

		else if(command == 2)
		{
			cin >> add;
			
			if (o1.size() < 20)
			{
				o1.push_back(add);
				o2.push_back(add);
				
				int start = 0;
				for (int i = o2.size(); i > 0; i--)
				{
					value = 0;					
					for (int j = 0; j < i; j++)
					{
						value = (value * x + o2[start + j] - 96) % M;
					}
					record[value]++;
					start += 1;
				}
				
			}
			
			else
			{
				o1.push_back(add);
				o2.pop_front();
				o2.push_back(add);
				
				int start = 0;
				for (int i = 20; i > 0; i--)
				{
					value = 0;
					
					for (int j = 0; j < i; j++)
					{
						value = (value * x + o2[start + j] - 96) % M;
					}
					record[value]++;
					start += 1;
				}		
			}
		}

		else
		{
			cin >> str2;
			
			long long hvO = 0;
			
			for (int i = 0; i < str2.length(); i++)
			{
				hvO = (hvO * x + str2[i] - 96) % M;
			}
			
		//	cout << hvO << endl;
			
			if (str2.length() > o1.size())
				ans.push_back(0);
				
			else if (str2.length() > 10)
			{
				int count = 0;
				long long hvC, value = 0;
				vector<long long> hC;
				hC.push_back(0);
				for (int i = 0; i < o1.size(); i++)
				{
					value = (value * x + o1[i] - 96) % M;
					hC.push_back(value);
				}

		//hash value(l, r) = (hash[r] - hash[l - 1] * x^(r - l + 1)mod M + M)mod M
			
				for (int i = 0; i < (hC.size() - str2.length()); i++)
				{
					hvC = (hC[i + str2.length()] - hC[i] * hi[str2.length()] % M + M) % M;
					if (hvC == hvO)
						count += 1;
				}
				ans.push_back(count);
			}

			else
				ans.push_back(record[hvO]);

		}	
	}

	for (int i = 0; i < ans.size(); i++)
	{
		cout << ans[i] << endl;
	}

	return 0;
}
