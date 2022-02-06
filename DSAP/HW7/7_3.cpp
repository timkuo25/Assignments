/*
DSAP 2019 Spring HW7-3, cycle detection and minimum tree height
ver 1.0
Author: tim_ezemnng
Last Modified: 6/1/2019

Note:
*/
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool cycle = false;
int* from;
int* ccount;
vector<int>* adj;
bool* explored;

void DFS(vector<int> v, int num)
{
	if (cycle) return;
//	cout << "now explore " << num << "\n";
	explored[num] = true;
	
	for(int i = 0; i < v.size(); i++)
	{
		if (explored[v[i]])
		{
//			cout << v[i] << " explored\n";
			if(from[num] == v[i]) continue;
			else
			{
//				cout << "cycle discovered\n";
				cycle = true;
				return;
			}
		}
		else
		{
//			cout << "next explore " << v[i] << "\n";
			from[v[i]] = num;
			DFS(adj[v[i]], v[i]);
		}
	}
}


void pruning(int size)
{
	vector<int> list;
	for(int i = 0; i < size; i++)
	{
		if (ccount[i] == 1) list.push_back(i);
	}
	for(int i = 0; i < list.size(); i++)
	{
		//	cout << "pruning " << list[i] << ", whose parent is " << adj[list[i]][0] << "\n";
			int parent = adj[list[i]][0];
			vector<int> :: iterator it = find(adj[parent].begin(), adj[parent].end(), list[i]);
			if(it != adj[parent].end()) adj[parent].erase(it);
			
			adj[list[i]].clear();
			ccount[list[i]] = 0;
			ccount[parent]--;
	}
}

int main()
{
	int size;
	cin >> size;
	
	adj = new vector<int> [size];
	
	explored = new bool[size];
	from = new int[size];
	ccount = new int[size];
	
	for(int i = 0; i < size; i++)
	{
		ccount[i] = 0;
		bool connected;
		for(int j = 0; j < size; j++)
		{	
			cin >> connected;
			if(connected)
			{
				adj[i].push_back(j);
				ccount[i]++;
			}
		}	
	}
	
/*	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < adj[i].size(); j++)
		{	
			cout << adj[i][j] << " ";
		}
		cout << "\n";	
	}*/
	
	//for DFS
	for(int i = 0; i < size; i++)
	{
		explored[i] = false;
		from[i] = -1;
	}

	DFS(adj[0], 0);
	
	if(cycle)
	{
		cout << 0;
		return 0;
	}
	

	int result = 0;
	while(1)
	{
		bool doo = false;
		for(int i = 0; i < size; i++)
		{
			if (ccount[i] > 0)
			{
				doo = true;
				break;
			}
		}
		if (doo)
		{
			result++;
			pruning(size);
		} 
		else break;
	}

/*	cout << "pruning\n";
	pruning(size);
	pruning(size);*/
/*	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < adj[i].size(); j++)
		{	
			cout << adj[i][j] << " ";
		}
		cout << "size = " << ccount[i];
		cout << "\n";	
	}*/
	
	cout << "1 " << result + 1;
	return 0;
}
