#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"stack.h"
#define MAX_NAME_LEN   15
typedef struct {
	int     token_type;
	int     variable;
} token;
#define TOKEN_PLUS       0
#define TOKEN_MINUS      1
#define TOKEN_MUL        2
#define TOKEN_DIV        3
#define TOKEN_LPARE      4 
#define TOKEN_RPARE      5 
#define TOKEN_EOS        6
#define TOKEN_VAR        7
token  get_token(FILE* fp)
{
	int ch;
	token temp;
	do { ch = fgetc(fp); } while ((ch == ' ') || (ch == '\t'));

	do {
		switch (ch) {
		case '+':  temp.token_type = TOKEN_PLUS;  return temp;
		case '-':  temp.token_type = TOKEN_MINUS; return temp;
		case '*':  temp.token_type = TOKEN_MUL;   return temp;
		case '/':  temp.token_type = TOKEN_DIV;   return temp;
		case '(':  temp.token_type = TOKEN_LPARE; return temp;
		case ')':  temp.token_type = TOKEN_RPARE; return temp;
		case '\n': temp.token_type = TOKEN_EOS;   return temp;
		default:
			temp.token_type = TOKEN_VAR;
			temp.variable = ch - '0';
			do {
				ch = fgetc(fp);
				if ((ch >= '0'&&ch <= '9')) {
					temp.variable = temp.variable * 10 + ch - '0';
				}
				else {
					ungetc(ch, fp);
					return temp;
				}
			} while (1);
		}
	} while (1);
}
void infix_to_postfix(FILE* fp)
{
	static int isp[] = { 12, 12, 13, 13, 0,  19, 0 }; // +, -, *, /, (, ), \n  
	static int icp[] = { 12, 12, 13, 13, 20, 19, 0 };
	static int operator_sym[] = { '+','-','*','/' };
	struct stack *s = create_stack(sizeof(int));
	int    op = TOKEN_EOS;
	token  x;
	push(s, &op);
	for (x = get_token(fp); x.token_type != TOKEN_EOS; x = get_token(fp))
	{
		if (x.token_type == TOKEN_VAR) { printf("%d ", x.variable); }
		else if (x.token_type == TOKEN_RPARE)
		{
			while (*((int*)top(s)) != TOKEN_LPARE) {
				printf("%c ", operator_sym[(*((int*)top(s)))]); pop(s);
			}
			pop(s);
		}   // 清光直到碰到 左括號  
		else if (x.token_type == TOKEN_LPARE) {
			push(s, &x.token_type);
		}
		else {
			while (isp[x.token_type]<=isp[(*((int*)top(s)))]&&stack_size>0)
			{
				printf("%c ", operator_sym[(*((int*)top(s)))]); pop(s);
			}
			push(s,&x.token_type);
			/*insert your code, 需要做一個push 的動作*/
		}
	}

	while (*((int*)top(s)) != TOKEN_EOS) {
		printf("%c ", operator_sym[(*((int*)top(s)))]);
		pop(s);
	} // 清光 stack 裡面所有東西
	printf("\n");
	release_stack(&s);
}

// Do not modify the main function
int main(void)
{
	infix_to_postfix(stdin);
	system("pause");
	return 0;
}
