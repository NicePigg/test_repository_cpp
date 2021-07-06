//
// Created by Siiitoka on 2021/6/29.
// ./receiver [Receiver IP address]  [Packet Size(Bytes)] [Delay(us)]
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

#define ALLOWIP_ADDR 		"0.0.0.0"	//接收端使用recvfrom时指定的接收地址
#define PORTNUM					5520		//定义socket的发送或监听端口
#define FILENAME            "recv_record.txt"
unsigned int recvnum;			//接收的报文数目
unsigned long totalrecvlen;		//接收字节数
double losspacknum, losspacknum1;

unsigned int packetsize;		//发送或接收应用层报文大小
unsigned int delaytimeslot;
int counts=0;                   // 期待接收总包数

struct sockaddr_in hostaddr;
struct sockaddr_in addr, clientaddr;
int sock;
void *dataRecvbuff=NULL;					//线程接收应用层报文的缓冲区地址
char endpacket[4] = {0};
int filefd;

void init_recv(){
    memset((void *) &addr, 0, sizeof(addr));
    memset((void *) &clientaddr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ALLOWIP_ADDR);
    addr.sin_port = htons(PORTNUM);

    if((filefd = open(FILENAME, O_RDWR|O_CREAT|O_APPEND, S_IRWXU)) == -1){
        printf("error open file\n");
        return;
    }
    printf("open file success!\n");

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("ERROR!create socket failed!\n");
        exit(1);
    }

    if (bind(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr))==-1) {
        perror("ERROR!");
        exit(1);
    }

    recvnum = 0;
    totalrecvlen = 0;
}

void clean_recv(){
    close(filefd);
    free(dataRecvbuff);
    close(sock);
}

void process_recv()
{
    fd_set fd;
    struct timeval tt;
    tt.tv_sec = 3;   // 超时3s
    tt.tv_usec = 0;
    int ret, len;
    socklen_t length = sizeof(struct sockaddr_in);
    char filestr[20] = {0};
    int k = 0;
    struct timeval tv1, tv2;
    char endflag[4] = "END";
    printf("-----------------------------RECV START-----------------------------\n");
    while (1) {
        FD_ZERO(&fd);
        FD_SET(sock, &fd);
        ret = select(sock + 1, &fd, NULL, NULL, &tt);
        memset(dataRecvbuff, 0, sizeof(packetsize));
        if (-1 == ret) {
            perror("select error!");
            break;
        } else if (0 == ret) {
            printf("No packet has been arrived!\n");
           // break;
            //continue;
        } else {
            if (FD_ISSET(sock, &fd)) {
                len = recvfrom(sock, dataRecvbuff, packetsize, 0, (struct sockaddr *)&(clientaddr), &length);
                if(len <= 0){
                    perror("recv error\n");
                    continue;
                }
                printf("recv succeed! %d\n", len);
                if(len == 4){
                    memcpy(endpacket, dataRecvbuff, 4);
                    printf("endpacket : %s\n", endpacket);
		    if(strcmp(endpacket, endflag) == 0){
			printf("recv end flag packet\n");
			break;
		    }
                   // break;
                }
                else{
                    if(recvnum == 0){
                        gettimeofday(&tv1, NULL);
                    }
                    recvnum++;
                    totalrecvlen += len;
                }

            }
        }
        usleep(delaytimeslot);  //延时delaytimeslot(us)
    }
    printf("run here\n");
    gettimeofday(&tv2, NULL);
    losspacknum = 100*((counts - (double)(totalrecvlen/packetsize)) / counts);
    losspacknum1 = 100*((double)(counts - recvnum) / (double)counts);

    int spendtime = tv2.tv_usec + tv2.tv_sec * 1000000 - tv1.tv_usec - tv1.tv_sec * 1000000;
    double throughput = (double)recvnum*packetsize*8/(double)spendtime;
    printf("received packet = %d\tpacket loss rate = %lf (%lf)\tthroughput = %lf\t\n", recvnum, losspacknum, losspacknum1,  throughput);
    //printf("received packet = %d\tpacket loss rate = %lf\tthroughput = %lf\t\n", recvnum, losspacknum,  throughput);
    //sprintf(filestr, "%d %lf \n", recvnum, losspacknum);

    /*if(write(filefd, filestr, strlen(filestr)) != strlen(filestr)){
        printf("error input\n");
    }*/
    printf("---------------input success--------------\n");
    printf("-----------------------------RECV END-----------------------------\n");
    return;
}

int main(int argc, char *argv[]) {

    init_recv();

    if(1==argc||(2==argc&&(!strcmp(argv[1],"-help"))))
    {
        printf("CMD Format: ./receiver [Receiver IP address]  [Packet Size(Bytes)]  [Delay(us)]  [Hope Total Counts]\n");
        return -1;
    }
    if (5 != argc) {
        printf("Invalid Parameter Number, Please cheack the Input parameter!\n");
        exit(1);
    }
    hostaddr.sin_addr.s_addr=inet_addr(argv[1]);
    packetsize=atoi(argv[2]);
    delaytimeslot = atoi(argv[3]);
    counts = atoi(argv[4]);

    dataRecvbuff= malloc(packetsize);
    memset(dataRecvbuff, 0, packetsize);

    process_recv();

    clean_recv();

    return 0;
}
