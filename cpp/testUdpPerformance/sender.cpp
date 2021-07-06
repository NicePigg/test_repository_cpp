//
// Created by Siiitoka on 2021/6/29.
// ./sender [Local IP address] [Receiver IP address]  [Interval(us)] [Packet Size(Bytes)] [Total Counts]
//
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <malloc.h>
#include <memory.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define PORTNUM					5520		//定义socket的发送或监听端口
#define ALLOWIP_ADDR 		"0.0.0.0"	    //接收端使用recvfrom时指定的接收地址
#define PRINTINTERVAL			3 			//打印线程打印信息的间隔，单位seconds

pthread_t thread_send,thread_show;    // 发送线程 +　显示线程

unsigned int count = 1;
struct sockaddr_in remoteaddr;
struct sockaddr_in hostaddr;
int sock;
void *dataSendbuff = NULL;					//线程发送应用层报文的缓存区地址


struct sockaddr_in hostaddr;	//节点自身的地址
unsigned int packetsize;		//发送或接收应用层报文大小

//unsigned int sendnum;			//发送的报文数目
//unsigned int misspktnum;		//关于丢包的统计变量
pthread_mutex_t  mutex;		    //发送缓存报文数目进行加锁同步


struct my_time {
    uint32_t secs;
    uint32_t usecs;
};

void initpthreadmutex(void);
void destorypthreadmutex(void);



// 发送参数
int counts=0; // 发送总包数
unsigned int sendinterval;		//发送应用层报文间隔(usleep和for延迟的实现)
int sendnum=0;  // 目前的发送个数
char endPacket[4] = "END";

int process_send(){
    printf("-----------------------------SEND START-----------------------------\n");
    int k = counts;
    struct my_time *tstart, *tend;
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    while(k--){
        if (-1 == sendto(sock, dataSendbuff, packetsize, 0, (struct sockaddr*)&(remoteaddr), sizeof(remoteaddr))) {
            //if (-1 ==sendto(sock, sendbuf, recordvar.packetsize, 0,(struct sockaddr *)&remoteaddr,sizeof(remoteaddr))) {
            perror("ERROR! send error!");
            exit(1);
        }
        	printf("send succeed! \n");
        sendnum++;
        usleep(sendinterval);
    }
    gettimeofday(&tv2, NULL);
    int diff = tv2.tv_usec + tv2.tv_sec * 1000000 - tv1.tv_usec - tv1.tv_sec * 1000000;
    printf("COUNTS = %d   RealSendNum = %d   SpendTime = %d(us)  THROUGHPUT = %lf(Mbps)\n", counts, sendnum, diff, (double)sendnum*packetsize*8/(double)diff);
    printf("-----------------------------SEND END-----------------------------\n");
    sendto(sock, endPacket, 4, 0, (struct sockaddr*)&(remoteaddr), sizeof(remoteaddr));
    printf("make server ending...\n");
    return 0;
}


int main(int argc, char *argv[]) {
    memset((void *) &remoteaddr, 0, sizeof(remoteaddr));
    memset((void *) &hostaddr, 0, sizeof(remoteaddr));
    remoteaddr.sin_family = AF_INET;

    if(1==argc||(2==argc&&strcmp(argv[1], "-help")))
    {
        printf("CMD Format: ./sender [Local IP address] [Receiver IP address]  [Interval(us)] [Packet Size(Bytes)] [Total Counts]\n");
        return -1;
    }
    if(6==argc)
    {
        counts=atoi(argv[5]);
    }

    if (5>argc) {
        printf("Invalid Parameter Number, Please cheack the Input parameter! ,argc = %d\n",argc);
        printf("CMD Format: ./sender [Local IP address] [Receiver IP address]  [Interval(us)] [Packet Size(Bytes)] [Total Counts]\n");
        return -1;
    }

    if (INADDR_NONE == (remoteaddr.sin_addr.s_addr = inet_addr(argv[2]))) {  //判断IP地址是否正确
        printf("Receiver IP address is invalid!");
        exit(1);
    } else {
        printf("Receiver IP address : %s\n", argv[2]);
    }
    printf("Send Interval : %sus   Single Packet Size : %s   Bytes couns : %s\n",argv[3], argv[4],argv[5]);
    remoteaddr.sin_port = htons(PORTNUM);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {  // UDP Socket
        printf("ERROR!create socket failed!\n");
        exit(1);
    }

    hostaddr.sin_addr.s_addr=inet_addr(argv[1]);  // 本地IP
    sendinterval=atoi(argv[3]);                   // 发送延时
    packetsize=atoi(argv[4]);                     // 发包大小
    sendnum=0;                                    // 发送个数

    dataSendbuff = (char *)malloc(packetsize*sizeof(char));
    memset(dataSendbuff, '1',packetsize);
    process_send();

    free(dataSendbuff);
    close(sock);
    return 0;
}


void initpthreadmutex(void)
{
    pthread_mutex_init(&mutex, NULL);
    return;
}

void destorypthreadmutex(void)
{
    pthread_mutex_destroy(&mutex);
    return;
}
