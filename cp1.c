/*

	`进程在用户空间，缓冲区时进程内存的一部分
	`进程有两个文件描述负，一个只想源文件，一个指向目标文件
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 4096
#define COPYMODE  0644

int main(int argc,char *argv[])
{
	int in_fd,out_fd,n_chars;
	char buf[BUFFERSIZE];
	
	if(argc != 3)
	{
		fprintf(stderr,"usage:%s source destination\n",*argv)
		exit(1);
	}
	
	if((in_fd = open(av[1],O_RDONLY)) == -1)
		oops("failed to open ",argv[1]);
	if((out_fd = creat(argv[2],COPYMODE)) == -1)
		oops("failed to open",argv[2]);
	while((n_chars = read(in_fd,buf,BUFFERSIZE))>0)
		if(n_chars == -1)
}
