#include <iostream>
#include <sstream>
#include <stack>
#include <string>
using namespace std;

bool isOper(char c);
bool cmp(char d, char u);

bool isOper(char c)
{
	if (c == '+' or c == '-' or c == '*' or c == '/') return true;
	return false;
}

bool cmp(char d, char u)
{
	bool down, up;
	
	down = (d == '+' or d == '-') ? 1 : 0;
	up = (u == '+' or u == '-') ? 1 : 0;
	
	return up >= down;
}

int main()
{
	string formula, target;
	cin >> formula;
	
	istringstream ss(formula);
	
	stack<int> num_stack;
	stack<char> oper_stack;
	
	for(int i = 0; i < formula.size(); i++)
	{
		//formula[i] is operator
		if(isOper(formula[i]))
		{
			char parse = formula[i];		
			getline(ss, target, parse);
			int num = stoi(target);
			num_stack.push(num);

			while(1)
			{
				if(oper_stack.empty()) break;
				
	//			cout << oper_stack.top() << " " << parse << "\n";
				if(!cmp(oper_stack.top(), formula[i])) break;
				
				int num2 = num_stack.top();
				num_stack.pop();
				int num1 = num_stack.top();
				num_stack.pop();
				int comp;
	//			cout << oper_stack.top();
				if(oper_stack.top() == '*')
				{		
					//multiplication
					comp = num1 * num2;
					num_stack.push(comp);
					
					oper_stack.pop();
				//	continue;
				}
				else if(oper_stack.top() == '/')
				{
					//devision
					comp = num1 / num2;
					num_stack.push(comp);
					
					oper_stack.pop();
				//	continue;
				}
				else if(oper_stack.top() == '+')
				{
					//addition
	//				cout << "hi";
					comp = num1 + num2;
					num_stack.push(comp);
					
					oper_stack.pop();
				//	continue;
				}
				else if(oper_stack.top() == '-')
				{
					//substraction
					comp = num1 - num2;
					num_stack.push(comp);
					
					oper_stack.pop();
				//	continue;
				}	
			}
			oper_stack.push(parse);
			
			int count = 0;
			stack<int> temp;
			while(!num_stack.empty())
			{
				temp.push(num_stack.top());
				num_stack.pop();
				count++;
			}
			count--;
			for(int i = 0; i < count; i++)
			{
				cout << temp.top() << ",";
				num_stack.push(temp.top());
				temp.pop();
			}
			cout << temp.top() << "\n";
			num_stack.push(temp.top());
			temp.pop();
			continue;
		}
		//formula[i] is num
		else continue;
	}
	
	getline(ss, target);
	int num = stoi(target);
	num_stack.push(num);		
	while(!oper_stack.empty())
	{
		int num2 = num_stack.top();
		num_stack.pop();
		int num1 = num_stack.top();
		num_stack.pop();
		
		if(oper_stack.top() == '*')
		{
			int num = num1 * num2;
			num_stack.push(num);
			oper_stack.pop();
		}
		else if(oper_stack.top() == '/')
		{
			int num = num1 / num2;
			num_stack.push(num);
			oper_stack.pop();
		}
		else if(oper_stack.top() == '-')
		{
			int num = num1 - num2;
			num_stack.push(num);
			oper_stack.pop();
		}
		else if(oper_stack.top() == '+')
		{
			int num = num1 + num2;
			num_stack.push(num);
			oper_stack.pop();
		}
	}
	cout << num_stack.top();
	return 0;
}

