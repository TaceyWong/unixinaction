/*
	与目录树相关的命令及系统调用
	1）mkdir
		目标：创建目录
		头文件： sys/stat.h
			 sys/types.h
		函数原型：int result = mkdir(char *pathname,mode_t mode)
		参数： pathname 新目录名
		       mode 权限位的掩码
		返回值： -1遇到错误
			0 成功创建
	2）rmdir
		目标：删除一个目录，此目录必须为空
		头文件： unistd.h
		函数原型： int result = rmdir(const char *path)
		参数： path 目录名
		返回值： -1 遇到错误
			0 成功删除
	3）rm
		目标：删除一个链接，从目录文件删除一个记录，使用unlink系统调用
		头文件：unistd.h
		函数原型：int result = unlink(const char *path)
		参数： path 需删除的链接名
		返回值： -1 遇到错误
			0 成功删除

	4）ln
		目标：创建一个文件的链接
		头文件： unistd.h
		函数原型： int result = link(const char *orig,const char *new)
		参数：orig 原始链接的名字
			new 新链接的名字
		返回值

		note：link不能用来生成目录的新连接
	5）mv
		目标 重命名或删除一个链接
		头文件： unistd.h
		函数原型： int result = rename(const char *from ,const char *to)
		参数： 原始链接的名字
		       新建链接的名字
		返回值

	6）cd
		只改变进程，不改变目录
		目标：改变所调用进程的当前目录
		头文件：unistd.h
		函数原型：int result = chdir(const char *path)
		参数：path 要到达的目录
		返回值

*/
//This is just a simple-pwd

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#define BUFSIZE 1024

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t,char *,int);

int main()
{
	printpathto(get_inode("."));
	putchar('\n');
	return 0;
}

void printpathto(ino_t this_inode)
{
	ino_t my_inode;
	char its_name[BUFSIZE];
	if(get_inode("..")!= this_inode)
	{
		chdir("..");
		inum_to_name(this_inode,its_name,BUFSIZE);
		my_inode = get_inode(".");
		printpathto(my_inode);
		printf("/%s",its_name);
	}
}

void inum_to_name(ino_t inode_to_find,char *namebuf,int buflen)
{
	DIR *dir_ptr;
	struct dirent *direntp;
	dir_ptr = opendir(".");
	if(dir_ptr == NULL)
	{
		perror(".");
		exit(1);
	}
	
	while (( direntp = readdir(dir_ptr)  ) != NULL)
		if(direntp->d_ino == inode_to_find)
		{
			strncpy(namebuf,direntp->d_name,buflen);
			namebuf[buflen-1] = '\0';
			closedir(dir_ptr);
			return ;
		}	
	fprintf(stderr,"error looking for inum%d\n",inode_to_find);
	exit(1);
}

ino_t get_inode( char *fname  )
{
	struct stat info;
	if( stat(fname , &info) == -1)
	{
		fprintf(stderr,"can not stat");
		perror(fname);
		exit(1);
	}
	return info.st_ino;	
}



