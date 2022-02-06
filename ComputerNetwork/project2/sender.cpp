#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> //Socket Address
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <signal.h>
using namespace std;

int timeout = 0, senderSocket;


typedef struct {
	int length;
	int seqNumber;
	int ackNumber;
	int fin;
	int syn;
	int ack;
} header;

typedef struct{
	header head;
	char data[1000];
} segment;

void sigalrm_fn(int);
void close_socket(int a)
{
	close(a);
}

int main(int argc, char* argv[])
{
	struct sockaddr_in sender, tmp_addr, agent; //IPv4的socket address structure
	socklen_t sender_size, tmp_size, agent_size;
	segment s_tmp;
	char ip[50] = "127.0.0.1";
	int port_sender = 5557, port_agent = 5558;
    
	/*Create UDP socket*/
	senderSocket = socket(PF_INET, SOCK_DGRAM, 0); 
	
	/*Configure settings in sender struct*/
	sender.sin_family = AF_INET;
	sender.sin_port = htons(port_sender);
	sender.sin_addr.s_addr = inet_addr(ip);
	memset(sender.sin_zero, '\0', sizeof(sender.sin_zero)); 
	
	/*Configure settings in agent struct*/
	agent.sin_family = AF_INET;
	agent.sin_port = htons(port_agent);
	agent.sin_addr.s_addr = inet_addr(ip);
	memset(agent.sin_zero, '\0', sizeof(agent.sin_zero)); 

	/*bind socket*/
	bind(senderSocket,(struct sockaddr *)&sender, sizeof(sender));

	/*Initialize size variable to be used later on*/
	sender_size = sizeof(sender);
	agent_size = sizeof(agent);
	tmp_size = sizeof(tmp_addr);

	/*Read File*/
	int pFile;
	pFile = open(argv[1], O_RDONLY);
	vector<segment> raw_data;
	int count = 1;
	while(1)
	{
		char cont[1000];
		int plen = read(pFile, cont, 1000);
	//	cout << plen << "\n";
		if (plen > 0)
		{
			for(int i = 0; i < 1000; i++)
			{
				s_tmp.data[i] = '\0';
			}
			for(int i = 0; i < plen; i++)
			{
				s_tmp.data[i] = cont[i];
			}
			s_tmp.head.seqNumber = count;
			s_tmp.head.ack = 0;
			raw_data.push_back(s_tmp);
			count += 1;
		}
		else
			break;
	}
	close(pFile);

	/*cout << raw_data.size() << "\n";
	for(int i = 0; i < raw_data.size(); i++)
	{
		cout << raw_data[i].data << "\n";
		cout << "第" << i + 1 << "個封包內容" << "\n";
	}*/
	
	//Transmission
	int w_size = 1, threshold = 16, next = 1, m = 0, segment_size;
	bool done = 0;
	while(done == 0)
	{
		int s_count = 0, r_count = 0;

		if (next + w_size - 1 > raw_data.size())
		{
			for(int i = 0; i < raw_data.size() - next + 1; i++)
			{
				sendto(senderSocket, &raw_data[next + i - 1], sizeof(raw_data[next + i - 1]), 0, (struct sockaddr *)&agent, agent_size);

				if (m < next + i)
				{
					printf("send	data	#%d,	winSize = %d\n", next + i, w_size);
					m = next + i;
				}
				else
					printf("resnd   data    #%d,    winSize = %d\n", next + i, w_size);
			}
			s_count = raw_data.size() - next + 1;
		}

		else
		{	
			for(int i = 0; i < w_size; i++)
			{
				sendto(senderSocket, &raw_data[next + i - 1], sizeof(raw_data[next + i - 1]), 0, (struct sockaddr *)&agent, agent_size);
				s_count += 1;
				if (m < next + i)
				{
					printf("send	data	#%d, 	winSize = %d\n", next + i, w_size);
					m = next + i;
				}
				else
					printf("resnd	data	#%d, 	winSize = %d\n", next + i, w_size);
			}
			s_count = w_size;
		}

		while(done == 0)
		{
			/*Timer activated*/
			signal(SIGALRM, sigalrm_fn);
			alarm(1);
			memset(&s_tmp, 0, sizeof(s_tmp));
			segment_size = recvfrom(senderSocket, &s_tmp, sizeof(s_tmp), 0, (struct sockaddr *)&tmp_addr, &tmp_size);
			/*If time out, retransmit*/
			if (timeout)
			{
				timeout = 0;
				senderSocket = socket(PF_INET, SOCK_DGRAM, 0); 
				bind(senderSocket,(struct sockaddr *)&sender, sizeof(sender));
				threshold = max(1, w_size / 2);
				w_size = 1;
				printf("time    out             threshold = %d\n", threshold);
				break;
			}
				printf("recv    ack     #%d\n", s_tmp.head.ackNumber);
			/*收到對的ack*/
			if (s_tmp.head.ackNumber == next)
			{
				alarm(0);
				r_count += 1;
				next += 1;
			/*	cout << "r_count = " << r_count << "\n";
				cout << "s_count = " << s_count << "\n";
			*/
					/*若檔案傳完了*/
				if (next == raw_data.size() + 1)
				{
					segment F;
					F.head.fin = 1;
					sendto(senderSocket, &F, sizeof(F), 0, (struct sockaddr *)&agent, agent_size);
					printf("send    fin\n");
						recvfrom(senderSocket, &s_tmp, sizeof(s_tmp), 0, (struct sockaddr *)&tmp_addr, &tmp_size);
					if (s_tmp.head.fin == 1)
					{
						printf("recv    finack\n");
						done = 1;
						break;
					}
				}
	
				//若檔案還沒傳完
				else
				{	
					//此批封包順利ack完了，傳下一批
					if(r_count == s_count)
					{
						if(w_size < threshold)
							w_size = w_size * 2;
						else
							w_size += 1;
						break;
					}
					/*還沒ack完，繼續收*/
					else
						continue;
				}
			}
			/*收到不對的ack，繼續收*/
			else
				continue;
		}
	}
}

void sigalrm_fn(int)
{
	timeout = 1;
	close_socket(senderSocket);
	return;
}
