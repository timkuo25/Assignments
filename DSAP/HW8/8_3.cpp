/*
DSAP 2019 Spring HW8-3, topological sort
ver 1.0
Author: tim_ezemnng
Last Modified: 6/19/2019

Note:
*/
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

class Node
{
	private:
		string name;
		string pre;
	public:
		Node(string n, string p = ""): name(n), pre(p) {;}
		string getName() {return name;}
		string getPre() {return pre;}
};

int main()
{
	int num;
	int count = 0;
	string input;
	size_t found;
	unordered_map<string, int> map;
	vector<Node*> list;
	vector<string> ans;
	
	getline(cin, input);
	num = stoi(input);
	
	bool** graph = new bool* [num];
	for(int i = 0; i < num; i++)
		graph[i] = new bool[num];
	
	for(int i = 0; i < num; i++)
	{
		for(int j = 0; j < num; j++)
			graph[i][j] = false;
	}
	
	while(getline(cin, input))
	{
		found = input.find(',');
		string c1;
		
		if (found != string::npos)
		{
			
			string c2;
			istringstream ss(input);
			getline(ss, c1, ',');
			getline(ss, c2);
			
			Node* course = new Node(c1, c2);
			list.push_back(course);
		}
		else
		{
			c1 = input;
			
			Node* course = new Node(c1);
			list.push_back(course);
		}
		
		pair<string, int> entry(c1, count);
		map.insert(entry);
		count++;
	}
	
	for(int i = 0; i < num; i++)
	{
		if(list[i]->getPre() != "") graph[i][ map[list[i]->getPre()] ] = true;
		else continue;
	}

	count = 0;
	bool* selected = new bool[num];
	for(int i = 0; i < num; i++)
		selected[i] = false;
		
	while(count != num)
	{
		for(int i = 0; i < num; i++)
		{
			bool notPointed = true;
			for(int j = 0; j < num; j++)
			{
				if(graph[i][j])
				{
					notPointed = false;
					break;
				}
			}
			
			if(notPointed and !selected[i]) //select i
			{
				selected[i] = true;
				ans.push_back(list[i]->getName());
				
				for(int j = 0; j < num; j++)
					graph[j][i] = false;
				break;
			}
		}
		count++;

	}
	for(int i = 0; i < ans.size(); i++)
		cout << ans[i] << "\n";
	
	return 0;
}
