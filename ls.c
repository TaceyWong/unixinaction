/*
	ls
		①列出目录内容
		②显示文件信息
	需要：
		①如何列出目录的内容
		②如何读取并显示文件的属性
		3给出一个名字，如何能够判断出他是目录还是文件
`目录是一种特殊的文件

*/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char []);

void main(int argc,char *argv[])
{
	if(argc == 1)
		do_ls(".");
	else 
		while( --argc)
		{
			printf("%s:\n",*++argv);
			do_ls(*argv);
		}
	
}

void do_ls( char dirname[] )
{
	DIR *dir_ptr;
	struct dirent *direntp;
	
	if((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr,"lsl:cannot open %s\n",dirname);
	else
	{
		while ((direntp = readdir(dir_ptr)) != NULL)
			printf("%s\n",direntp->d_name);
		closedir(dir_ptr);
	}
}

/*
*1、排序
	把所有文件名读入一个数组，用qsort函数把数组排序
*2、分栏
	把文件名读入数组，然后ujisuanchu列的宽度和行数
×3、.文件隐藏
	使其接受-a选项，无-a选项的时候不显示隐藏的文件
×4、-l选项
*/
