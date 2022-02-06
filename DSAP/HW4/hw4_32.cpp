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
bool check[4] = {0, 0, 0, 0};

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
		int getCon() {return con;}
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
	if (var == 'z')
	{
		if (con == 0) return;
		else cout << con;
	}
	
	else
	{
		if (mul == 0) return;
		else if (mul == 1)
		{
			if (div == 1) cout << var;
			else cout << var << "/" << div;
		}
		else if (mul == -1)
		{
			if (div == 1) cout << "-" << var;
			else cout << "-" << var << "/" << div;	
		}
		else 
		{
			if (div == 1) cout << mul << var;
			else cout << mul << var << "/" << div;
		}
	}
}

void Variable::neg()
{
	if (var == 'z') con = -con;
	else mul = -mul;
}

void insert(Variable v)
{
	char key = v.getVar();
	int ch;
	if (key == 'a') ch = 0;
	if (key == 'b') ch = 1;
	if (key == 'c') ch = 2;
	if (key == 'z') ch = 3;
	
	if (!check[ch])
	{
		check[ch] = true;
		v_stack.push(v);
	}
	else
	{
		stack<Variable> fornow;
		while(v_stack.top().getVar() != key)
		{
			fornow.push(v_stack.top());
			v_stack.pop();
		}
		v_stack.top() = v_stack.top() + v;
		while(!(fornow.empty()))
		{
			v_stack.push(fornow.top());
			fornow.pop();
		}
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
	bool zero = true;
	int count = 0;
	
	while(!(v_stack.empty()))
	{
		Variable top = v_stack.top();
		if(top.getVar() != 'z' and top.getMul() != 0) zero = false;
		if(top.getVar() == 'z' and top.getCon() != 0) zero = false;
		if (top.getMul() != 0) count++;
		ps.push(top);
		v_stack.pop();
	}
	while(! (ps.empty()) )
	{
		v_stack.push(ps.top());
		ps.pop();
	}
	if(zero)
	{
		cout << 0;
		return;
	}
	if(count == 1)
	{
		while(!(v_stack.empty()))
		{
			Variable top = v_stack.top();
			
			ps.push(top);
			v_stack.pop();
		}
		ps.top().print();
		v_stack.push(ps.top());
		ps.pop();
		while(! (ps.empty()) )
		{
			ps.top().print();
			v_stack.push(ps.top());
			ps.pop();
		}
		return;
	}
	
	
	while(!(v_stack.empty()))
	{
		Variable top = v_stack.top();
		
		ps.push(top);
		v_stack.pop();
	}
	ps.top().print();
	v_stack.push(ps.top());
	ps.pop();
	while(! (ps.empty()) )
	{
		Variable top = ps.top();
		if ( (top.getVar() != 'z' and top.getMul() > 0) or (top.getVar() == 'z' and top.getCon() > 0) )cout << "+";
		ps.top().print();
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
				num_stack.push(-88888888);
				Variable v('a', 0);
				v_stack2.push(v);
			}
			else if(target == "b")
			{
				num_stack.push(-88888888);
				Variable v('b', 0);
				v_stack2.push(v);
			}
			else if(target == "c")
			{
				num_stack.push(-88888888);
				Variable v('c', 0);
				v_stack2.push(v);
			}

			while(1)
			{
				if(oper_stack.empty()) break;
				if(!cmp(oper_stack.top(), formula[i])) break;
				
				int num2 = num_stack.top();
				num_stack.pop();
				int num1 = num_stack.top();
				num_stack.pop();
				
				if (num1 >= -88888887 and num2 >= -88888887) //5 5
				{
					int comp;
					if(oper_stack.top() == '*')
					{		
						comp = num1 * num2;
						num_stack.push(comp);
						
						oper_stack.pop();
					}
					else if(oper_stack.top() == '/')
					{
						comp = num1 / num2;
						num_stack.push(comp);
						
						oper_stack.pop();
					}
					else if(oper_stack.top() == '+')
					{
						comp = num1 + num2;
						num_stack.push(comp);
						
						oper_stack.pop();
					}
					else if(oper_stack.top() == '-')
					{
						comp = num1 - num2;
						num_stack.push(comp);
						
						oper_stack.pop();
					}
				}
				else if (num1 >= -88888887 and num2 == -88888888) // 5 a
				{
					//+-*
					if(oper_stack.top() == '*')
					{
						num_stack.push(-88888888);
						oper_stack.pop();
						
						Variable comp = v_stack2.top() * num1;
						v_stack2.pop();
						v_stack2.push(comp);
					}
					else if(oper_stack.top() == '+')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable CON('z', num1);
						insert(CON);
						Variable comp(v_stack2.top());
						insert(comp);
						v_stack2.pop();
					}
					else if(oper_stack.top() == '-')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable CON('z', num1);
						insert(CON);
						v_stack2.top().neg();
						Variable comp(v_stack2.top());
						insert(comp);
						v_stack2.pop();
					}
				}
				else if (num1 == -99999999 and num2 >= -88888887) // 5-a 9
				{
					//+-
					if(oper_stack.top() == '+')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable CON('z', num2);
						insert(CON);
					}
					else if(oper_stack.top() == '-')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable CON('z', -num2);
						insert(CON);
					}
				}
				else if (num1 == -99999999 and num2 == -88888888) // 5-a a
				{
					//+-
					if(oper_stack.top() == '+')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable comp(v_stack2.top());
						insert(comp);
						v_stack2.pop();
					}
					else if(oper_stack.top() == '-')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable comp(v_stack2.top());
						comp.neg();
						insert(comp);
						v_stack2.pop();
					}
				}
				else if (num1 == -88888888 and num2 >= -88888887) // a 5
				{
					//+-*/
					if(oper_stack.top() == '/')
					{
						num_stack.push(-88888888);
						oper_stack.pop();
						
						Variable comp = v_stack2.top() / num2;
						v_stack2.pop();
						v_stack2.push(comp);
					}
					else if(oper_stack.top() == '*')
					{
						num_stack.push(-88888888);
						oper_stack.pop();
						
						Variable comp = v_stack2.top() * num2;
						v_stack2.pop();
						v_stack2.push(comp);
					}
					else if(oper_stack.top() == '+')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable comp(v_stack2.top());
						insert(comp);
						v_stack2.pop();
						Variable CON('z', num2);
						insert(CON);
					}
					else if(oper_stack.top() == '-')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable comp(v_stack2.top());
						insert(comp);
						v_stack2.pop();
						Variable CON('z', -num2);
						insert(CON);
					}
				}
				else if (num1 == -88888888 and num2 == -88888888) // a a
				{
					//+-
					if(oper_stack.top() == '+')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable comp1(v_stack2.top());
						v_stack2.pop();
						Variable comp2(v_stack2.top());
						v_stack2.pop();
						
						insert(comp2);
						insert(comp1);
					}
					else if(oper_stack.top() == '-')
					{
						num_stack.push(-99999999);
						oper_stack.pop();
						
						Variable comp1(v_stack2.top());
						v_stack2.pop();
						Variable comp2(v_stack2.top());
						v_stack2.pop();
						comp1.neg();
						
						insert(comp2);
						insert(comp1);
					}
				}
			}
			oper_stack.push(parse);
			//print statement
			stack<int> tmp1;
			int count = 0;	
			while(!(num_stack.empty()))
			{
				int top = num_stack.top();
				tmp1.push(top);
				num_stack.pop();
				count++;
			}
			for(int i = 0; i < count - 1; i++)
			{	
				if(tmp1.top() == -99999999)
				{
					printVStack();
					cout << ",";
				}
				else if(tmp1.top() == -88888888)
				{
					v_stack2.top().print();
					cout << ",";
				}
				else cout << tmp1.top() << ",";
				num_stack.push(tmp1.top());
				tmp1.pop();
			}
			if(tmp1.top() == -99999999)
			{
				printVStack();
				cout << "\n";
			}
			else if(tmp1.top() == -88888888)
			{
				v_stack2.top().print();
				cout << "\n";
			}
			else cout << tmp1.top() << "\n";
			num_stack.push(tmp1.top());
			tmp1.pop();
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
		num_stack.push(-88888888);
		Variable v('a', 0);
		v_stack2.push(v);
	}
	else if(target == "b")
	{
		num_stack.push(-88888888);
		Variable v('b', 0);
		v_stack2.push(v);
	}
	else if(target == "c")
	{
		num_stack.push(-88888888);
		Variable v('c', 0);
		v_stack2.push(v);
	}
	while(1)
	{
		if(oper_stack.empty()) break;
		
		int num2 = num_stack.top();
		num_stack.pop();
		int num1 = num_stack.top();
		num_stack.pop();
		
		if (num1 >= -88888887 and num2 >= -88888887) //5 5
		{
			int comp;
			if(oper_stack.top() == '*')
			{		
				comp = num1 * num2;
				num_stack.push(comp);
				
				oper_stack.pop();
			}
			else if(oper_stack.top() == '/')
			{
				comp = num1 / num2;
				num_stack.push(comp);
				
				oper_stack.pop();
			}
			else if(oper_stack.top() == '+')
			{
				comp = num1 + num2;
				num_stack.push(comp);
				
				oper_stack.pop();
			}
			else if(oper_stack.top() == '-')
			{
				comp = num1 - num2;
				num_stack.push(comp);
				
				oper_stack.pop();
			}
		}
		else if (num1 >= -88888887 and num2 == -88888888) // 5 a
		{
			//+-*
			if(oper_stack.top() == '*')
			{
				num_stack.push(-88888888);
				oper_stack.pop();
				
				Variable comp = v_stack2.top() * num1;
				v_stack2.pop();
				v_stack2.push(comp);
			}
			else if(oper_stack.top() == '+')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable CON('z', num1);
				insert(CON);
				Variable comp(v_stack2.top());
				insert(comp);
				v_stack2.pop();
			}
			else if(oper_stack.top() == '-')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable CON('z', num1);
				insert(CON);
				v_stack2.top().neg();
				Variable comp(v_stack2.top());
				insert(comp);
				v_stack2.pop();
			}
		}
		else if (num1 == -99999999 and num2 >= -88888887) // 5-a 9
		{
			//+-
			if(oper_stack.top() == '+')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable CON('z', num2);
				insert(CON);
			}
			else if(oper_stack.top() == '-')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable CON('z', -num2);
				insert(CON);
			}
		}
		else if (num1 == -99999999 and num2 == -88888888) // 5-a a
		{
			//+-
			if(oper_stack.top() == '+')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable comp(v_stack2.top());
				insert(comp);
				v_stack2.pop();
			}
			else if(oper_stack.top() == '-')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable comp(v_stack2.top());
				comp.neg();
				insert(comp);
				v_stack2.pop();
			}
		}
		else if (num1 == -88888888 and num2 >= -88888887) // a 5
		{
			//+-*/
			if(oper_stack.top() == '/')
			{
				num_stack.push(-88888888);
				oper_stack.pop();
				
				Variable comp = v_stack2.top() / num2;
				v_stack2.pop();
				v_stack2.push(comp);
			}
			else if(oper_stack.top() == '*')
			{
				num_stack.push(-88888888);
				oper_stack.pop();
				
				Variable comp = v_stack2.top() * num2;
				v_stack2.pop();
				v_stack2.push(comp);
			}
			else if(oper_stack.top() == '+')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable comp(v_stack2.top());
				insert(comp);
				v_stack2.pop();
				Variable CON('z', num2);
				insert(CON);
			}
			else if(oper_stack.top() == '-')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable comp(v_stack2.top());
				insert(comp);
				v_stack2.pop();
				Variable CON('z', -num2);
				insert(CON);
			}
		}
		else if (num1 == -88888888 and num2 == -88888888) // a a
		{
			//+-
			if(oper_stack.top() == '+')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable comp1(v_stack2.top());
				v_stack2.pop();
				Variable comp2(v_stack2.top());
				v_stack2.pop();
				
				insert(comp2);
				insert(comp1);
			}
			else if(oper_stack.top() == '-')
			{
				num_stack.push(-99999999);
				oper_stack.pop();
				
				Variable comp1(v_stack2.top());
				v_stack2.pop();
				Variable comp2(v_stack2.top());
				v_stack2.pop();
				comp1.neg();
				
				insert(comp2);
				insert(comp1);
			}
		}
	}
	if(num_stack.top() == -99999999)
	{
		printVStack();
	}
	else if(num_stack.top() == -88888888)
	{
		v_stack2.top().print();
	}
	else cout << num_stack.top();
	num_stack.push(num_stack.top());
	num_stack.pop();
	return 0;
}

