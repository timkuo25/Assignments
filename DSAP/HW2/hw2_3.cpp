/*
DSAP 2019 Spring Hw2-3, Operating Time Forecasting and Exception Handling
ver 1.0
Author: tim_ezemnng
Last Modified: 3/24/2019

Note:
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cstdlib>
using namespace std;

/*-----------------forward declaration------------------*/

class Ship;
class ShipList;
class Job;
class In;
class Out;
class Transfer;
class JobList;

/*----------------------classes-----------------------*/ 

class Ship
{
	private:
		string id; 
		int wei; 
		string nat; //nationality
		string cap; //captain name
		bool dan; //dangerous?
	public:
		Ship(string i, int w, string n, string c, bool d) : id(i), wei(w), nat(n), cap(c), dan(d) {};
		//getters
		int getWei() const{return wei;}
		bool getDan() const{return dan;}
		void print() const{ cout << id << "," << wei << "," << nat << "," << cap << "," << dan << endl;};
		
		friend class ShipList;
		friend class In;
		friend class Out;
		friend class Transfer;
};

class ShipList
{
	private:
		int shipCapacity;
		int shipNum;
		Ship **shipList;
	public:
		ShipList(int numOfShips);
		~ShipList();
		void addShip(Ship& S);
		Ship* search(string n) const;
		void printShips() const;
};

class Job
{
	protected:
		Ship* shipPtr;
		int dep; //departure time
		string type;
		int port;
		int start;
		int end;
		
	public:
		Job(Ship* S, int d, string t);
		//getters
		Ship* getShipPtr() const {return shipPtr;}
		int getDep() const{return dep;}
		string getType() const{return type;}
		int getPort() const{return port;}
		int getStart() const{return start;}
		int getEnd() const{return end;}
		virtual void print() const = 0;
		
		friend class JobList;
};

class In : public Job
{
	private:
	public:
		In(Ship* S, int d, string t, int p, int e);
		void print() const;
		
		friend class JobList;
};

class Out : public Job
{
	private:
	public:
		Out(Ship* S, int d, string t, int p, int s);
		void print() const;
		
		friend class JobList;
};

class Transfer : public Job
{
	private:
	public:
		Transfer(Ship* S, int d, string t, int s, int e);
		void print() const;
		
		friend class JobList;
};

class JobList
{
	private:
		Job** jobList;
		int jobCapacity;
		int jobNum;		
	public:
		JobList(int numOfJobs);
		~JobList();
		void addJob(Job& J);
		
		void printJobs() const;
		Job* operator[] (int index) const {return jobList[index];}
};


/*-----------------------Global Functions--------------------------*/


/*-----------------------Main Function--------------------------*/


int main()
{
	int m, t1, t2, t3;
	double a0, a1, a2, a3, a4, b0, b1, b2, b3, b4, c0, c1, c2, c3, c4;
	int s = 0, n = 0; //number of piers, ships, jobs
	string ship, job, temp;

	string name;		
	istream& is = cin;
	getline(is, name, ',');
	m = stoi(name);
	getline(is, name, ',');
	a0 = stod(name);
	getline(is, name, ',');
	a1 = stod(name);
	getline(is, name, ',');
	a2 = stod(name);
	getline(is, name, ',');
	a3 = stod(name);
	getline(is, name, ',');
	a4 = stod(name);
	getline(is, name, ',');
	b0 = stod(name);
	getline(is, name, ',');
	b1 = stod(name);
	getline(is, name, ',');
	b2 = stod(name);
	getline(is, name, ',');
	b3 = stod(name);
	getline(is, name, ',');
	b4 = stod(name);
	getline(is, name, ',');
	c0 = stod(name);
	getline(is, name, ',');
	c1 = stod(name);
	getline(is, name, ',');
	c2 = stod(name);
	getline(is, name, ',');
	c3 = stod(name);
	getline(is, name, ',');
	c4 = stod(name);
	getline(is, name, ',');
	t1 = stod(name);
	getline(is, name, ',');
	t2 = stod(name);
	getline(is, name);
	t3 = stod(name);

	cin >> ship >> job;
	ifstream myfile;
	//counting s and n
	myfile.open(ship);
	while(getline(myfile, temp)) s++;
	myfile.close();
	
	myfile.open(job);
	while(getline(myfile, temp)) n++;
	myfile.close();
	
	//creating arrays, might be modified later
	ShipList sl(s); //ship list
	JobList jl(n); //job list
	
	//assigning value to ships, upper letters for strings to cast
	string id; 
	string WEIGHT;
	int weight; 
	string nat; //nationality
	string cap; //captain name
	string DAN;
	bool dan; //dangerous?
	myfile.open(ship);
	for(int i = 0; i < s; i++)
	{
		getline(myfile, id, ',');
		getline(myfile, WEIGHT, ',');
		getline(myfile, nat, ',');
		getline(myfile, cap, ',');
		getline(myfile, DAN);
		dan = (DAN == "Y") ? 1 : 0;
		weight = stoi(WEIGHT);
		
		Ship ship(id, weight, nat, cap, dan);
		sl.addShip(ship);
	//	sa[i].print();
	}
	myfile.close();
	
	//assigning value to jobs, upper letters for strings to cast
	string DEP;
	string DEP_H; //time of departure
	string DEP_M;
	int dep;
	int dep_h;
	int dep_m;

	string type; //different usage depending on type
	string NUM1;
	string NUM2;
	int num1, num2;
	int exptCnt = 0;
	myfile.open(job);
	
	for(int i = 0; i < n; i++)
	{
		try
		{
			getline(myfile, id, ',');
			getline(myfile, DEP, ',');
			getline(myfile, type, ',');
			getline(myfile, NUM1, ',');
			getline(myfile, NUM2);
			if (DEP == "" or type == "" or NUM1 == "" or NUM2 == "") throw exception();
			
			istringstream ss(DEP);
			getline(ss, DEP_H, ':');
			getline(ss, DEP_M);
			dep_h = stoi(DEP_H);
			dep_m = stoi(DEP_M);
			dep = 60*dep_h + dep_m;
			num1 = stoi(NUM1);
			num2 = stoi(NUM2);
			
			if (type == "I")
			{
				In in(sl.search(id), dep, type, num1, num2);
				jl.addJob(in);
				continue;
			}
			if (type == "O")
			{
				Out out(sl.search(id), dep, type, num1, num2);
				jl.addJob(out);
				continue;
			}
			if (type == "T")
			{
				Transfer transfer(sl.search(id), dep, type, num1, num2);
				jl.addJob(transfer);
				continue;
			}
		}
		catch (exception e)
		{
			Transfer transfer(sl.search(id), -1, "T", -1, -1);
			jl.addJob(transfer);
			exptCnt++;
			continue;
		}
	}
	myfile.close();

	//building model
	int MovingDST;
	int ShipWeight; 
	bool Danger;
	bool Night;
	int result;
	for(int i = 0; i < n; i++)
	{
		if (jl[i]->getDep() == -1)
		{
			cout << "-1";
			if (i != n - 1) cout << ",";
			else cout << "\n";
			continue;
		}
		Night = (jl[i]->getDep() <= 360 or jl[i]->getDep() >= 1080) ? 1 : 0;
		Danger = jl[i]->getShipPtr()->getDan();
		ShipWeight = jl[i]->getShipPtr()->getWei();
		
		if (jl[i]->getType() == "I")
		{
			bool even = !(jl[i]->getEnd() % 2); //t3 or not
			
			int inside; // *t2
			if (jl[i]->getPort() == 1)
			{
				if (even) inside = jl[i]->getEnd() / 2;
				else inside = (jl[i]->getEnd() + 1) / 2;
			}
			if (jl[i]->getPort() == 2)
			{
				if (even) inside = ( m - jl[i]->getEnd() ) / 2 + 1;
				else inside = ( m - jl[i]->getEnd() + 1) / 2;
			}
			
			MovingDST = t1 + inside*t2 + even*t3;
			result = a0 + a1*MovingDST + a2*ShipWeight + a3*Danger + a4*Night;
			cout << result;
			if (i != n - 1) cout << ",";
			else cout << "\n";
			continue;
		}
		if (jl[i]->getType() == "O")
		{
			bool even = !(jl[i]->getStart() % 2); //t3 or not
			
			int inside; // *t2
			if (jl[i]->getPort() == 1)
			{
				if (even) inside = jl[i]->getStart() / 2;
				else inside = (jl[i]->getStart() + 1) / 2;
			}
			if (jl[i]->getPort() == 2)
			{
				if (even) inside = ( m - jl[i]->getStart() ) / 2 + 1;
				else inside = ( m - jl[i]->getStart() + 1) / 2;
			}
			
			MovingDST = t1 + inside*t2 + even*t3;
			result = b0 + b1*MovingDST + b2*ShipWeight + b3*Danger + b4*Night;
			cout << result;
			if (i != n - 1) cout << ",";
			else cout << "\n";
			continue;
		}
		if (jl[i]->getType() == "T")
		{

			bool same = ( (jl[i]->getStart() - jl[i]->getEnd()) % 2 == 0 );
			
			int inside; // *t2
			if (same) inside = ( abs(jl[i]->getStart() - jl[i]->getEnd()) / 2);
			else
			{
				int large, little;
				large = max(jl[i]->getStart(), jl[i]->getEnd());
				little = min(jl[i]->getStart(), jl[i]->getEnd());
				
				if(large % 2 == 0) inside = (large - little) / 2;
				else inside = (large - little + 1) / 2;
			}
			
			MovingDST = inside*t2 + (!same)*t3;
			result = c0 + c1*MovingDST + c2*ShipWeight + c3*Danger + c4*Night;
			cout << result;
			if (i != n - 1) cout << ",";
			else cout << "\n";
			continue;
		}
	}
	cout << exptCnt;

	return 0;
}


/*-----------------------member functions------------------------*/

//constructor: make a shiplist
ShipList::ShipList(int numOfShips)
{
	shipNum = 0;
	shipCapacity = numOfShips;
	shipList = new Ship*[numOfShips];
	//initialize nullptr
	for(int i = 0; i < shipCapacity; i++)
	{
		shipList[i] = nullptr;
	}
}

//destructor: delete a shiplist
ShipList::~ShipList()
{
	for(int i = 0; i < shipNum; i++)
	{
		delete shipList[i];
		shipList[i] = nullptr;
	}
}

//print items in shiplist
void ShipList::printShips() const
{
	cout << "***************************************" << endl;
	for(int i = 0; i < shipNum; i++)
		shipList[i]->print();
	cout << "***************************************" << endl;
}

//add ship into shiplist
void ShipList::addShip(Ship& S)
{
	if(shipNum < shipCapacity)
	{	
		Ship* newShip = new Ship(S.id, S.wei, S.nat, S.cap, S.dan);
		shipList[shipNum] = newShip;
		shipNum++;
	}
}

Ship* ShipList::search(string n) const
{
	for(int i = 0; i < shipNum; i++)
	{
	//	cout << shipList[i]->id << " " << n << " " << " " << (shipList[i]->id.compare(n)) << "\n";
		if(shipList[i]->id == n) return shipList[i];
	}
	return nullptr;
}

Job::Job(Ship* S, int d, string t) : shipPtr(S), dep(d), type(t) {}

In::In(Ship* S, int d, string t, int p, int e) : Job(S, d, t)
{
	port = p, end = e, start = -1;
}

void In::print() const
{
	int h = dep/60;
	int m = dep%60;
	
	cout << "In Ship ID: " << shipPtr->id << endl << "Departure Time: " << h << ":" << m << endl;
	cout << "Port: " << this->port << endl << "End: " << this->end << endl << endl;
}

Out::Out(Ship* S, int d, string t, int p, int s) : Job(S, d, t)
{
	port = p, start = s, end = -1;
}

void Out::print() const
{
	int h = dep/60;
	int m = dep%60;
	
	cout << "Out Ship ID: " << shipPtr->id << endl << "Departure Time: " << h << ":" << m << endl;
	cout << "Port: " << this->port << endl << "Start: " << this->start << endl << endl;
}

Transfer::Transfer(Ship* S, int d, string t, int s, int e) : Job(S, d, t)
{
	start = s, end = e, port = -1;
}

void Transfer::print() const
{
	cout << "dep= " << dep << endl;
	int h = dep/60;
	int m = dep%60;
	
	cout << "Transfer Ship ID: " << shipPtr->id << endl << "Departure Time: " << h << ":" << m << endl;
	cout << "Start: " << this->start << endl << "End: " << this->end << endl << endl;
}

//constructor: create array of jobs
JobList::JobList(int numOfJobs)
{
	jobNum = 0;
	jobCapacity = numOfJobs;
	jobList = new Job*[numOfJobs];
}

//destructor: delete jobs in jobList
JobList::~JobList()
{
	for(int i = 0; i < jobNum; i++)
	{
		delete jobList[i];
		jobList[i] = nullptr;
	}
}

//print each task's infomation in taskList
void JobList::printJobs() const
{
	cout << "***********************************************" << endl;
	for(int i = 0; i < jobNum; i++)
		jobList[i]->print();
	cout << "***********************************************" << endl;
}

//add job into jobList
void JobList::addJob(Job& J)
{
	if(jobNum < jobCapacity)
	{
		Job* newJob;
		if (J.type == "I") newJob = new In(J.shipPtr, J.dep, J.type, J.port, J.end);
		if (J.type == "O") newJob = new Out(J.shipPtr, J.dep, J.type, J.port, J.start);
		if (J.type == "T") newJob = new Transfer(J.shipPtr, J.dep, J.type, J.start, J.end);
		
		jobList[jobNum] = newJob;
		jobNum++;
	}
}
