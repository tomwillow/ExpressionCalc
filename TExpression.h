#pragma once
#include <iostream>
#include <tchar.h>
#include <cstring>
#include <queue>
#include <stack>
#include <sstream>//double to string

class TExpression
{
private:
	/* 单个元素 */
	struct Element
	{
		int type;//0:数字  1:运算符  2:变量
		double value;//数字的值
		TCHAR operate[5];//运算符
		int op_num;//操作数个数
		bool left2right;//运算顺序左结合
	};

	/* 错误信息 */
	std::string err_info;
	TCHAR *expression;

	/* 返回运算符结合性 */
	bool isLeft2Right(TCHAR c);

	/* 返回运算符的优先级 */
	int Rank(TCHAR s[]);

	/* 是运算符 */
	int isOperator(TCHAR c);

	/* 有效性检查（返回0则出现异常字符） 是数字或运算符 */
	int isLegal(TCHAR c);

	/* 字符是0-9或. */
	int isDouble(TCHAR c);

	/* 由位数得到应该相乘的倍数 如digit=2返回100 */
	int digit2num(int digit);

	/* 复制字符串 */
	void substr(TCHAR *Dest, const TCHAR *Source, int start, int count);
	
	/*由字符串表达式得到in order队列*/
	int Str2Queue(std::queue<Element> *inorder, TCHAR expression[], std::string error);

	/*由in order队列得到post order队列*/
	int InQueue2PostQueue(std::queue<Element> *post, std::queue<Element> *in);//返回0:正常 1:括号不配对

	/*由运算符c及a,b计算出值*/
	double CalcNum(double a, double b, TCHAR c);

	/*由post order序列计算出值*/
	double Calc(std::queue<Element> *post);
public:
	TExpression(int max_TCHAR);
	~TExpression();
	void OutputIntro();
	std::string GetValue();
	TCHAR * GetExpression()
	{
		return expression;
	};
};

