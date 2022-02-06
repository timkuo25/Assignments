/*
DSAP 2019 Spring HW6-3, Multilevel-Feedback Queue Scheduling 
ver 1.0
Author: tim_ezemnng
Last Modified: 5/19/2019
 
Note:
*/
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;
 
int total = 0;
bool sig = false;
bool first = true;
 
class Process
{
    private:
        string name;
        int start;
        int oper;
        int rem;
        int lv;
    public:
        Process(string n, int s, int o): name(n), start(s), oper(o), rem(o), lv(0){;}
         
        //getters
        string getName(){return name;}
        int getStart(){return start;}
        int getOper(){return oper;}
        int getRem(){return rem;}
        int setRem(int r){rem = r;}
        int getLV(){return lv;}
        void exe1()
        {
            rem--;
            total--;
        }
};
 
int main()
{
    int n; //number of queues
    cin >> n;
    vector<int> v_quan;
    vector< queue<Process> > v_queue;
    queue<Process> done;
     
    Process** rec = new Process*[500];
    int count = 0;
//  int done_count = 0;
    int now = 0;
    int quan = 0;
    int curLV = 0;
     
    for(int i = 0; i < n - 1; i++)
    {
         
        int quantum;
        cin >> quantum;
         
        v_quan.push_back(quantum);
        queue<Process> q;
        v_queue.push_back(q);
    }
    v_quan.push_back(99999);
    queue<Process> q;
    v_queue.push_back(q);
 
    string command;
    while(getline(cin, command))
    {
        int space_count = 0;
        for(int i = 0; i < command.size(); i++)
        {
            if (command[i] == ' ') space_count++;
        }
         
        if(space_count == 2) //execute a process 
        {
            string name;
            string START;
            string OPER;
            int start;
            int oper;
             
            istringstream ss(command);
            getline(ss, START, ' ');
            getline(ss, name, ' ');
            getline(ss, OPER);
            start = stoi(START);
            oper = stoi(OPER);
             
            while(now != start)
            {
                    if(v_queue[curLV].empty() && total == 0)
                    {
                        now++;
                        continue;
                    }
                     
                    if(v_queue[curLV].empty())
                    {
                        for(int i = 0; i < n; i++)
                        {
                            if(!v_queue[i].empty())
                            {
                                curLV = i;
                                break;
                            }
                        }
                    }
                    v_queue[curLV].front().exe1();
                    quan++;
                    now++;
                    if(v_queue[curLV].front().getRem() == 0)
                    {
                    	if(curLV != 0 and now == start) sig = true;
                        quan = 0;
                        done.push(v_queue[curLV].front());
                        v_queue[curLV].pop();
                         
                        for(int i = 0; i < count; i++)
                        {
                            if(rec[i]->getName() == done.back().getName())
                            {
                            //  cout << "i = " << i << "\n" ; //
                                rec[i] = &(done.back());
                                break;
                            }
                        }
                    }
                    if(quan == v_quan[curLV])
                    {
                    	if(curLV != 0 and now == start) sig = true;
                        quan = 0;
                        v_queue[curLV + 1].push(v_queue[curLV].front());
                        v_queue[curLV].pop();
                        for(int i = 0; i < count; i++)
                        {
                            if(rec[i]->getName() == v_queue[curLV + 1].back().getName())
                            {
                                rec[i] = &(v_queue[curLV + 1].back());
                                break;
                            }
                        }
                    }
            }
            Process p(name, start, oper);
            v_queue[0].push(p);
            rec[count] = &(v_queue[0].back());
            total += p.getOper();           
            count++;
             
            if(curLV != 0)
            {
                quan = 0;
                if(sig)
                {
                    curLV = 0;
                    sig = false;
                }
                else
                {
	                v_queue[curLV].push(v_queue[curLV].front());
	                for(int i = 0; i < count; i++)
	                {
	                    if(rec[i]->getName() == v_queue[curLV].back().getName())
	                    {
	                        rec[i] = &(v_queue[curLV].back());
	                        break;
	                    }
	                }
	                v_queue[curLV].pop();
	                curLV = 0;
				}
            }
             
        //  cout << "now = " << now << ",quan = " << quan << ",curLV = " << curLV << ", rec[0]rem = " << rec[0]->getRem() << "\n";
        }
        else if(space_count == 1) //print process
        {
            string TIME;
            int time;
            istringstream ss(command);
            getline(ss, TIME, ' ');
            time = stoi(TIME);  
             
            while(now != time)
            {
                    if(v_queue[curLV].empty() && total == 0)
                    {
                        now++;
                        continue;
                    }
                     
                    if(v_queue[curLV].empty())
                    {
                        for(int i = 0; i < n; i++)
                        {
                            if(!v_queue[i].empty()) 
                            {
                                curLV = i;
                                break;
                            }
                        }
                    }
                    v_queue[curLV].front().exe1();
                    quan++;
                    now++;
                    if(v_queue[curLV].front().getRem() == 0)
                    {
                        quan = 0;
                        done.push(v_queue[curLV].front());
                        v_queue[curLV].pop();
                         
                        for(int i = 0; i < count; i++)
                        {
                            if(rec[i]->getName() == done.back().getName())
                            {
                            //  cout << "i = " << i << "\n" ;
                                rec[i] = &(done.back());
                                break;
                            }
                        }
                    }
                    if(quan == v_quan[curLV])
                    {
                        quan = 0;
                         
                        v_queue[curLV + 1].push(v_queue[curLV].front());
            //          cout << "hi, curLV = " << curLV << ", v_queue[curLV + 1] = " << v_queue[curLV + 1].back().getName() << "\n";
                        v_queue[curLV].pop();
                        for(int i = 0; i < count; i++)
                        {
                            if(rec[i]->getName() == v_queue[curLV + 1].back().getName())
                            {
                                rec[i] = &(v_queue[curLV + 1].back());
                                break;
                            }
                        }
                    }
            }
        //  cout << "now = " << now << ",quan = " << quan << ",curLV = " << curLV << "\n";
             
            for(int i = 0; i < count; i++)
            {
                if(!first) cout << "\n";
                cout << rec[i]->getName() << " " << rec[i]->getRem();
                first = false;
            }
             
            continue;
        }
    }
    return 0;
}
