/*
DSAP 2019 Spring HW8-4, variable declaration and hash table
ver 1.0
Author: tim_ezemnng
Last Modified: 6/17/2019

Note:
the variable type includes const, long int, and long long int, while excluding pointers and references
*/
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Node
{
	private:
		string name;
		string type;
		Node* next;
	
	public:
		Node(string n, string t) : name(n), type(t) , next(nullptr){;}
		string getName() const {return name;}
		string getType() const {return type;}
		Node* getNext() const {return next;}
		void setType(string s) {type = s;}
		void setNext(Node* n) {next = n;}
		
	friend class Hash;
		
};

class Hash
{
	private:
		Node** table;
		int hashing(string s)
		{
			int result = 0;
			for(int i = 0; i < s.size(); i++)
				result += s[i];
			result = result % 100;
			
			return result;
		}
		
	public:
		Hash() //constructor
		{
			table = new Node*[100];
			for(int i = 0; i < 100; i++)
				table[i] = nullptr;
		}
		
		void insert(Node* entry);
		Node* search(string key);
};

void Hash::insert(Node* entry)
{ 
	int index = hashing(entry->getName());
	
//	cout << "now insert " << entry->getName() << " index = " << index << "\n";
	
	if(table[index] == nullptr) table[index] = entry; //no entry
	else //collision
	{
		Node* result = search(entry->getName());
		if(result != nullptr)
		{
			result->setType(entry->getType());
			return;
		}
		Node* cur = table[index];
		while(cur->getNext() != nullptr)
			cur = cur->getNext();
		cur->setNext(entry);
	}
}

Node* Hash::search(string key) 
{
	
	bool found = false;
	int index = hashing(key);
//	cout << "now search " << key << " index = " << index << "\n";
	
	if(table[index] == nullptr) return nullptr;
	else
	{
		Node* cur = table[index];
		
		do
		{
			if (cur->getName() == key)
			{
				found = true;
				break;
			}
			cur = cur->getNext();
		} while(cur != nullptr);
		
		if (found) return cur;
		else return nullptr;
	}
}

int main()
{
	string code;
	size_t found;
	Hash h;

	while(getline(cin, code))
	{
		found = code.find(';');
		if (found != string::npos)
		{
			string type;
			string name;
			istringstream ss(code);
			
			getline(ss, type, ' ');
			if(type == "const")
			{
				getline(ss, type, ' ');
				if(type == "long")
				{
					getline(ss, type, ' ');
					if(type == "long") //const long long
					{
						getline(ss, type, ' ');
						type = "const long long " + type;
					}
					else
					{
						type = "const long " + type;
					}
				}
				else
					type = "const " + type;
			}
			else if(type == "long")
			{
				getline(ss, type, ' ');
				if(type == "long") //long long
				{
					getline(ss, type, ' ');
					type = "long long " + type;
				}
				else type = "long " + type;
			}
			getline(ss, name, ';');
			
			Node* n = new Node (name, type);
			h.insert(n);
		}
		else
		{
			string name = code;
			Node* result = h.search(code);
			if(result == nullptr) cout << "not defined\n";
			else cout << result->getType() << "\n";
		}
	}

	return 0;
}
