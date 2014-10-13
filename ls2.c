/*
	ls -l列出的信息每行包含7个字段
	模式 mode 代表文件类型 -文件、d目录文件......
	链接数 links 被引用次数
	文件所有者 owner 文件所有者的用户名
	组 group 文件所在组
	大小 size
	最后修改时间 last-modified
	文件名 name 
	
	要获得文件信息，调用stst系统调用
		目标： 得到文件的属性
		头文件： sys/stat.h
		函数原型： int result = stat(char * fname,struct stat *bufp)
		参数： fname 文件名
		       bufp  指向buffer的指针
		返回值： -1 遇到错误
			0 成功返回
	stat获得文件大小的示例
	#include <stdio.h>
	#include <sys/stat.h>
	int main()
	{
		struct stat infobuf;
		if(stat("/etc/passwd",&infobuf) == -1)
			perro("/etc/passwd");
		else
			printf("The size of this file is %d\n",infobuf.st_size);
	}
		
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

void do_ls(char []);
void dostat(char *);
void show_file_info(char * ,struct stat *);
void mode_to_letters(int ,char []);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

void main(int argc,char *argv[])
{
	if(argc == 1)
		do_ls(".");
	else 
		while(--ac)
		{
			printf("%s:\n",*++argv);
			do_ls(*argv);
		}
}

void do_ls(char dirname[])
{
	DIR *dir_ptr;
	struct dirent *direntp;
	
	if((dir_ptr = opendir(dirname) ) == NULL)
		printf(stderr,"ls:cannot open %s\n",dirname);
	else
	{
		while((direntp = readdir(dir_ptr)) != NULL)
			dostat(direntp->d_name);
		closedir(dir_ptr);
	}
}

void dostat(char *filename)
{
	struct stat info;
	if(stat(filename,&info) == -1)
		perror(filename);
	else
		show_file_info(filename,&info);
}

void show_file_info(char *filename,struct stat *info_p)
{
	char *uid_to_name(),*ctime(),*gid_to_name,*filemode();
	void mode_to_letters();
	char modestr[11];
	
	mode_to_letters(info_p->st_mode,modetr);
	
	printf
}
