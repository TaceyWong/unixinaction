/*
第一个版本出现的问题：
	①该怎样消除空白记录
		解决方案：utmp结构中有一个成员ut_type,当他等于7才表示一个已经登陆的用户
			 增加一个判断 if(utbufp->ut_type != USER_PROCESS)
						return 
	
	②怎样正确显示登陆时间
		解决方案：1）unix存储时间的方式：time_t数据类型
				数值是从1970年1月1日0时开始所经过的秒数，time_t = long int
			  2) ctime可以讲表示时间的整数值转换成人们日常所使用的时间形式
*/

#include <stdio.h>
#include <unistd.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>

#define SHOWHOST

void show_time(long timeval)
{
	char *cp;
	cp = ctime(&timeval);
	printf("%12.12s",cp+4);
}

void show_info(struct utmp *utbuf)
{
	if(utbuf->ut_type != USER_PROCESS)
		return ;
	printf("% -8.8s",utbuf->ut_name);
	printf(" ");
	printf("% -8.8s",utbuf->ut_line);
	printf(" ");
	show_time(utbuf->ut_time);
	
	#ifdef SHOWHOST
		if(utbuf->ut_host[0] !='\0')
			printf("(%s)",utbuf->ut_host);
	#endif
	printf("\n");
}


int main()
{
	struct utmp utbuf;
	int utmpfd;
	
	if((utmpfd = open(UTMP_FILE,O_RDONLY)) == -1 )
	{
		perror(UTMP_FILE);
		exit(1);
	}
	while(read(utmpfd,&utbuf,sizeof(utbuf)) == sizeof(utbuf))
		show_info(&utbuf);
	close(utmpfd);
	return 0;

}

