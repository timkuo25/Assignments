/*
DSAP 2019 Spring Hw3-4, Still Scheduling Algorithm
ver 1.0
Author: tim_ezemnng
Last Modified: 4/5/2019

Note: ERT and ertCompare is actually not ERT Algo but algo this problem required
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
bool searchPri(vector<string>& p, string s);
bool ertDebugMode = 0;

int b1, b2, b3, b4;
/*----------------------classes-----------------------*/ 

class Ship
{
	private:
		string id; 
		int wei; 
		string nat; //nationality
		string cap; //captain name
		bool dan; //dangerous?
		bool pri;
	public:
		Ship(string i, int w, string n, string c, bool d, bool p) : id(i), wei(w), nat(n), cap(c), dan(d), pri(p) {};
		void print() const{ cout << id << "," << wei << "," << nat << "," << cap << "," << dan << "," << pri << endl;};
		
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
		int getShipNum() {return shipNum;}
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
		int oper1;
		int oper2;
		string type;
		int port;
		int start;
		int end;
		int realDep;
		int occuStart;
		int occuEnd;
		
	public:
		Job(Ship* S, int d, int o1, int o2, string t);
		//getters
		virtual int getRealEnd() const = 0;
		virtual int getWaitingTime() const = 0;
		//setters
		virtual void setRealDep(int n) = 0; //for In
		virtual void setOccuStart(int n) = 0; //for Out
		virtual void setOccuEnd(int n) = 0;
		
		virtual void print() const = 0;
		
		friend class JobList;
		friend bool ertCompare(Job& J1, Job& J2);
		friend bool sptCompare(Job& J1, Job& J2);
};

class In : public Job
{
	private:
	public:
		In(Ship* S, int d, int o1, int o2, string t, int p, int e);
		//getters
		int getRealEnd() const {return occuEnd + oper2;}
		int getWaitingTime() const {return occuEnd + oper2 - dep - oper;}
		//setters
		void setRealDep(int num) {realDep = num;}
		void setOccuStart(int num) {occuStart = num;}
		void setOccuEnd(int num) {occuEnd = num;}
		void print() const;
		
		friend class JobList;
};

class Out : public Job
{
	private:
	public:
		Out(Ship* S, int d, int o1, int o2, string t, int p, int s);
		//getters
		int getRealEnd() const {return occuEnd;}
		int getWaitingTime() const {return occuEnd - dep - oper;}
		//setters
		void setRealDep(int num) {realDep = num;}
		void setOccuStart(int num) {occuStart = num;}
		void setOccuEnd(int num) {occuEnd = num;}
		void print() const;
		
		friend class JobList;
};

class Transfer : public Job
{
	private:
	public:
		Transfer(Ship* S, int d, int o, string t, int s, int e);
		//getters
		int getRealEnd() const {return dep + oper;}
		int getWaitingTime() const {return 0;}
		//setters
		void setRealDep(int num) {realDep = num;}
		void setOccuStart(int num) {occuStart = num;}
		void setOccuEnd(int num) {occuEnd = num;}
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
		
		void ERT();
		
		void printJobs() const;
		Job* operator[] (int index) const {return jobList[index];}
};


/*-----------------------Global Functions--------------------------*/
bool searchPri(vector<string>& p, string s)
{
	for(int i = 0; i < p.size(); i++)
		if (s == p[i]) return true;
	return false;
}

bool ertCompare(Job& J1, Job& J2)
{
	int s1, s2, r1, r2, p1, p2, d1, d2, q1, q2;
	r1 = J1.dep;
	p1 = J1.oper1 + J1.oper2;
	d1 = J1.shipPtr->dan;
	q1 = J1.shipPtr->pri;
	s1 = b1*r1 + b2*p1 - b3*d1 - b4*q1;
	
	r2 = J2.dep;
	p2 = J2.oper1 + J2.oper2;
	d2 = J2.shipPtr->dan;
	q2 = J2.shipPtr->pri;
	s2 = b1*r2 + b2*p2 - b3*d2 - b4*q2;

	//compare start time
	if(s1 < s2) return true;
	else if(s1 > s2) return false;

	//compare working duration time
	if(J1.shipPtr->dan > J2.shipPtr->dan) return true;
	else if(J1.shipPtr->dan < J2.shipPtr->dan) return false;
	
	if(J1.shipPtr->pri > J2.shipPtr->pri) return true;
	else if(J1.shipPtr->pri < J2.shipPtr->pri) return false;
	
	//compare ship name
	if(J1.shipPtr->id < J2.shipPtr->id) return true;
	else return false;
}

/*-----------------------Main Function--------------------------*/


int main()
{
	int m, s = 0, n = 0;
	string M, B1, B2, B3, B4, ship, job, naysh, tok;
	vector<string> priority;

	istream& ParaInf = cin;
	getline(ParaInf, M, ',');
	getline(ParaInf, B1, ',');
	getline(ParaInf, B2, ',');
	getline(ParaInf, B3, ',');
	getline(ParaInf, B4);
	
	m = stoi(M);
	b1 = stoi(B1);
	b2 = stoi(B2);
	b3 = stoi(B3);
	b4 = stoi(B4);

	cin >> ship >> job >> naysh;
	
	
	ifstream myfile;
	
	//counting s and n
	myfile.open(ship);
	while(getline(myfile, tok)) s++;
	myfile.close();
	
	myfile.open(job);
	while(getline(myfile, tok)) n++;
	myfile.close();
	
	myfile.open(naysh);
	while(getline(myfile, tok))
		priority.push_back(tok);
	myfile.close();
	
	//creating arrays, might be modified later
	ShipList sl(s); //ship list
	JobList jl(n); //job list
	
	//assigning value to ships, upper letters for strings to cast
	string id; 
	string WEIGHT;
	int weight;
	string NAT; 
	 //nationality
	string cap; //captain name
	string DAN;
	bool dan; //dangerous?
	bool pri;
	myfile.open(ship);
	for(int i = 0; i < s; i++)
	{
		string nat = "";
		getline(myfile, id, ',');
		getline(myfile, WEIGHT, ',');
		getline(myfile, NAT, ',');
		
		if (NAT.find(" ") != string::npos)
		{
			while(NAT.find(" ")!= string::npos)
			{
				size_t pos = NAT.find(" ");
				nat += NAT[0];
				NAT = NAT.substr(pos + 1, NAT.length() - pos);
			}
			nat += NAT[0];
		}
		else nat = NAT;
		
		getline(myfile, cap, ',');
		getline(myfile, DAN);
		dan = (DAN == "Y") ? 1 : 0;
		weight = stoi(WEIGHT);
		pri = searchPri(priority, nat);
		
		Ship ship(id, weight, nat, cap, dan, pri);
		sl.addShip(ship);
	//	sa[i].print();
	}
	myfile.close();
	
//	sl.printShips();
	
	//assigning value to jobs, upper letters for strings to cast
	string DEP_H; //time of departure
	string DEP_M; 
	int dep_h;
	int dep_m;

	string OP1, fourth;
	int op, op1, op2;

	string type; //different usage depending on type
	string NUM1, NUM2;
	int num1, num2;

	myfile.open(job);
	for(int i = 0; i < n; i++)
	{
		getline(myfile, id, ',');
		getline(myfile, DEP_H, ':');
		getline(myfile, DEP_M, ',');
		getline(myfile, OP1, ',');
		
		//working on departure time
		dep_h = stoi(DEP_H);
		dep_m = stoi(DEP_M);
		int dep = 60*dep_h + dep_m;
		
		getline(myfile, fourth, ',');

		if (fourth == "T")
		{
			getline(myfile, NUM1, ',');
			getline(myfile, NUM2);
			
			type = fourth;
			op = stoi(OP1);
			num1 = stoi(NUM1);
			num2 = stoi(NUM2);
			Transfer transfer(sl.search(id), dep, op, type, num1, num2);
			jl.addJob(transfer);
			continue;
		}
		
		op1 = stoi(OP1);
		op2 = stoi(fourth);
		getline(myfile, type, ',');
		getline(myfile, NUM1, ',');
		getline(myfile, NUM2);
		
		//working on last 2 integers whose usage depend on job type
		num1 = stoi(NUM1);
		num2 = stoi(NUM2);
		
		if (type == "I")
		{
			In in(sl.search(id), dep, op1, op2, type, num1, num2);
			jl.addJob(in);
			continue;
		}
		
		if (type == "O")
		{
			Out out(sl.search(id), dep, op1, op2, type, num1, num2);
			jl.addJob(out);
			continue;
		}
	}
	myfile.close();
	jl.ERT();

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
		Ship* newShip = new Ship(S.id, S.wei, S.nat, S.cap, S.dan, S.pri);
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

Job::Job(Ship* S, int d, int o1, int o2, string t) : shipPtr(S), dep(d), oper1(o1), oper2(o2), type(t)
{
	realDep = dep;
}

In::In(Ship* S, int d, int o1, int o2, string t, int p, int e) : Job(S, d, o1, o2, t)
{
	oper = o1 + o2;
	port = p, end = e, start = -1;
}

void In::print() const
{
	int h = dep/60;
	int m = dep%60;
	
	cout << "In Ship ID: " << shipPtr->id << endl << "Departure Time: " << h << ":" << m << endl;
	cout << "Operation Time1: " << oper1 << ", Operation Time2: " << oper2 << endl;
	cout << "Port: " << this->port << endl << "End: " << this->end << endl << endl;
}

Out::Out(Ship* S, int d, int o1, int o2, string t, int p, int s) : Job(S, d, o1, o2, t)
{
	oper = o1 + o2;
	port = p, start = s, end = -1;
}

void Out::print() const
{
	int h = dep/60;
	int m = dep%60;
	
	cout << "Out Ship ID: " << shipPtr->id << endl << "Departure Time: " << h << ":" << m << endl;
	cout << "Operation Time1: " << oper1 << ", Operation Time2: " << oper2 << endl;
	cout << "Port: " << this->port << endl << "Start: " << this->start << endl << endl;
}

Transfer::Transfer(Ship* S, int d, int o, string t, int s, int e) : Job(S, d, -1, -1, t)
{
	oper = o, start = s, end = e, port = -1;
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
		if (J.type == "I") newJob = new In(J.shipPtr, J.dep, J.oper1, J.oper2, J.type, J.port, J.end);
		if (J.type == "O") newJob = new Out(J.shipPtr, J.dep, J.oper1, J.oper2, J.type, J.port, J.start);
		if (J.type == "T") newJob = new Transfer(J.shipPtr, J.dep, J.oper, J.type, J.start, J.end);
		
		jobList[jobNum] = newJob;
		jobNum++;
	}
}

void JobList::ERT()
{
	
	//shallow copy taskList
	Job **sortedList = new Job*[jobNum];
	for(int i = 0; i < jobNum; i++)
		sortedList[i] = jobList[i];
	
	//sort
	for(int i = 0; i < jobNum; i++)
	{
		for(int j = i + 1; j < jobNum; j++)
		{
			if(!ertCompare(*sortedList[i],*sortedList[j])) swap(sortedList, i, j);
		}
	}

	//calculate waiting time
	int p1IWaitingTime = 0;
	int p1OWaitingTime = 0;
	int p2IWaitingTime = 0;
	int p2OWaitingTime = 0;

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
			if (curJob->type == "I")
			{
				curJob->setRealDep(curJob->dep);
				curJob->setOccuStart(curJob->realDep);
				curJob->setOccuEnd(curJob->occuStart + curJob->oper1);
			}
			else if (curJob->type == "O")
			{
				curJob->setRealDep(curJob->dep);
				curJob->setOccuStart(curJob->realDep + curJob->oper1);
				curJob->setOccuEnd(curJob->occuStart + curJob->oper2);
			}
			port1Job[port1Count] = curJob;
			port1Count++;
		}
		else if(portNum == 2 && port2Count== 0)
		{
			if (curJob->type == "I")
			{
				curJob->setRealDep(curJob->dep);
				curJob->setOccuStart(curJob->realDep);
				curJob->setOccuEnd(curJob->occuStart + curJob->oper1);
			}
			else if (curJob->type == "O")
			{
				curJob->setRealDep(curJob->dep);
				curJob->setOccuStart(curJob->realDep + curJob->oper1);
				curJob->setOccuEnd(curJob->occuStart + curJob->oper2);
			}
			port2Job[port2Count] = curJob;
			port2Count++;
		}
		//non empty
		else if(portNum == 1)
		{
			if (curJob->type == "I")
			{
				//head: waiting time = 0
				if (curJob->dep + curJob->oper1 <= port1Job[0]->occuStart)
				{
				//move and insert
					for(int j = port1Count; j > 0; j--)  //
						port1Job[j] = port1Job[j - 1];    //
						
					curJob->setRealDep(curJob->dep);
					curJob->setOccuStart(curJob->realDep);
					curJob->setOccuEnd(curJob->occuStart + curJob->oper1);
					port1Job[0] = curJob;  //
					port1Count++;	//
				}
				//body
				else
				{
					bool isFound = false;
					//find insertion point
					for(int j = 1; j < port1Count; j++) //
					{
						if(max( curJob->dep, port1Job[j - 1]->occuEnd ) + curJob->oper1 <= port1Job[j]->occuStart) //
						{
							//move
							for(int k = port1Count; k > j; k--) //
							{
								port1Job[k] = port1Job[k - 1]; //
							}
							//insert
							curJob->setRealDep(max( curJob->dep, port1Job[j - 1]->occuEnd )); //
							curJob->setOccuStart(curJob->realDep);
							curJob->setOccuEnd(curJob->occuStart + curJob->oper1);
							port1Job[j] = curJob; //
							
							//add waiting time
							p1IWaitingTime += port1Job[j]->getWaitingTime(); //
							
							port1Count++; //
							isFound = true;
							break;
						}
					}
					//tail
					if(!isFound)
					{
						//insert
						curJob->setRealDep(max( curJob->dep, port1Job[port1Count - 1]->occuEnd )); //
						curJob->setOccuStart(curJob->realDep);
						curJob->setOccuEnd(curJob->occuStart + curJob->oper1);
						port1Job[port1Count] = curJob; //
						
						//add waiting time
						p1IWaitingTime += port1Job[port1Count]->getWaitingTime(); //
						
						port1Count++; //
					}
				}
			}
			else if (curJob->type == "O")
			{
				//head: waiting time = 0
				if (curJob->dep + curJob->oper1 + curJob->oper2 <= port1Job[0]->occuStart)
				{
				//move and insert
					for(int j = port1Count; j > 0; j--)  //
						port1Job[j] = port1Job[j - 1];    //
						
					curJob->setRealDep(curJob->dep);
					curJob->setOccuStart(curJob->realDep + curJob->oper1);
					curJob->setOccuEnd(curJob->occuStart + curJob->oper2);
					port1Job[0] = curJob;  //
					port1Count++;	//
				}
				//body
				else
				{
					bool isFound = false;
					//find insertion point
					for(int j = 1; j < port1Count; j++) //
					{
						if(max( curJob->dep + curJob->oper1, port1Job[j - 1]->occuEnd ) + curJob->oper2 <= port1Job[j]->occuStart) //
						{
							//move
							for(int k = port1Count; k > j; k--) //
							{
								port1Job[k] = port1Job[k - 1]; //
							}
							//insert
							curJob->setOccuStart(max( curJob->dep + curJob->oper1, port1Job[j - 1]->occuEnd ));
							curJob->setOccuEnd(curJob->occuStart + curJob->oper2);
							port1Job[j] = curJob; //
							
							//add waiting time
							p1OWaitingTime += port1Job[j]->getWaitingTime(); //
							
							port1Count++; //
							isFound = true;
							break;
						}
					}
					//tail
					if(!isFound)
					{
						//insert
						curJob->setOccuStart(max( curJob->dep + curJob->oper1, port1Job[port1Count - 1]->occuEnd )); //
						curJob->setOccuEnd(curJob->occuStart + curJob->oper2);
						port1Job[port1Count] = curJob; //
						
						//add waiting time
						p1OWaitingTime += port1Job[port1Count]->getWaitingTime(); //
						
						port1Count++; //
					}
				}
			}
		}
		else if(portNum == 2)
		{
			if (curJob->type == "I")
			{
				//head: waiting time = 0
				if (curJob->dep + curJob->oper1 <= port2Job[0]->occuStart) //
				{
				//move and insert
					for(int j = port2Count; j > 0; j--)  //
						port2Job[j] = port2Job[j - 1];    //
						
					curJob->setRealDep(curJob->dep);
					curJob->setOccuStart(curJob->realDep);
					curJob->setOccuEnd(curJob->occuStart + curJob->oper1);
					port2Job[0] = curJob;  //
					port2Count++;	//
				}
				//body
				else
				{
					bool isFound = false;
					//find insertion point
					for(int j = 1; j < port2Count; j++) //
					{
						if(max( curJob->dep, port2Job[j - 1]->occuEnd ) + curJob->oper1 <= port2Job[j]->occuStart) //
						{
							//move
							for(int k = port2Count; k > j; k--) //
							{
								port2Job[k] = port2Job[k - 1]; //
							}
							//insert
							curJob->setRealDep(max( curJob->dep, port2Job[j - 1]->occuEnd )); //
							curJob->setOccuStart(curJob->realDep);
							curJob->setOccuEnd(curJob->occuStart + curJob->oper1);
							port2Job[j] = curJob; //
							
							//add waiting time
							p2IWaitingTime += port2Job[j]->getWaitingTime(); //
							
							port2Count++; //
							isFound = true;
							break;
						}
					}
					//tail
					if(!isFound)
					{
						//insert
						curJob->setRealDep(max( curJob->dep, port2Job[port2Count - 1]->occuEnd )); //
						curJob->setOccuStart(curJob->realDep);
						curJob->setOccuEnd(curJob->occuStart + curJob->oper1);
						port2Job[port2Count] = curJob; //
						
						//add waiting time
						p2IWaitingTime += port2Job[port2Count]->getWaitingTime(); //
						
						port2Count++; //
					}
				}
			}
			else if (curJob->type == "O")
			{
				//head: waiting time = 0
				if (curJob->dep + curJob->oper1 + curJob->oper2 <= port2Job[0]->occuStart) //
				{
				//move and insert
					for(int j = port2Count; j > 0; j--)  //
						port2Job[j] = port2Job[j - 1];    //
						
					curJob->setRealDep(curJob->dep);
					curJob->setOccuStart(curJob->realDep + curJob->oper1);
					curJob->setOccuEnd(curJob->occuStart + curJob->oper2);
					port2Job[0] = curJob;  //
					port2Count++;	//
				}
				//body
				else
				{
					bool isFound = false;
					//find insertion point
					for(int j = 1; j < port2Count; j++) //
					{
						if(max( curJob->dep + curJob->oper1, port2Job[j - 1]->occuEnd ) + curJob->oper2 <= port2Job[j]->occuStart) //
						{
							//move
							for(int k = port2Count; k > j; k--) //
							{
								port2Job[k] = port2Job[k - 1]; //
							}
							//insert
							curJob->setOccuStart(max( curJob->dep + curJob->oper1, port2Job[j - 1]->occuEnd ));
							curJob->setOccuEnd(curJob->occuStart + curJob->oper2);
							port2Job[j] = curJob; //
							
							//add waiting time
							p2OWaitingTime += port2Job[j]->getWaitingTime(); //
							
							port2Count++; //
							isFound = true;
							break;
						}
					}
					//tail
					if(!isFound)
					{
						//insert
						curJob->setOccuStart(max( curJob->dep + curJob->oper1, port2Job[port2Count - 1]->occuEnd )); //
						curJob->setOccuEnd(curJob->occuStart + curJob->oper2);
						port2Job[port2Count] = curJob; //
						
						//add waiting time
						p2OWaitingTime += port2Job[port2Count]->getWaitingTime(); //
						
						port2Count++; //
					}
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
		//	cout << "Accu waiting Time: " << totalWaitingTime << endl << endl;
		}	
	}
	
	cout << p1IWaitingTime << " " << p1OWaitingTime << " " << p2IWaitingTime << " " << p2OWaitingTime;
}
