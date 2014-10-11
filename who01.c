/*
	①从文件中读取数据结构（使用open、read、close）
		1）open。该系统调用在进程和文件之间简历一条连接，这个链接被称为文件描述符
			头文件：fcntl.h
			函数原型：int fd = open(char *name,int how)
					name 文件名
					how  O_RDONLY,O_WRONLY,O_RDWR
			返回值：-1 遇到问题
				int 文件描述符，唯一标识这个链接（必须通过文件描述符号对文件进行操作）
		2）read。用于读取数据
			头文件： unistd.h
			函数原型： ssize_t numread = read(int fd,void *buf,size_t qty)
					fd 文件描述符
					buf 用来存放数据的目的缓冲区
					qty 要读取的字节数
			返回值:-1 遇到问题
				numread 成功读取
		3）close。关闭文件
			头文件： unistd.h
			函数原型： int result = close(int fd)
			
	②将结构中的信息以合适的形式显示出来
*/
#include <stdio.h>
#include <utmp.h> //保存着登陆信息结构信息
#include <fcntl.h>
#include <unistd.h> //系统调用头文件

#define SHOWHOST  //输出包含远程机器


void show_info(struct utmp *utbufp)
{
	printf("% -8.8s ",utbufp->ut_name);
	printf(" ");
	printf("% -8.8s ",utbufp->ut_line);
	printf("% 10ld ",utbufp->ut_time);
	printf(" ");
	
	#ifdef SHOWHOST
		printf("%s",utbufp->ut_host);
	#endif
		printf("\n");
}









int main()
{
	struct utmp current_record; //用于保存读取的信息
	int         utmpfd ;         //
	int         reclen = sizeof(current_record);
	
	if (( utmpfd = open(UTMP_FILE,O_RDONLY) ) == -1)
	{
		perror(UTMP_FILE);            //UTMP_FILE在utmp.h中
		exit(1);
	}

	while( read ( utmpfd,&current_record,reclen ) == reclen )
		show_info(&current_record);
	close(utmpfd);
	return 0;
}
