/*
DSAP 2019 Spring HW7-4, prefix tree
ver 1.0
Author: tim_ezemnng
Last Modified: 6/2/2019

Note:
*/
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>

using namespace std;

string ans;
priority_queue<string, vector<string>, greater<string>> mypq;
bool sig = false;

class Node
{
	private:
		char value;
		Node* parent;
		vector<Node*> children;
		bool aWord;
	
	public:
		Node(char v) : value(v), parent(nullptr), aWord(false){;}
		void setParent(Node* n) {parent = n;}
		void setAWord() {aWord = true;}
		bool isAWord() {return aWord;}
		void addChild(Node* n) {children.push_back(n);}
		
		char getValue() {return value;}
		vector<Node*>& getChildren() {return children;}
		int getSize() {return children.size();}
	
	friend class Tree;
};

class Tree
{
	private:
		Node* root;
	
	public:
		Tree() {root = new Node(',');}
		
		Node* getRoot() {return root;}
		
		void addFirstWord(string s);
		void addWord(string s);
		Node* query(string s) const;
		void DFS(string s, Node* r) const;
		
		void print(Node* n) const;
};

void Tree::print(Node* n) const
{
	cout << n->getValue() << "\n";
	for(int i = 0; i < n->getSize(); i++)
	{
		this->print(n->getChildren()[i]);
	}
}

void Tree::addWord(string s)
{
	Node* cur = root;
	int count = -1;
	
	bool found;
	do{
		found = false;
		count++;
		for(int i = 0; i < cur->getSize(); i++)
		{
			if( s[count] == cur->getChildren()[i]->getValue() ) 
			{
				found = true;
				cur = cur->getChildren()[i];
				break;
			}
		}
	}while(found);
	
	while(count < s.size())
	{
		Node* next = new Node(s[count]);
		next->setParent(cur);
		cur->addChild(next);
		cur = next;
		count++;
	}
	cur->setAWord();
}

Node* Tree::query(string s) const
{
	Node* cur = root;
	
	for(int i = 0; i < s.size(); i++)
	{
		sig = true;
		for(int j = 0; j < cur->getSize(); j++)
		{
			if( s[i] == cur->getChildren()[j]->getValue() ) 
			{
				cur = cur->getChildren()[j];
				sig = false;
				break;
			}
		}
		
	}
	
	return cur;
}

void Tree::DFS(string s, Node* r) const
{
	if (sig)
	{
		cout << "\n";
		return;
	}
	ans += r->getValue();
	
	Node* cur = r;
	for(int i = 0; i < cur->getSize(); i++)
	{
		this->DFS(s, cur->getChildren()[i]);
	}
	if(cur->isAWord()) mypq.push(ans);
	ans = ans.substr(0, ans.size() - 1);	
}


int main()
{
	int n_word, n_query;
	cin >> n_word >> n_query;
	
	string word;
	
	//initialize tree with first word
	Tree prefix;
	
	for(int i = 0; i < n_word; i++)
	{
		cin >> word;
		prefix.addWord(word);
	}

//	prefix.print(prefix.getRoot());
	string query;
	for(int i = 0; i < n_query - 1; i++)
	{
		cin >> query;
		ans = query.substr(0, query.size() - 1);
		prefix.DFS(query, prefix.query(query));
		
		if (sig)
		{
			sig = false;
			continue;
		}
		cout << mypq.top();
		mypq.pop();
		while(!mypq.empty())
		{
			cout << " ";
			cout << mypq.top();
			mypq.pop();
		}
		cout << "\n";
	}
	cin >> query;
	ans = query.substr(0, query.size() - 1);
	prefix.DFS(query, prefix.query(query));
	
	if (sig) return 0;
	cout << mypq.top();
	mypq.pop();
	while(!mypq.empty())
	{
		cout << " ";
		cout << mypq.top();
		mypq.pop();
	}

	return 0;
}
