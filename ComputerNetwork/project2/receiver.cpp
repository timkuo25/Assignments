#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h> //Socket Address
#include <string.h>
#include <iostream>
using namespace std;

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

int main(int argc, char* argv[])
{
	int receiverSocket;
	struct sockaddr_in receiver, tmp_addr, agent; //IPv4的socket address structure
	socklen_t receiver_size, agent_size, tmp_size;
	segment s_tmp;
	char ip[50] = "127.0.0.1";
	int port_receiver = 5559, port_agent = 5558;

	/*Create UDP socket*/
	receiverSocket = socket(PF_INET, SOCK_DGRAM, 0); 
	
	/*Configure settings in receiver struct*/
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(port_receiver);
	receiver.sin_addr.s_addr = inet_addr(ip);
	memset(receiver.sin_zero, '\0', sizeof(receiver.sin_zero));
	
	agent.sin_family = AF_INET;
	agent.sin_port = htons(port_agent);
	agent.sin_addr.s_addr = inet_addr(ip);
	memset(agent.sin_zero, '\0', sizeof(agent.sin_zero)); 

	/*bind socket*/
	bind(receiverSocket,(struct sockaddr *)&receiver, sizeof(receiver));

	/*Initialize size variable to be used later on*/
	receiver_size = sizeof(receiver);
	agent_size = sizeof(agent);
	tmp_size = sizeof(tmp_addr);
	
	int segment_size, next = 1, buffptr = 0;
	int b = 0;
	char buff[32][1000];
	segment A;
	int pFile;
	pFile = open(argv[1], O_WRONLY);

	while(1)
	{
		memset(&s_tmp, 0, sizeof(s_tmp));
		segment_size = recvfrom(receiverSocket, &s_tmp, sizeof(s_tmp), 0, (struct sockaddr *)&tmp_addr, &tmp_size);
		printf("recv	data	#%d\n", s_tmp.head.seqNumber);

		if (s_tmp.head.fin)
		{
			printf("recv    fin\n");
			segment F;
			F.head.ack = 1;
			F.head.fin = 1;
			sendto(receiverSocket, &F, sizeof(F), 0, (struct sockaddr *)&agent, agent_size);
			printf("send    finack\n");
			/*flush*/
			for (int i = 0; i < buffptr; i++)
			{
				write(pFile, buff[i], 1000);
			}
			printf("flush\n");
			break;
		}

		/*cout << "hi\n" << s_tmp.head.seqNumber << "\n" << next << "\n";*/

		if (buffptr == 32)
		{
			//drop
			A.head.ack = 1;
			A.head.ackNumber = next - 1;
			sendto(receiverSocket, &A, sizeof(A), 0, (struct sockaddr *)&agent, agent_size);
			printf("drop    data    #%d\n", s_tmp.head.seqNumber);
			printf("send    ack     #%d\n", next - 1);
			//flush
			for (int i = 0; i < 32; i++)
			{
				write(pFile, buff[i], 1000);
			}
			printf("flush\n");
			buffptr = 0;
		}
		else
		{	
			if (s_tmp.head.seqNumber == next)
			{
				A.head.ack = 1;
				A.head.ackNumber = next;
				sendto(receiverSocket, &A, sizeof(A), 0, (struct sockaddr *)&agent, agent_size);
				printf("send    ack     #%d\n", next);
				next += 1;

				for(int i = 0; i < 1000; i++)
				{
					buff[buffptr][i] = s_tmp.data[i];
				}

				buffptr += 1;
			}
			else
			{
				A.head.ack = 1;
				A.head.ackNumber = next - 1;
				sendto(receiverSocket, &A, sizeof(A), 0, (struct sockaddr *)&agent, agent_size);
				printf("drop    data    #%d\n", s_tmp.head.seqNumber);
				printf("send    ack     #%d\n", next - 1);
			}
		}
	}
	return 0;
}
