/*DSAP hw4_3 - polynomial computation
v 2.0
note: 
didnt handle const
operator + not written
*/

#include <iostream>
#include <sstream>
#include <stack>
#include <string>

using namespace std;



class Variable;
bool isOper(char c);
bool cmp(char d, char u);
int gcd(int a, int b);

stack<int> num_stack;
stack<char> oper_stack;
stack<Variable> v_stack;
stack<Variable> v_stack2;
stack<char> oper_stack2;

class Variable
{
	private:
		char var;
		int mul;
		int div;
		int con;
		
	public:
		Variable(char v, int c): var(v), mul(1), div(1), con(c) {};
		Variable operator* (int n);
		Variable operator/ (int n);
		Variable operator+ (const Variable& v);
		Variable operator- (const Variable& v);
		char getVar() {return var;}
		int getMul() {return mul;}
		void neg();
		void print();
};

Variable Variable::operator* (int n)
{
	Variable result(*this);
	
	if(result.var == 'z') result.con *= n;
	else
	{
		result.mul *= n;
		int d = gcd(abs(result.mul), result.div);
		result.mul /= d;
		result.div /= d;
	}
	return result;
}

Variable Variable::operator/ (int n)
{
	Variable result(*this);
	
	if(result.var == 'z') result.con /= n;
	else
	{
		result.div *= n;
		int d = gcd(abs(result.mul), result.div);
		result.mul /= d;
		result.div /= d;
	}
	
	return result;
}

Variable Variable::operator+ (const Variable& v)
{
	Variable result(*this);
	
	if(result.var == 'z') result.con += v.con;
	else result.mul += (v.mul / v.div) * result.div;
	
	return result;
}

Variable Variable::operator- (const Variable& v)
{
	Variable result(*this);
	
	if(result.var == 'z') result.con -= v.con;
	else result.mul -= (v.mul / v.div) * result.div;
	
	return result;
}

void Variable::print()
{
	if (var == 'z') cout << con;
	else
	{
		if (mul == 1 and div == 1) cout << var;
		else
		{
			cout << mul << var;
			if(div != 1) cout << "/" << div;
		}
	}
}

void Variable::neg()
{
	if (var == 'z') con = -con;
	else mul = -mul;
}

void printN(stack<int> st)
{
	stack<int> tmp;	
	while(!(st.empty()))
	{
		int top = st.top();
		tmp.push(top);
		st.pop();
	}
	while(! (tmp.empty()) )
	{	
		cout << tmp.top() << "\n";
		st.push(tmp.top());
		tmp.pop();
	}
}

void printNStack()
{
	stack<int> ps;	
	while(!(num_stack.empty()))
	{
		int top = num_stack.top();
		ps.push(top);
		num_stack.pop();
	}
	while(! (ps.empty()) )
	{	
		cout << ps.top() << "\n";
		num_stack.push(ps.top());
		ps.pop();
	}
}

void printVStack()
{
	stack<Variable> ps;	
	while(!(v_stack.empty()))
	{
		Variable top = v_stack.top();
		ps.push(top);
		v_stack.pop();
	}
	while(! (ps.empty()) )
	{	
		ps.top().print();
		cout << "\n";
		v_stack.push(ps.top());
		ps.pop();
	}	
}

void printVStack2()
{
	stack<Variable> ps;	
	while(!(v_stack2.empty()))
	{
		Variable top = v_stack2.top();
		ps.push(top);
		v_stack2.pop();
	}
	while(! (ps.empty()) )
	{	
		ps.top().print();
		cout << "\n";
		v_stack2.push(ps.top());
		ps.pop();
	}
}

int gcd(int a, int b) 
{ 
	if (!b) return (a); 
	return gcd(b, a % b); 
}

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
	
	bool check[4] = {0, 0, 0, 0};
	
	for(int i = 0; i < formula.size(); i++)
	{
		//formula[i] is operator
		if(isOper(formula[i]))
		{
			char parse = formula[i];		
			getline(ss, target, parse);
			
			if(isdigit(target[0]))
			{
				int num = stoi(target);
				num_stack.push(num);
			}
			else if(target == "a")
			{
				num_stack.push(-2);
				Variable v('a', 0);
				v_stack2.push(v);
			}
			else if(target == "b")
			{
				num_stack.push(-2);
				Variable v('b', 0);
				v_stack2.push(v);
			}
			else if(target == "c")
			{
				num_stack.push(-2);
				Variable v('c', 0);
				v_stack2.push(v);
			}
			oper_stack.push(parse);
			//print statement
		}
		else continue;
	}
	getline(ss, target);
	
	if(isdigit(target[0]))
	{
		int num = stoi(target);
		num_stack.push(num);
	}
	else if(target == "a")
	{
		num_stack.push(-2);
		Variable v('a', 0);
		v_stack2.push(v);
	}
	else if(target == "b")
	{
		num_stack.push(-2);
		Variable v('b', 0);
		v_stack2.push(v);
	}
	else if(target == "c")
	{
		num_stack.push(-2);
		Variable v('c', 0);
		v_stack2.push(v);
	}
	
	//print statement
	printN(num_stack);
	
	return 0;
}

