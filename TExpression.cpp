#include "TExpression.h"


TExpression::TExpression(int max_TCHAR)
{
	expression = (TCHAR *)malloc(max_TCHAR*sizeof(TCHAR));
}


TExpression::~TExpression()
{
	free(expression);
}


void TExpression::OutputIntro()
{
	using namespace std;
	cout << "----- 表达式运算Demo -----" << endl;
	cout << "作者：Tom Willow 2016.11.04" << endl;
	cout << "功能：输入浮点数表达式计算出值。" << endl;
	cout << "支持：加+ 减- 乘* 除/ 幂运算^ 求余% 按位与& 按位或|" << endl;
	cout << "举例：输入6-5*(4-3+(2+1)) 返回-14" << endl;
}


std::string TExpression::GetValue()
{
	std::string s;

	/* 1.字符串转换为in order队列 */
	std::queue<Element> inorder, postorder;
	if (Str2Queue(&inorder, expression, err_info) != 0)
	{
		s.append("表达式非法：");
		s.append(err_info.data());
	}
	else
		/* 2.in order队列转换为post order队列 */
		if (InQueue2PostQueue(&postorder, &inorder))
		{
			s.append("括号不匹配。");
		}
		else
		{
			/* 3.由post order队列计算出值 */
			std::ostringstream oss;
			oss << Calc(&postorder);
			s = oss.str();
		}
	return s;
}


/* 返回运算符结合性 */
bool TExpression::isLeft2Right(TCHAR c)
{
	switch (c)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '&':
	case '|':
	case '%':return true;
	case '^':return false;
	}
}

/* 返回运算符的优先级 */
int TExpression::Rank(TCHAR s[])
{
	switch (s[0])
	{
	case '(':return 0;
	case ')':return 0;//左右括号优先级小是为了不被其余任何运算符挤出
	case '+':
	case '-':return 5;//低优先级将挤出高优先级
	case '*':
	case '/':return 10;
	case '^':return 11;
	case '&':
	case '|':return 12;
	case '%':return 15;//取余运算
	}
	int err = -1;
	if (isDouble(s[0])) err = 0;
	return err;
}

/* 是运算符 */
int TExpression::isOperator(TCHAR c)
{
	switch (c)
	{
	case '(':
	case ')':return 10;
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
	case '&':
	case '|':
	case '%':return 1;
	}
	return 0;
}

/* 有效性检查（返回0则出现异常字符） */
int TExpression::isLegal(TCHAR c)
{
	if (isDouble(c)) return 1;
	if (isOperator(c)) return 1;
	return 0;
}

/* 字符是0-9或. */
int TExpression::isDouble(TCHAR c)
{
	if (c >= '0' && c <= '9' || c == '.')
		return 1;
	else
		return 0;
}

/* 由位数得到应该相乘的倍数 如digit=2返回100 */
int TExpression::digit2num(int digit)
{
	int temp = 1;
	digit--;
	while (digit)
	{
		temp *= 10;
		digit--;
	}
	return temp;
}

/* 复制字符串 */
void TExpression::substr(TCHAR *Dest, const TCHAR *Source, int start, int count)
{
	int j = 0;

	for (int i = start; i<start + count; i++, j++)
	{
		Dest[j] = Source[i];
	}
	Dest[j] = '\0';
}

/*由in order队列得到post order队列*/
int TExpression::InQueue2PostQueue(std::queue<Element> *post, std::queue<Element> *in)//返回0:正常 1:括号不配对
{
	int sq_err = 0;
	std::stack<Element> temp;
	while (in->size()>0)
	{
		if (in->front().type == 0)
		{
			post->push(in->front());
			in->pop();
		}
		else
		{
			if (in->front().operate[0] == '(') //左括号直接入栈
			{
				temp.push(in->front());
				in->pop();
				sq_err++;
			}
			else
			{
				if (in->front().operate[0] == ')')//出现右括号
				{
					while (temp.size()>0)
					{
						if (temp.top().operate[0] == '(')
						{
							temp.pop();
							sq_err--;
							break;
						}
						else
						{
							post->push(temp.top());
							temp.pop();
						}
					}
					in->pop();
				}
				else//不是括号
				{
					if (temp.size()>0 && temp.top().left2right == true)//左结合
						while (temp.size()>0 && Rank(in->front().operate) <= Rank(temp.top().operate))//临时栈有内容，且新进符号优先级低，则挤出高优先级及同优先级符号
						{
							post->push(temp.top());//符号进入post队列
							temp.pop();
						}
					else//右结合
						while (temp.size()>0 && Rank(in->front().operate)<Rank(temp.top().operate))//临时栈有内容，且新进符号优先级低，则挤出高优先级，但不挤出同优先级符号（因为右结合）
						{
							post->push(temp.top());//符号进入post队列
							temp.pop();
						};
					temp.push(in->front());//高优先级已全部挤出，当前符号入栈
					in->pop();
				}

			}
		}
	}
	while (temp.size()>0)
	{
		post->push(temp.top());
		temp.pop();
	}
	return sq_err;
}

//int Str2Queue(queue<Element> *inorder, TCHAR expression[], string err_info)
//{
//	int i = 0;
//	TCHAR c[10];
//	while ((c[0]=expression[i]) != '/0')
//	{
//		if (isDouble(
//		switch (expression[i])
//		{
//		case ' ':
//			i++;
//			break;
//		}
//	}
//}

/*由字符串表达式得到in order队列*/
int TExpression::Str2Queue(std::queue<Element> *inorder, TCHAR expression[], std::string error)
{
	int err = 0;
	Element tempe;
	int a, b;
	int type, sign = 1;
	double num;
	for (int i = 0; i<(int)_tcslen(expression);)
	{
		num = 0;
		type = isDouble(expression[i]);
		if (type) a = i;//是数字
		while (isDouble(expression[i]) && i<(int)_tcslen(expression))
		{
			i++;
		}
		if (type)//是数字
		{
			b = i - a;//得到数字位数
			TCHAR newnum[32];
			substr(newnum, expression, a, b);
			num = _tcstod(newnum,(TCHAR **)(newnum+b));
			tempe.type = 0;
			tempe.value = num*sign;
			sign = 1;//sign符号正常化
			tempe.operate[0] = '\0';
			(*inorder).push(tempe);
		}
		else//不是数字
		{
			if ((expression[i] == '-' && i == 0) || (expression[i] == '-' && isOperator(expression[i - 1])))//
			{
				sign *= -1;
			}
			else
			{
				/* 非法性判定 */
				if (isLegal(expression[i]) != 1) { err = -1; error = "出现非法字符"; break; }//
				if (i == 0 && isOperator(expression[i])) { err = -1; error = "首字符出现非-的符号"; break; }//
				if (i + 1 < (int)_tcslen(expression) && isOperator(expression[i]) == 1 && expression[i + 1] == ')') { err = -1; error = "出现*)形式"; break; }//
				if (i - 1 >= 0 && isOperator(expression[i]) == 1 && (expression[i - 1] == '(' || isOperator(expression[i - 1]) == 1)) { err = -1; error = "出现(*形式或**形式"; break; }//

				tempe.type = 1;
				tempe.value = 0;
				tempe.operate[0] = expression[i];
				tempe.operate[1] = '\0';
				tempe.left2right = isLeft2Right(expression[i]);
				tempe.op_num = 2;
				(*inorder).push(tempe);
			}
			i++;
		}
	}
	return err;
}

/*由运算符c及a,b计算出值*/
double TExpression::CalcNum(double a, double b, TCHAR c)
{
	switch (c)
	{
	case '+':return a + b;
	case '-':return a - b;
	case '*':return a*b;
	case '/':return a / b;
	case '%':return (int)a % (int)b;
	case '&':return (int)a&(int)b;
	case '|':return (int)a | (int)b;
	case '^':return pow((double)a, (double)b);
	}
}

/*由post order序列计算出值*/
double TExpression::Calc(std::queue<Element> *post)
{
	Element tempe;
	std::stack<Element> temp;
	double a, b;
	while (post->size()>0)
	{
		if (post->front().type == 1)//运算符
		{
			b = temp.top().value; temp.pop();
			a = temp.top().value; temp.pop();
			tempe.type = 0; tempe.operate[0] = '\0'; tempe.value = CalcNum(a, b, post->front().operate[0]);
			temp.push(tempe);
			post->pop();
		}
		else
		{
			temp.push(post->front());
			post->pop();
		}
	}
	return temp.top().value;
}