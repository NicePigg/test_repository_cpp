#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
int main(int argc, char *argv[])
{
	struct in_addr addr1,addr2;
	ulong l1,l2;
	l1= inet_addr("192.168.0.74");
	l2 = inet_addr("211.100.21.179");
	memcpy(&addr1, &l1, 4);
	memcpy(&addr2, &l2, 4);
//inet_ntoa返回一个char *,
//这个char *的空间是在inet_ntoa里面静态分配的，
//所以inet_ntoa后面的调用会覆盖上一次的调用。
//第一句printf的结果只能说明在printf里面的可变参数的求值是从右到左的，仅此而已。

	printf("%s : %s\n", inet_ntoa(addr1), inet_ntoa(addr2)); //注意这一句的运行结果
	printf("%s\n", inet_ntoa(addr1));
	printf("%s\n", inet_ntoa(addr2));
	return 0;
}
