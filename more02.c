/*
 /dev/tty文件是键盘和显示器的设备描述文件，
向这个文件写相当于显示在用户的屏幕上，
读相当于从键盘获取用户的输入。
即使程序的输入输出被重定向，程序还是可以通过这个文件与终端交换数据。







*/



#include <stdio.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more(FILE *);

int main(int argc,char *argv[])
{
	FILE *fp;
	if( argc == 1)
		do_more(stdin);
	else
		while( --argc)
			if((fp = fopen(*++argv,"r")) !=NULL)
			{
				do_more(fp);
				fclose(fp);
			}
			else
				exit(1);
}

void do_more(FILE *fp)
/*
  读取PAGELEN行的文本，接着调用see_more()函数进行下一步的指令
*/
{
	char line[LINELEN];
	int num_of_lines = 0;
	int see_more(FILE *),reply;
	FILE *fp_tty;
	fp_tty = fopen("/dev/tty","r"); //NEW：cmd流
	if(fp_tty == NULL)		//如果打开失败
		exit(1);
	while(fgets(line,LINELEN,fp))	//more输出
	{
		if(num_of_lines == PAGELEN)
		{
			reply = see_more(fp_tty); //NEW:传递FILE*
			if(reply == 0)		 //完成
				break;
			num_of_lines -= reply;		//重新计数
		}
		if(fputs(line,stdout) == EOF)           //显示一行
			exit(1);			//或退程序
		num_of_lines++;				//计数
	}
}

int see_more(FILE *cmd)
/*
	打印信息，等待回应，返回要显示的行数
	q表示否定（退出），空格表示肯定，回车表示一行	
*/
{
	int c;
	printf("\033[7m more? \033[m");
	while((c = getc(cmd))!= EOF)
	{
		if(c == 'q')
			return 0 ;
		if(c == ' ')         // 下一页
			return PAGELEN; //要显示的行数
		if(c == '\n')
			return 1;
	}
	return 0;
}
