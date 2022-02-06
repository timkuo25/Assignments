/*
DSAP 2019 Spring Hw3-3, Maximum Flow Problem using augmenting path algorithm
ver 1.0
Author: tim_ezemnng
Last Modified: 4/3/2019

Note: weird
*/

#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

class Node;
class Edge;
class Path;
class Graph;

bool cmp(Edge* EPtr1, Edge* EPtr2);

//Node(string n = "n"), string name, int eCount, vector<Edge*> ePtrList, void addEPtr(Edge& e);
class Node 
{
	private:
		string name;
		int eCount;
		vector<Edge*> ePtrList;
	public:
		Node(string n = "n") : name(n), eCount(0) {};
		void addEPtr(Edge& e);
		void print() const {cout << "Node " << name << " hi" << endl;}
				
	friend class Edge;
	friend class Path;
	friend class Graph;
	friend bool cmp(Edge* EPtr1, Edge* EPtr2);
};

void Node::addEPtr(Edge& e) // add edge
{
	Edge* ePtr = &e;
	ePtrList.push_back(ePtr);
	eCount++;
}

//Edge(int w = 0), Node* nPtr1,	Node* nPtr2, int wei, bool dir
class Edge
{
	private:
		Node* nPtr1;
		Node* nPtr2;
		int wei;
	public:
		Edge (int w = 0) : wei(w) {};
		//getters
		int getWei() const {return wei;};
		//setters
		void setNPtr1(Node* np1) {nPtr1 = np1;}
		void setNPtr2(Node* np2) {nPtr2 = np2;}
		
		void print() const {cout << nPtr1->name << " " << nPtr2->name << " " << wei << endl;}
		
	friend class Node;
	friend class Graph;
	friend bool cmp(Edge* EPtr1, Edge* EPtr2);
};

class Path
{
	private:
		vector<Node*> nPtrList;
		int flow;
		int len;
	public:
		Path() : flow(9999), len(0) {}; 
		Path(const Path& p);
		//getters
		int getLen() const {return len;}
		int getFlow() const {return flow;}
		//setters
		void setFlow(int f) {flow = f;}
		
		void addNodePtr(Node* np);
		
		void print() const;
		
	friend class Graph;
};

Path::Path(const Path& p)
{
	for(int i = 0; i < p.nPtrList.size(); i++)
		this->nPtrList.push_back(p.nPtrList[i]);
	this->flow = p.flow;
	this->len = p.len;
}

void Path::addNodePtr(Node* np)
{
	this->nPtrList.push_back(np);
	len++;
}

void Path::print() const
{
	for(int i = 0; i < len - 1; i++)
		cout << nPtrList[i]->name << ",";
	cout << nPtrList[len - 1]->name << ";" << flow;
}

class Graph
{
	private:
		int nCount;
		int eCount;
		int pCount;
		int index;
		Node* s;
		Node t;
		vector<Node> nList;
		vector<Edge> eList;
		vector<Path> pList;
		
	public:
		Graph() : nCount(0), eCount(0), pCount(0) {};
		//getters
		int getECount() const {return eCount;}
		int getIndex() const {return index;}
		
		//setters
		void setIndex(int i) {index = i;}
		void setS() {s = &(nList[0]);}
		void setT() {t = nList[nCount - 1];}
		
		void addN(Node& n);
		void addE(Edge& e);
		void addP(Path& p);
		Node* findNodeByString(string n);
		void addEToN(int n1);
		void sortNodeEPtrList();
		Path findOneAugPath();
		void printResult();
		
		void print() const;
};

void Graph::addN(Node& n)
{
	nList.push_back(n);
	nCount++;
}

void Graph::addE(Edge& e)
{
	eList.push_back(e);
	eCount++;
}

void Graph::addP(Path& p)
{
	pList.push_back(p);
	pCount++;
	
	// reduce weight of edges according to augmenting path
	for(int i = 0; i < p.nPtrList.size() - 1; i++) //0 1 2
	{
		string s1 = p.nPtrList[i]->name;
		string s2 = p.nPtrList[i + 1]->name;
		for(int j = 0; j < eList.size(); j++)
		{
			if ( (s1 == eList[j].nPtr1->name) and (s2 == eList[j].nPtr2->name) ) 
			{
				eList[j].wei -= p.getFlow();
				break;
			}
		}	
	}
	//add edges to graph, set Ntprs of edges
	for(int i = 0; i < p.nPtrList.size() - 1; i++)
	{
		string s1 = p.nPtrList[i]->name;
		string s2 = p.nPtrList[i + 1]->name;
		bool repeat = false;
		int jR = 0;
		for(int j = 0; j < index; j++)
		{
			if ( (s2 == eList[j].nPtr1->name) and (s1 == eList[j].nPtr2->name) ) 
			{
				jR = j;
				repeat = true;
				break;
			}
		}
		
		if(repeat) eList[jR].wei += p.getFlow();
		else
		{
			Edge e(p.getFlow());
			//set nPtrs to address of elements of nList
			e.setNPtr1( findNodeByString(p.nPtrList[i + 1]->name) );
			e.setNPtr2( findNodeByString(p.nPtrList[i]->name) );
			addE(e);
		}
	}
	addEToN(index);
	sortNodeEPtrList();	
	
	setIndex(eCount);
}

void Graph::printResult()
{
	if (!pList.empty())
	{
		int max = 0;
		for(int i = 0; i < pList.size(); i++)
		{
			pList[i].print();
			cout << endl;
			max += pList[i].flow;
		}
		cout << max;
	}
	else cout << "0";
}

Node* Graph::findNodeByString(string n)
{
	for(int i = 0; i < nCount; i++)
	{
		if (nList[i].name == n) return &(nList[i]);
	}
	return nullptr;
}

void Graph::addEToN(int n1)
{
	for(int i = n1; i < eCount; i++)
	{
		(eList[i].nPtr1)->addEPtr(eList[i]);
	}
}

void Graph::sortNodeEPtrList()
{
	for(int i = 0; i < nCount; i++)
	{
		int nextECount = nList[i].ePtrList.size();
		for(int j = 0; j < nextECount; j++)
		{
			for(int k = j + 1; k < nextECount; k++)
			{
				if(!cmp(nList[i].ePtrList[j], nList[i].ePtrList[k])) swap(nList[i].ePtrList[j], nList[i].ePtrList[k]);
			}
		}
	}
}

Path Graph::findOneAugPath()
{
	queue<Path> q;
	Path p1;
	
	p1.addNodePtr(s);
	
	q.push(p1);
	
	while(!q.empty())
	{
		for(int i = 0; i < q.front().nPtrList.back()->ePtrList.size() ; i++)
		{
			bool loop = false;
			Path p2(q.front());
			
			for(int j = 0; j < p2.nPtrList.size(); j++)
			{
				
				if( q.front().nPtrList.back()->ePtrList[i]->nPtr2->name ==  p2.nPtrList[j]->name)
				{
					loop = true;
					break;
				}
			}
			if (loop) continue;
			if (min( q.front().flow, q.front().nPtrList.back()->ePtrList[i]->wei ) == 0) continue;
			
			p2.setFlow( min( q.front().flow, q.front().nPtrList.back()->ePtrList[i]->wei ) );
			p2.addNodePtr( q.front().nPtrList.back()->ePtrList[i]->nPtr2 );
	
			if ( q.front().nPtrList.back()->ePtrList[i]->nPtr2->name == t.name ) return p2;		
			q.push(p2);
		}
		q.pop();
	}
	Path p3;
	p3.setFlow(-1);
	return p3;
}

void Graph::print() const
{
	cout << "**********\n eCount = " << eCount << ", index = " << index << endl;
//	cout << "size = " << nList.size() << endl;
	for(int i = 0; i < nCount; i++)
	{
		cout << "Node " << nList[i].name << endl;
		nList[0].ePtrList[0]->print();
	//	cout << nList[i].ePtrList.size() << endl;
	//	for (int j = 0; j < nList[i].ePtrList.size(); j++)
	//		nList[i].ePtrList[j]->print();
		cout << "**********\n";
	}
}

bool cmp(Edge* EPtr1, Edge* EPtr2)
{
	//compare weight
	if(EPtr1->wei > EPtr2->wei) return true;
	else if(EPtr1->wei < EPtr2->wei) return false;

	//if same weight compare node name
	if((EPtr1->nPtr2)->name < (EPtr2->nPtr2)->name) return true;
	else return false;
}

int main()
{
	Graph g;
	int nNum, eNum;
	string NNUM, ENUM;
	
	//set basic graph information
	istream& graphInf = cin;
	getline(graphInf, NNUM, ',');
	getline(graphInf, ENUM);
	
	nNum = stoi(NNUM);
	eNum = stoi(ENUM);
	
	//add nodes to graph
	istream& nodes = cin;
	
	for(int i = 0; i < nNum; i++)
	{
		string nName;
		if (i == nNum - 1)
		{
			getline(nodes, nName);
			Node n(nName);
			g.addN(n);
			break;
		}
		getline(nodes, nName, ',');
		Node n(nName);
		g.addN(n);
	}

	//add edges to graph, set Ntprs of edges
	for(int i = 0; i < eNum; i++)
	{
		string N1, N2, WEI;
		int wei;
		
		istream& eInf = cin;
		getline(eInf, N1, ',');
		
		getline(eInf, N2, ',');
		getline(eInf, WEI);

		wei = stoi(WEI);
		
		Edge e(wei);
		//set nPtrs to address of elements of nList
		e.setNPtr1(g.findNodeByString(N1));
		e.setNPtr2(g.findNodeByString(N2));
		g.addE(e);
	}
	
	//add edges to information of nodes
	g.addEToN(0);
	g.sortNodeEPtrList();
	
	g.setS();
	g.setT();
	g.setIndex(g.getECount());
	
	while(1)
	{
		Path aug(g.findOneAugPath());
		if (aug.getFlow() == -1) break;
		
		g.addP(aug);
	}
	g.printResult();
}
