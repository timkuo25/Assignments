/* NOTE: 本程式僅供檢驗格式，並未檢查 congestion control 與 go back N */
/* NOTE: UDP socket is connectionless，每次傳送訊息都要指定ip與埠口 */
/* HINT: 建議使用 sys/socket.h 中的 bind, sendto, recvfrom */

/*
 * 連線規範：
 * 本次作業之 agent, sender, receiver 都會綁定(bind)一個 UDP socket 於各自的埠口，用來接收訊息。
 * agent接收訊息時，以發信的位址與埠口來辨認訊息來自sender還是receiver，同學們則無需作此判斷，因為所有訊息必定來自agent。
 * 發送訊息時，sender & receiver 必需預先知道 agent 的位址與埠口，然後用先前綁定之 socket 傳訊給 agent 即可。
 * (如 agent.c 第126行)
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>

#define DEFAULT_THREASHOLD 16
#define TIME_ONE_MORE_RECV 0.1
#define TIME_WAIT_RECV 0.1
#define TIME_BETWEEN_SEGMENT 0.1
#define TIME_ACK_TIMEOUT 0.8

#define MAX_WINDOW 5000
#define ALARM(sec) ualarm(sec*1000*1000, 0)
#define MAX(a, b) a > b ? a : b

int timedout = 0;

void handler(int s) {
    timedout = 1;
}

typedef struct {
	int length;
	int seqNumber;
	int ackNumber;
	int fin;
	int syn;
	int ack;
} header;

typedef struct {
	header head;
	char data[1000];
} segment;

void getSelfIP(char *ip) {
    struct ifaddrs *ifaddr, *ifa;
    int s;
    if(getifaddrs(&ifaddr) == -1) {
        fprintf(stderr, "抓不到自己的ip QQ\n");
        exit(1);
    }
    for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if(ifa->ifa_addr != NULL) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), ip,
                    NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if(strcmp(ifa->ifa_name, "enp3s0") == 0
                    && ifa->ifa_addr->sa_family == AF_INET) {
                if(s != 0) {
                    fprintf(stderr, "抓不到自己的ip QQ\n");
                    exit(1);
                } else {
                    return;
                }
            }
        }
    }
    fprintf(stderr, "抓不到自己的ip QQ\n");
    exit(1);
}

void setIP(char *dst, char *src) {
    if(strcmp(src, "0.0.0.0") == 0 || strcmp(src, "local") == 0
            || strcmp(src, "localhost") == 0) {
        //getSelfIP(dst);
        sscanf("127.0.0.1", "%s", dst);
    } else {
        sscanf(src, "%s", dst);
    }
}

void sendBufferedSegments(int agentsocket, segment *buffer, int buff_len, struct sockaddr_in receiver, float loss_rate) {
    int i;
    for(i = 0; i < buff_len; i++) {
        if (rand() % 100 < 100 * loss_rate) {
            printf("drop data #%d\n", buffer[i].head.seqNumber);
        } else {
            sendto(agentsocket, &buffer[i], sizeof(buffer[i]), 0,
                   (struct sockaddr *)&receiver, sizeof(receiver));
            printf("fwd	data	#%d\n", buffer[i].head.seqNumber);
        }
    }
}

int getFirstNoneAcked(int *mem, int win_size, int win_start_idx) {
    int i;
    for(i = 0; i < win_size; i++) {
        if(mem[i + win_start_idx] == 0) {
            return i + win_start_idx;
        }
    }
    return -1;
}

long getCurTime() {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    return spec.tv_nsec / 1.0e6 + spec.tv_sec * 1000;
}

int main(int argc, char* argv[]){
    int agentsocket, portNum, nBytes;
    float loss_rate;
    segment s_tmp;
    struct sockaddr_in sender, agent, receiver, tmp_addr;
    socklen_t sender_size, recv_size, tmp_size;
    char ip[3][50];
    int port[3], i;

    /* FOR Judging */
    int win_size = 1;
    int threshold = DEFAULT_THREASHOLD;
    segment segment_buffer[MAX_WINDOW];
    int win_offset = 0;
    int mem_ack[MAX_WINDOW];
    int win_start_idx = 1;
    int first_none_acked = -1;
    int first = 1;
    long time_buffer_sent = -1;
    int expect_fin = 0, fin_received = 0;
    memset(mem_ack, 0, sizeof(mem_ack));
    /**************/

    if(argc != 7){
        fprintf(stderr,"用法: %s <sender IP> <recv IP> <sender port> <agent port> <recv port> <loss_rate>\n", argv[0]);
        fprintf(stderr, "例如: ./agent local local 8887 8888 8889 0.3\n");
        exit(1);
    } else {
        setIP(ip[0], argv[1]);
        setIP(ip[1], "local");
        setIP(ip[2], argv[2]);

        sscanf(argv[3], "%d", &port[0]);
        sscanf(argv[4], "%d", &port[1]);
        sscanf(argv[5], "%d", &port[2]);

        sscanf(argv[6], "%f", &loss_rate);
    }

    /*Create UDP socket*/
    agentsocket = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in sender struct*/
    sender.sin_family = AF_INET;
    sender.sin_port = htons(port[0]);
    sender.sin_addr.s_addr = inet_addr(ip[0]);
    memset(sender.sin_zero, '\0', sizeof(sender.sin_zero));  

    /*Configure settings in agent struct*/
    agent.sin_family = AF_INET;
    agent.sin_port = htons(port[1]);
    agent.sin_addr.s_addr = inet_addr(ip[1]);
    memset(agent.sin_zero, '\0', sizeof(agent.sin_zero));    

    /*bind socket*/
    bind(agentsocket,(struct sockaddr *)&agent,sizeof(agent));

    /*Configure settings in receiver struct*/
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(port[2]);
    receiver.sin_addr.s_addr = inet_addr(ip[2]);
    memset(receiver.sin_zero, '\0', sizeof(receiver.sin_zero)); 

    /*Initialize size variable to be used later on*/
    sender_size = sizeof(sender);
    recv_size = sizeof(receiver);
    tmp_size = sizeof(tmp_addr);

    printf("可以開始測囉^Q^\n");
    printf("sender info: ip = %s port = %d and receiver info: ip = %s port = %d\n",ip[0], port[0], ip[2], port[2]);
    printf("agent info: ip = %s port = %d\n", ip[1], port[1]);

    int total_data = 0;
    int drop_data = 0;
    int segment_size, index;
    char ipfrom[1000];
    char *ptr;
    int portfrom;
    srand(time(NULL));
    struct sigaction int_handler = {.sa_handler=handler};
    sigaction(SIGALRM, &int_handler, 0);
    while(1){
        /*Receive message from receiver and sender*/
        memset(&s_tmp, 0, sizeof(s_tmp));
        ALARM(TIME_WAIT_RECV);
        segment_size = recvfrom(agentsocket, &s_tmp, sizeof(s_tmp), 0, (struct sockaddr *)&tmp_addr, &tmp_size);

        if(timedout) {
            timedout = 0;
            // NOTE: 檢查window傳完了沒
            if(!first && win_offset < win_size) {
                if(fin_received) {
                    continue;
                }
                if (win_offset > 0 && getCurTime() - time_buffer_sent >= (long)(TIME_BETWEEN_SEGMENT * 1000)) {
                    // NOTE: 假設已經讀到標案尾了，先送一波
                    printf("好像到結尾囉~開始傳送 #%d ~ #%d\n", win_start_idx, win_start_idx + win_offset - 1);
                    sendBufferedSegments(agentsocket, segment_buffer, win_offset, receiver, loss_rate);
                    time_buffer_sent = getCurTime();
                    expect_fin = 1;
                }
            } else if(!first) {
                if(getCurTime() - time_buffer_sent >= (long)(TIME_ACK_TIMEOUT * 1000)) {
                    // NOTE: ack timeout!!
                    first_none_acked = getFirstNoneAcked(mem_ack, win_size, win_start_idx);
                    printf("#%d ack timeout\n", first_none_acked);
                    threshold = MAX(1, win_size / 2);
                    win_offset = 0;
                    win_start_idx = first_none_acked;
                    win_size = 1;
                }
            }
            continue;
        }

        if (segment_size > 0)
        {
            portfrom = ntohs(tmp_addr.sin_port);
            inet_ntop(AF_INET, &tmp_addr.sin_addr.s_addr, ipfrom, sizeof(ipfrom));

            if (strcmp(ipfrom, ip[0]) == 0 && portfrom == port[0])
            {
                first = 0;
                /*segment from sender, not ack*/
                if (s_tmp.head.ack)
                {
                    fprintf(stderr, "收到來自 sender 的 ack segment\n");
                }
                if(win_size == win_offset) {
                    fprintf(stderr, "ERROR: 還在等timeout就傳東西過來\n");
                    exit(1);
                }
                total_data++;
                segment_buffer[win_offset++] = s_tmp;
                if (s_tmp.head.fin == 1)
                {
                    printf("get     fin\n");
                    sendto(agentsocket, &s_tmp, segment_size, 0, (struct sockaddr *)&receiver, recv_size);
                    printf("fwd     fin\n");
                    fin_received = 1;
                }
                else
                {
                    if(expect_fin) {
                        fprintf(stderr, "ERROR: window size 太小！\n");
                        exit(1);
                    }
                    index = s_tmp.head.seqNumber;
                    if(index != win_offset + win_start_idx - 1) {
                        fprintf(stderr, "ERROR: 順序有誤！預計收到#%d，卻收到#%d\n", win_start_idx+win_offset-1, index);
                        exit(1);
                    }
		    printf("get	data	#%d\n", index);
                    printf("win_offset = %d, win_size = %d, threshold = %d\n", win_offset, win_size, threshold);
                    if (win_offset == win_size) {
                        // NOTE: 試著再收個封包看看
                        ALARM(TIME_ONE_MORE_RECV);
                        segment_size = recvfrom(agentsocket, &s_tmp, sizeof(s_tmp), 0, (struct sockaddr *)&tmp_addr, &tmp_size);
                        if (segment_size > 0) {
                            // 有資料多送了
                            fprintf(stderr, "ERROR: window size 太大！");
                            exit(1);
                        }
                        timedout = 0;
                        printf("開始傳送 #%d ~ #%d\n", win_start_idx, win_start_idx + win_offset - 1);
                        sendBufferedSegments(agentsocket, segment_buffer, win_offset, receiver, loss_rate);
                        time_buffer_sent = getCurTime();
                    }
                }
            }
            else if (strcmp(ipfrom, ip[2]) == 0 && portfrom == port[2])
            {
                /*segment from receiver, ack*/
                if (s_tmp.head.ack == 0)
                {
                    fprintf(stderr, "收到來自 receiver 的 non-ack segment\n");
                }
                if (s_tmp.head.fin == 1)
                {
                    printf("get     finack\n");
                    sendto(agentsocket, &s_tmp, segment_size, 0, (struct sockaddr *)&sender, sender_size);
                    printf("fwd     finack\n");
                    break;
                }
                else
                {
                    index = s_tmp.head.ackNumber;
                    mem_ack[index] = 1;
                    printf("get     ack	#%d\n", index);
                    sendto(agentsocket, &s_tmp, segment_size, 0, (struct sockaddr *)&sender, sender_size);
                    printf("fwd     ack	#%d\n", index);

                    if(win_offset == win_size) {
                        first_none_acked = getFirstNoneAcked(mem_ack, win_size, win_start_idx);
                        if(first_none_acked == -1) {
                            printf("收到 #%d ~ #%d ack\n", win_start_idx, win_start_idx + win_size - 1);
                            // NOTE: 增加window size
                            win_offset = 0;
                            win_start_idx += win_size;
                            if(win_size < threshold) {
                                win_size *= 2;
                            } else {
                                win_size++;
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
