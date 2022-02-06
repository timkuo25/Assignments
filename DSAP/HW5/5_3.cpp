/*
DSAP 2019 Spring HW5-3, Linked List Implementation 
ver 1.0
Author: tim_ezemnng
Last Modified: 5/1/2019

Note:
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
 
using namespace std;

class Node;
class List;
int search(string s, const vector<List*>& lv);

class Node
{
	private:
		int value;
		Node* next;
	public:
		Node(int v) : value(v), next(nullptr){}
		
	friend class List;
};

class List
{
	private:
		string name;
		int size;
		Node* headPtr;
	public:
		List(string s): name(s), size(0), headPtr(nullptr) {}; 
		~List();
		void print() const;
		
		void append(int element);
		void insert(int pos, int element);
		void remove(int element);
		int index(int element) const;
		int count(int element) const;
		void extend(const List& anotherList);
		void reverse();
		void sort();
		
	friend int search(string s, const vector<List*>& lv);		
};

List::~List()
{
	if (size == 0) return;
	
	Node* cur;
	Node* del;
	
	cur = headPtr;
	while(cur->next != nullptr)
	{
		del = cur;
		cur = cur->next;
		delete del;
	}
	del = cur;
	delete del;
}

void List::print() const
{
	Node* current = headPtr;
	cout << "[";
	while(current != nullptr)
	{
		cout << current->value;
		current = current->next;
		if (current != nullptr) cout << ",";
	}
	cout << "]" << "\n";
}

void List::append(int element)
{
	if(size == 0)
	{
		headPtr = new Node(element);
		size++;
		return;
	}
	
	Node* current = headPtr;
	while(current->next != nullptr)
		current = current->next;

	current->next = new Node(element);
	size++;
}

void List::insert(int pos, int element)
{
	Node* cur = headPtr;
	if (pos <= 0 or size == 0)
	{
		headPtr = new Node(element);
		size++;
		headPtr->next = cur;
		return;
	}	

	if(pos >= size)
	{
		Node* current = headPtr;
		while(current->next != nullptr)
			current = current->next;
		current->next = new Node(element);
		size++;
		return;
	}

	int count = 1;
	
	while(count != pos)
	{
		cur = cur->next;
		count++;
	}
	Node* tmp = cur->next;
	cur->next = new Node(element);
	size++;
	cur->next->next = tmp;
}

void List::remove(int element)
{
	if (size == 0) return;
	if (headPtr->value == element)
	{
		Node* del = headPtr;
		headPtr = headPtr->next;
		delete del;
		size--;
		return;
	}

	Node* current = headPtr;
	
	while(current->next != nullptr)
	{
		if(current->next->value == element)
		{
			Node* del = current->next;
			current->next = current->next->next;
			delete del; 
			size--;
			return;
		}
		current = current->next;
	}
	if(current->value == element)
	{
		delete current; 
		size--;
		return;
	}
}

int List::index(int element) const
{
	if(size == 0)
	{
		cout << -1 << "\n";
		return -1;
	}
	int result = 0;
	
	Node* current = headPtr;
	while(current->next != nullptr)
	{
		if (current->value == element)
		{
			cout << result << "\n";
			return result;
		}
		result++;
		current = current->next;
	}
	if (current->value == element)
	{
		cout << result << "\n";
		return result;
	}
	cout << -1 << "\n";
	return -1;
}

int List::count(int element) const
{
	if(size == 0)
	{
		cout << 0 << "\n";
		return 0;
	}
	int result = 0;
	
	Node* current = headPtr;
	while(current->next != nullptr)
	{
		if(current->value == element) result++;
		current = current->next;
	}
	if(current->value == element) result++;
	cout << result << "\n";
	return result;
}

void List::extend(const List& anotherList) //boundary condition not handled
{
	if(anotherList.size == 0) return;
	
	if(anotherList.name == this->name)
	{
		Node* cpy = headPtr;
		int s = size;
		for(int i = 0; i < s; i++)
		{
			this->append(cpy->value);
			cpy = cpy->next;
		}
		return;
	}

	Node* cpy = anotherList.headPtr;
	for(int i = 0; i < anotherList.size; i++)
	{
		this->append(cpy->value);
		cpy = cpy->next;
	}
}

void List::reverse()
{
	if(size == 1 or size == 0) return;
	
	Node* rec = headPtr->next;
	Node* cur = headPtr;
	Node* rrec = nullptr;
	
	cur->next = nullptr;
	while(rec->next != nullptr)
	{
		rrec = rec->next;
		rec->next = cur;
		cur = rec;
		rec = rrec;
	}
	rec->next = cur;
	headPtr = rec;
}

int search(string s, const vector<List*>& lv)
{
	for(int i = 0; i < lv.size(); i++)
	{
		if(lv[i]->name == s) return i;
	}
	return -1;
}

void List::sort()
{
	if(size == 0 or size == 1) return;
	
	for(int i = size - 1; i > 0; i--) //if size == 2, swap once
	{
		Node* left = nullptr;
		Node* mid = headPtr;
		Node* right = headPtr->next;
		Node* tmp = nullptr;
		
		//first
		if(mid->value > right->value)
		{
			mid->next = right->next;
			right->next = mid;
			headPtr = right;
			
			tmp = right;
			right = mid;
			mid = tmp;
		}
		//others
		for(int j = 1; j < i; j++)
		{
			left = mid;
			mid = right;
			right = right->next;
			if(mid->value > right->value)
			{
				left->next = right;
				mid->next = right->next;
				right->next = mid;
				
				tmp = right;
				right = mid;
				mid = tmp;
			}
		}
	}
}

int main()
{
	vector<List*> vList;
	string input;
	
/*	List a("a");
	a.append(1);
	a.append(2);
	a.append(3);
	a.remove(3);
	a.print();*/

	while(getline(cin, input))
	{
		if(input.find(" = list()") != string::npos)
		{
			istringstream ss(input);
			string name;
			getline(ss, name, ' ');
			
			List* ptr = new List(name);
			vList.push_back(ptr);
			continue;
		}
		
		if(input.find(".append(") != string::npos)
		{
			istringstream ss(input);
			string name;
			string NUM;
			getline(ss, name, '.');
			getline(ss, NUM, '(');
			getline(ss, NUM, ')');
			int num = stoi(NUM);
			
			int index = search(name, vList);
			vList[index]->append(num);
			continue;
		}
		
		if(input.find(".insert(") != string::npos)
		{
			istringstream ss(input);
			string name;
			string POS;
			string NUM;
			getline(ss, name, '.');
			getline(ss, POS, '(');
			getline(ss, POS, ',');
			getline(ss, NUM, ')');
			int pos = stoi(POS);
			int num = stoi(NUM);
			
			int index = search(name, vList);
			vList[index]->insert(pos, num);
			continue;
		}
		
		if(input.find(".remove(") != string::npos)
		{
			istringstream ss(input);
			string name;
			string NUM;
			getline(ss, name, '.');
			getline(ss, NUM, '(');
			getline(ss, NUM, ')');
			int num = stoi(NUM);
			
			int index = search(name, vList);
			vList[index]->remove(num);
			continue;
		}
		
		if(input.find(".index(") != string::npos)
		{
			istringstream ss(input);
			string name;
			string NUM;
			getline(ss, name, '.');
			getline(ss, NUM, '(');
			getline(ss, NUM, ')');
			int num = stoi(NUM);
			
			int index = search(name, vList);
			vList[index]->index(num);
			continue;
		}
		
		if(input.find(".count(") != string::npos)
		{
			istringstream ss(input);
			string name;
			string NUM;
			getline(ss, name, '.');
			getline(ss, NUM, '(');
			getline(ss, NUM, ')');
			int num = stoi(NUM);
			
			int index = search(name, vList);
			vList[index]->count(num);
			continue;
		}
		
		if(input.find(".extend(") != string::npos)
		{
			istringstream ss(input);
			string name1;
			string name2;
			getline(ss, name1, '.');
			getline(ss, name2, '(');
			getline(ss, name2, ')');
			
			int index1 = search(name1, vList);
			int index2 = search(name2, vList);
			vList[index1]->extend(*vList[index2]);
			continue;
		}
		
		if(input.find(".reverse()") != string::npos)
		{
			istringstream ss(input);
			string name;
			getline(ss, name, '.');
			
			int index = search(name, vList);
			vList[index]->reverse();
			continue;
		}
		
		if(input.find(".sort()") != string::npos)
		{
			istringstream ss(input);
			string name;
			getline(ss, name, '.');
			
			int index = search(name, vList);
			vList[index]->sort();
			continue;
		}
		
		istringstream ss(input);
		string name;
		getline(ss, name);
		int index = search(name, vList);
		vList[index]->print();
	}
	
	return 0;
}
