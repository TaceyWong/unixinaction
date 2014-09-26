#include <stdio.h>

typedef struct 
{
	int code;//token的类别，1->char 2->digit
	char value;
}TOKEN,*token;

typedef struct
{
	int id; // 产生式标号
	char left;//产生式左部
	char right[256] = //产生式又不
	int r_right;//产生式右部长度
}FORMULA,*formula;

typedef struct 
{
	char symbol; // 操作符
	char num1; //第一个操作数
	char num2;//第二个操作数
	char result;//结果变量
}EXPRESSION,*expression;


int main()
{
	token retToken = malloc(sizeof(TOKEN));
	/*     */
	
	while(1)
	{
		switch(state)
		{
			case 0:
			
		}
	}
}
