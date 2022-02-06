/*
DSAP 2019 Spring Hw1, Scheduling Algorithm
ver 1.0
Author: tim_ezemnng
Last Modified: 3/21/2019

Note:
Why does TA write compare as 
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*-----------------forward declaration------------------*/

class Ship;
class ShipList;
class Job;
class In;
class Out;
class Transfer;
class JobList;
bool ertCompare(Job& J1, Job& J2);
bool sptCompare(Job& J1, Job& J2);
bool sptDebugMode = 0;
bool ertDebugMode = 0;

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
		void print() const{ cout << id << "," << wei << "," << nat << "," << cap << "," << dan << endl;};
		
		friend class ShipList;
		friend class In;
		friend class Out;
		friend class Transfer;
		friend bool ertCompare(Job& J1, Job& J2);
		friend bool sptCompare(Job& J1, Job& J2);
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
		int oper; //operation time
		string type;
		int port;
		int start;
		int end;
		int realDep;
		
	public:
		Job(Ship* S, int d, int o, string t);
		//getters
		int getRealEnd() const {return realDep + oper;}
		int getWaitingTime() const{return realDep - dep;}
		//setters
		void setRealDep(int rTime){realDep = rTime;}
		
		virtual void print() const = 0;
		
		friend class JobList;
		friend bool ertCompare(Job& J1, Job& J2);
		friend bool sptCompare(Job& J1, Job& J2);
};

class In : public Job
{
	private:
	public:
		In(Ship* S, int d, int o, string t, int p, int e);
		void print() const;
		
		friend class JobList;
};

class Out : public Job
{
	private:
	public:
		Out(Ship* S, int d, int o, string t, int p, int s);
		void print() const;
		
		friend class JobList;
};

class Transfer : public Job
{
	private:
	public:
		Transfer(Ship* S, int d, int o, string t, int s, int e);
		void print() const;
		
		friend class JobList;
};

class JobList
{
	private:
		Job** jobList;
		int jobCapacity;
		int jobNum;

		void swap(Job** swapList, int i, int j);
		
	public:
		JobList(int numOfJobs);
		~JobList();
		void addJob(Job& J);
		
		int ERT();
		int SPT();
		
		void printJobs() const;
		Job* operator[] (int index) const {return jobList[index];}
};


/*-----------------------Global Functions--------------------------*/


bool ertCompare(Job& J1, Job& J2)
{
//	cout << (J1.dep < J2.dep) << (J1.oper < J2.oper) << (J1.shipPtr->id < J2.shipPtr->id) << "hi\n";
	//compare start time
	if(J1.dep < J2.dep) return true;
	else if(J1.dep > J2.dep) return false;

	//compare working duration time
	if(J1.oper < J2.oper) return true;
	else if(J1.oper > J2.oper) return false;
	
	//compare ship name
	if(J1.shipPtr->id < J2.shipPtr->id) return true;
	else return false;
}


bool sptCompare(Job& J1, Job& J2)
{
	//compare working duration time
	if(J1.oper < J2.oper) return true;
	else if(J1.oper > J2.oper) return false;
	
	//compare start time
	if(J1.dep < J2.dep) return true;
	else if(J1.dep > J2.dep) return false;
	
	//compare ship name
	if(J1.shipPtr->id < J2.shipPtr->id) return true;
	else return false;
}


/*-----------------------Main Function--------------------------*/


int main()
{
	int m, s = 0, n = 0; //number of piers, ships, jobs
	string ship, job, tok;

	cin >> m >> ship >> job;
	ifstream myfile;
	
	//counting s and n
	myfile.open(ship);
	while(getline(myfile, tok)) s++;
	myfile.close();
	
	myfile.open(job);
	while(getline(myfile, tok)) n++;
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
	string DEP_H; //time of departure
	string DEP_M; 
	int dep_h;
	int dep_m;

	string OP_M; //time needed for operation
	int op_m;

	string type; //different usage depending on type
	string NUM1, NUM2;
	int num1, num2;

	myfile.open(job);
	for(int i = 0; i < n; i++)
	{
		getline(myfile, id, ',');
		getline(myfile, DEP_H, ':');
		getline(myfile, DEP_M, ',');
		getline(myfile, OP_M, ',');
		getline(myfile, type, ',');
		getline(myfile, NUM1, ',');
		getline(myfile, NUM2);
		
		//working on departure time
		dep_h = stoi(DEP_H);
		dep_m = stoi(DEP_M);
		int dep = 60*dep_h + dep_m;
		
		//working on operation time
		op_m = stoi(OP_M);
		int op = op_m;
		
		//working on last 2 integers whose usage depend on job type
		num1 = stoi(NUM1);
		num2 = stoi(NUM2);
		
		if (type == "I")
		{
			In in(sl.search(id), dep, op, type, num1, num2);
			jl.addJob(in);
			continue;
		}
		
		if (type == "T")
		{
			Transfer transfer(sl.search(id), dep, op, type, num1, num2);
			jl.addJob(transfer);
			continue;
		}
		
		if (type == "O")
		{
			Out out(sl.search(id), dep, op, type, num1, num2);
			jl.addJob(out);
			continue;
		}

	}
	myfile.close();

	cout << jl.ERT() << " " << jl.SPT();

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

Job::Job(Ship* S, int d, int o, string t) : shipPtr(S), dep(d), oper(o), type(t) {}

In::In(Ship* S, int d, int o, string t, int p, int e) : Job(S, d, o, t)
{
	port = p, end = e, start = -1;
}

void In::print() const
{
	int h = dep/60;
	int m = dep%60;
	
	cout << "In Ship ID: " << shipPtr->id << endl << "Departure Time: " << h << ":" << m << endl;
	cout << "Operation Time: " << oper << endl;
	cout << "Port: " << this->port << endl << "End: " << this->end << endl << endl;
}

Out::Out(Ship* S, int d, int o, string t, int p, int s) : Job(S, d, o, t)
{
	port = p, start = s, end = -1;
}

void Out::print() const
{
	int h = dep/60;
	int m = dep%60;
	
	cout << "Out Ship ID: " << shipPtr->id << endl << "Departure Time: " << h << ":" << m << endl;
	cout << "Operation Time: " << oper << endl;
	cout << "Port: " << this->port << endl << "Start: " << this->start << endl << endl;
}

Transfer::Transfer(Ship* S, int d, int o, string t, int s, int e) : Job(S, d, o, t)
{
	start = s, end = e, port = -1;
}

void Transfer::print() const
{
	int h = dep/60;
	int m = dep%60;
	
	cout << "Transfer Ship ID: " << shipPtr->id << endl << "Departure Time: " << h << ":" << m << endl;
	cout << "Operation Time: " << oper << endl;
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

//swap task
void JobList::swap(Job** swapList, int i, int j)
{
	Job* temp = swapList[i];
	swapList[i] = swapList[j];
	swapList[j] = temp;
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
		if (J.type == "I") newJob = new In(J.shipPtr, J.dep, J.oper, J.type, J.port, J.end);
		if (J.type == "O") newJob = new Out(J.shipPtr, J.dep, J.oper, J.type, J.port, J.start);
		if (J.type == "T") newJob = new Transfer(J.shipPtr, J.dep, J.oper, J.type, J.start, J.end);
		
		jobList[jobNum] = newJob;
		jobNum++;
	}
}

int JobList::ERT()
{
	
	//shallow copy taskList
	Job **sortedList = new Job*[jobNum];
	for(int i = 0; i < jobNum; i++)
	{
		sortedList[i] = jobList[i];
	}
	
	//sort
	for(int i = 0; i < jobNum; i++)
	{
		for(int j = i + 1; j < jobNum; j++)
		{
			if(!ertCompare(*sortedList[i],*sortedList[j])) swap(sortedList, i, j);
		}
	}

	//calculate waiting time
	int totalWaitingTime = 0;

	//count 
	int port1Count = 0;
	int port2Count = 0;
	for(int i = 0; i < jobNum; i++)
	{
		if(sortedList[i]->port == 1) port1Count++;
		else if(sortedList[i]->port == 2) port2Count++;	 
	}
	
	Job** port1Job = new Job*[port1Count];
	Job** port2Job = new Job*[port2Count];

	//reset to zero
	port1Count = 0;
	port2Count = 0;
	Job* curJob;
	for(int i = 0; i < jobNum; i++)
	{
		curJob = sortedList[i];
//		cout << "i = " << i << endl;
//		curJob->print();
		
		//skip T type task 
		if(curJob->type == "T") continue;
		
		int portNum = curJob->port;
		
		//empty: directly insert
		if(portNum == 1 && port1Count== 0)
		{
			curJob->setRealDep(curJob->dep);
			port1Job[port1Count] = curJob;
			port1Count++;
		}
		else if(portNum == 2 && port2Count== 0)
		{
			curJob->setRealDep(curJob->dep);
			port2Job[port2Count] = sortedList[i];
			port2Count++;
		}
		//non empty
		else if(portNum == 1)
		{
			
			//head: waiting time = 0
			if(curJob->dep + curJob->oper <= port1Job[0]->dep)
			{
				//move and insert
				for(int j = port1Count; j > 0; j--)
				{
					port1Job[j] = port1Job[j - 1]; 
				}
				curJob->setRealDep(curJob->dep);
				port1Job[0] = curJob;
				port1Count++;
			}
			else
			{
				//body
				bool isFound = false;
				//find insertion point
				for(int j = 1; j < port1Count; j++)
				{
					//
					if(max( curJob->dep, port1Job[j - 1]->getRealEnd() ) + curJob->oper <= port1Job[j]->realDep)
					{
						//move
						for(int k = port1Count; k > j; k--)
						{
							port1Job[k] = port1Job[k - 1]; 
						}
						//insert
						curJob->setRealDep(max( curJob->dep, port1Job[j - 1]->getRealEnd() ));
						port1Job[j] = curJob;
						
						//add waiting time
						totalWaitingTime += port1Job[j]->getWaitingTime();
						
						port1Count++;
						isFound = true;
						break;
					}
				}
				//tail
				if(!isFound)
				{
					//insert
					curJob->setRealDep(max( curJob->dep, port1Job[port1Count - 1]->getRealEnd() ));
					port1Job[port1Count] = curJob;
					
					//add waiting time
					totalWaitingTime += port1Job[port1Count]->getWaitingTime();
					
					port1Count++;
				}
			}	
		}
		else if(portNum == 2)
		{

			//head: waiting time = 0
			if(curJob->dep + curJob->oper <= port2Job[0]->dep)
			{
				//move and insert
				for(int j = port2Count; j > 0; j--)
				{
					port2Job[j] = port2Job[j - 1]; 
				}
				curJob->setRealDep(curJob->dep);
				port2Job[0] = curJob;
				port2Count++;
			}
			else
			{
				//body
				bool isFound = false;
				//find insertion point
				for(int j = 1; j < port2Count; j++)
				{
					//
					if(max( curJob->dep, port2Job[j - 1]->getRealEnd() ) + curJob->oper <= port2Job[j]->realDep)
					{
						//move
						for(int k = port2Count; k > j; k--)
						{
							port2Job[k] = port2Job[k - 1]; 
						}
						//insert
						curJob->setRealDep(max( curJob->dep, port2Job[j - 1]->getRealEnd() ));
						port2Job[j] = curJob;
						
						//add waiting time
						totalWaitingTime += port2Job[j]->getWaitingTime();
						
						port2Count++;
						isFound = true;
						break;
					}
				}
				//tail
				if(!isFound)
				{
					//insert
					curJob->setRealDep(max( curJob->dep, port2Job[port2Count - 1]->getRealEnd() ));
					port2Job[port2Count] = curJob;
					
					//add waiting time
					totalWaitingTime += port2Job[port2Count]->getWaitingTime();
					port2Count++;
				}
			}	
		}
		
		//debug message
		if(ertDebugMode)
		{
			cout << "Port " << curJob->port << endl;
			curJob->print();
			cout << "---------------------PORT 1---------------------------" << endl;
			for(int p = 0; p < port1Count; p++)
			{
				port1Job[p]->print();
			}	
			cout << "---------------------PORT 2---------------------------" << endl;
			for(int p = 0; p < port2Count; p++)
			{
				port2Job[p]->print();
			}	
			cout << "Accu waiting Time: " << totalWaitingTime << endl << endl;
		}	
	}
	return totalWaitingTime;
}

int JobList::SPT()
{
	//shallow copy taskList
	Job **sortedList = new Job*[jobNum];
	for(int i = 0; i < jobNum; i++)	sortedList[i] = jobList[i];
	
	//sort
	for(int i = 0; i < jobNum; i++)
	{
		for(int j = i + 1; j < jobNum; j++)
			if(!sptCompare(*sortedList[i], *sortedList[j]))	swap(sortedList, i, j);
	}

	//calculate waiting time
	int totalWaitingTime = 0;

	//count 
	int port1Count = 0;
	int port2Count = 0;
	for(int i = 0; i < jobNum; i++)
	{
		if(sortedList[i]->port == 1) port1Count++;
		else if(sortedList[i]->port == 2) port2Count++;	 
	}
	
	Job** port1Job = new Job*[port1Count];
	Job** port2Job = new Job*[port2Count];
	
	//reset to zero
	port1Count = 0;
	port2Count = 0;
	
	Job* curJob;
	for(int i = 0; i < jobNum; i++)
	{
		curJob = sortedList[i];
		
		//skip T type task 
		if(curJob->type == "T") continue;
		
		int portNum = curJob->port;
		
		//empty: directly insert
		if(portNum == 1 && port1Count== 0)
		{
			curJob->setRealDep(curJob->dep);
			port1Job[port1Count] = curJob;
			port1Count++;
		}
		else if(portNum == 2 && port2Count== 0)
		{
			curJob->setRealDep(curJob->dep);
			port2Job[port2Count] = curJob;
			port2Count++;
		}
		//non empty
		else if(portNum == 1)
		{
			//head: waiting time = 0
			if(curJob->dep + curJob->oper <= port1Job[0]->realDep)
			{
				//move and insert
				for(int j = port1Count; j > 0; j--)
					port1Job[j] = port1Job[j - 1];
					
				curJob->setRealDep(curJob->dep);
				port1Job[0] = curJob;
				port1Count++;
			}
			else
			{
				//body
				bool isFound = false;
				//find insertion point
				for(int j = 1; j < port1Count; j++)
				{
					//
					if(max( curJob->dep, port1Job[j - 1]->getRealEnd() ) + curJob->oper <= port1Job[j]->realDep)
					{
						//move
						for(int k = port1Count; k > j; k--)
						{
							port1Job[k] = port1Job[k - 1]; 
						}
						//insert
						curJob->setRealDep(max( curJob->dep, port1Job[j - 1]->getRealEnd() ));
						port1Job[j] = curJob;
						
						//add waiting time
						totalWaitingTime += port1Job[j]->getWaitingTime();
						
						port1Count++;
						isFound = true;
						break;
					}
				}	
				//tail
				if(!isFound)
				{
					//insert
					curJob->setRealDep(max( curJob->dep, port1Job[port1Count - 1]->getRealEnd() ));
					port1Job[port1Count] = curJob;
					
					//add waiting time
					totalWaitingTime += port1Job[port1Count]->getWaitingTime();
					
					port1Count++;
				}
			}	
		}
		else if(portNum == 2)
		{
			//head: waiting time = 0
			if(curJob->dep + curJob->oper <= port2Job[0]->realDep)
			{
				//move and insert
				for(int j = port2Count; j > 0; j--)
					port2Job[j] = port2Job[j - 1];
					
				curJob->setRealDep(curJob->dep);
				port2Job[0] = curJob;
				port2Count++;
			}
			else
			{
				//body
				bool isFound = false;
				//find insertion point
				for(int j = 1; j < port2Count; j++)
				{
					//
					if(max( curJob->dep, port2Job[j - 1]->getRealEnd() ) + curJob->oper <= port2Job[j]->realDep)
					{
						//move
						for(int k = port2Count; k > j; k--)
						{
							port2Job[k] = port2Job[k - 1]; 
						}
						//insert
						curJob->setRealDep(max( curJob->dep, port2Job[j - 1]->getRealEnd() ));
						port2Job[j] = curJob;
						
						//add waiting time
						totalWaitingTime += port2Job[j]->getWaitingTime();
						
						port2Count++;
						isFound = true;
						break;
					}
				}
				//tail
				if(!isFound)
				{
					//insert
					curJob->setRealDep(max( curJob->dep, port2Job[port2Count - 1]->getRealEnd() ));
					port2Job[port2Count] = curJob;
					
					//add waiting time
					totalWaitingTime += port2Job[port2Count]->getWaitingTime();
					
					port2Count++;
				}
			}	
		}
		if(sptDebugMode)
		{
			cout << "i = " << i << endl;
			curJob->print();
			cout << "---------------------PORT 1---------------------------" << endl;
			for(int p = 0; p < port1Count; p++)
			{
				port1Job[p]->print();
			}	
			cout << "---------------------PORT 2---------------------------" << endl;
			for(int p = 0; p < port2Count; p++)
			{
				port2Job[p]->print();
			}	
			cout << "Accu waiting Time: " << totalWaitingTime << endl << endl;
		}
	}
	return totalWaitingTime;
}
