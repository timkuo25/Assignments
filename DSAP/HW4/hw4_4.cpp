/*DSAP hw4_4 - Depth-First Search*/

#include <iostream>
#include <sstream>
#include <stack>
#include <string>
using namespace std;

bool* record;
bool** matrix;
bool ans;
stack<int> n_stack;
int count = 0;

void operation()
{
	if (n_stack.empty()) return;
	bool push = false;
	
	int top = n_stack.top();
	for(int i = 0; i < count; i++)
	{
		if (!matrix[top - 1][i]) continue;
		else
		{
			if (record[i]) continue;
			else
			{
				push = true;
				record[i] = true;
				n_stack.push(i + 1);
				if (i + 1 == count) ans = true;
				continue;	
			}
		}
	}
	if(push)
	{
		stack<int> tmp;
		string out;
		while(!n_stack.empty())
		{
			tmp.push(n_stack.top());
			n_stack.pop();
		}
		while(!tmp.empty())
		{
			stringstream SS;
			SS << tmp.top();
			string str = SS.str();
			out = out + str + ",";
			n_stack.push(tmp.top());
			tmp.pop();
		}
		out.erase(out.end() - 1);
		
		cout << out << "\n";
		operation();
	}
	else
	{
		n_stack.pop();
		operation();
	}
}

int main()
{
	string s;

	getline(cin, s);
	count = (s.size() + 1) / 2;
	
	record = new bool[count];
	matrix = new bool*[count];
	
	for(int i = 0; i < count; i++)
	{
		matrix[i] = new bool[count];
		record[i] = 0;
	}
	
	istringstream ss(s);
	string temp;
	for(int i = 0; i < count - 1; i++)
	{
		getline(ss, temp, ',');
		matrix[0][i] = (temp == "0") ? 0 : 1;
	}
	getline(ss, temp);
	matrix[0][count - 1] = (temp == "0") ? 0 : 1;
	
	for(int i = 1; i < count; i++)
	{
		getline(cin, s);
		istringstream ss(s);
		string temp;
		for(int j = 0; j < count - 1; j++)
		{
			getline(ss, temp, ',');
			matrix[i][j] = (temp == "0") ? 0 : 1;
		}
		getline(ss, temp);
		matrix[i][count - 1] = (temp == "0") ? 0 : 1;
	}

	if(count == 1 and matrix[0][0] == 0)
	{
		cout << "1\n0";
		return 0; 
	}
	if(count == 1)
	{
		cout << "1\n1";
		return 0;
	}
	n_stack.push(1);
	cout << "1\n";
	record[0] = true;
	operation();
	
	cout << ans;
	//print statement
	/*for(int i = 0; i < count; i++)
	{
		for(int j = 0; j < count - 1; j++)
			cout << matrix[i][j] << ",";
		cout << matrix[i][count - 1] << "\n";
	}*/
	return 0;
}
