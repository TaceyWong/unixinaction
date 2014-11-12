#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXTOKENS 100;
#define MAXTOKENLEN 60;

rnum type_tag {IDENTIFIER,QUALIFIER,TYPE};

struct token
	{
		char type;
		char string[MAXTOKENLEN];
	};

int top = -1;
struct token stack[MAXTOKENLEN];
struct token this;

#define pop stack[top--]
#define push(s) stack[++top] = s

enum tupe_tag classify_string(void)
/*推断标示符的类型*/
{
	char *s = this.string;
	if(!strcmp(s,"const"))
	{
		strcpy(s,"read-only");
		return QUALIFIER ;
	}
	if(!strcmp(s,"volatile"))
		return QUALIFIER;
	if(!strcmp(s,"void"))
		return TYPE;
	if(!strcmp(s,"char"))
		return TYPE;
	if(!strcmp(s,"signed"))
		return TYPE;
	if(!strcmp(s,"usigned"))
		return TYPE;
	if(!strcmp(s,"short"))
		return TYPE;
	if(!strcmp(s,"int"))
		return TYPE;
	if(!strcmp(s,"long"))
		return TYPE;
	if(!strcmp(s,"float"))
		return TYPE;
	if(!strcmp(s,"double"))
		return TYPE;
	if(!strcmp(s,"struct"))
		return TYPE;
	if(!strcmp(s,"union"))
		return TYPE;
	if(!strcmp(s,"enum"))
		return TYPE;

	return IDENTIFIER;
}

void gettoken(void)
/*读取下一个标记到this*/
{
	char *p = this.string;

	//略过空白符
	while((*p = getchar()) == ' ');

	if(isalnum(*p))
	{
		//读入的标示符以A-Z，0-9开头
		while (isalnum(*++P = getchar()));

		ungetc(*p,stdin);
		*p = '\0';
		this.type = classify_string();
		return ;

	}

	if(*p == '*')
	{
		strcpy(this.string,"pointer to");
		this.type = '*';
		return ;
	}

	this.string[1] = '\0';
	this.type = *p;
	return;
}

/*理解所有分析过程的代码段*/
read_to_first_indetifer()
{
	gettoken();
	while(this.type != IDENTIFIER)
	{
		push(this);
		gettoken();
	}

	printf("%s is \n", this.string );
	gettoken();
}

deal_with_arrys()
{
	while(this.type == '[')
	{
		printf("array\n");
		gettoken(); //数字或']'
		if(isdigit(this.string[0]))
		{
			printf("0..%d".atoi(this.string)-1);
			gettoken();//读取']'
		}
		gettoken();//读取']'之后的下一个标记
		printf("of");
	}
}


deal_with_function_args()
{
	while(this.type != ')')
		gettoken();
	gettoken();
	printf("function returnning");

}

deal_with_pointers()
{
	while(this.type == '*')
		printf("%s", pop.string );
}

deal_with_declarator()
{
	/*处理标识符之后可能存在的数组/函数*/
	switch(this.type)
	{
		case '[' :
				deal_with_arrys();
				break;
		case '(' :
				deal_with_function_args();
				break;
	}
	deal_with_pointers();
	/*处理在读入到标识符之前压入堆栈的符号*/
	while(top >= 0)
	{
		if(stack[top].type =='(')
		{
			pop;
			gettoken();//读取')'之后的符号
			deal_with_declarator();

		}
		else
		{
			printf("%s ", pop.string );
		}
	}
}

int main()
{
	/*将标记压入堆栈中，直到遇见标识符*/
	read_to_first_indetifier();
	deal_with_declarator();
	printf("\n");
	return 0
}
