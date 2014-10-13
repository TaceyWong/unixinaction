/*
	注销过程：
		①打开文件utmp
			fd = open(UTMP_FILE,O_RDWR)
		②从utmp中找到包含你所在终端的登录信息
			比对ut_line和你的终端名字如果相同则调用修改函数
		④对当前记录做修改
			文件操作中，改变一个文件的当前读写位置：系统调用lseek
			目标：使指针指向文件中的指定位置
			头文件 sys/type.h
				unistd.h
			函数原型： off_t oldpos = lseek(int_fd,off_t dist, int base)
					fd 文件描述符
					dist 移动的距离（可以为负）
					base SEEK_SET -> WENJIANDEKAISHI
					     SEEK_CUR -> 当前位置
					     SEEK_END -> 文件的结尾
			返回值： -1   遇到错误
				oldpos 指针变化前的位置
			note ：lseek(fd,0,SEEK_CUR)返回指针当前指向的位置
					
		④关闭文件
			close(fd)
*/

int logout_tty(char *line)
{
	int fd;
	struct utmp rec;
	int len = sizeof(struct utmp);
	int retval = -1;
	
	if((fd = open(UTMP_FILE,O_RDWR)== -1))
		return -1
	
	while(read(fd,&rec,len == len)
		if(strncmp(rec.ut_line,line sizeof(rec.ut_line)) == 0)
		{
			rec.ut_type = DEAD_PROCESS;
			if(time(&rec,ut_Line !=-1))
				if(lseek(fd,-len,SEEK_CUR) != -1)
					if(write(fd,&rec,len) == len)
						retval = 0;
			break;
		}
		
		if (close(fd) == -1)
			retval = -1;
		return retval;
}
