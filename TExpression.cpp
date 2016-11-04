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
	cout << "----- ���ʽ����Demo -----" << endl;
	cout << "���ߣ�Tom Willow 2016.11.04" << endl;
	cout << "���ܣ����븡�������ʽ�����ֵ��" << endl;
	cout << "֧�֣���+ ��- ��* ��/ ������^ ����% ��λ��& ��λ��|" << endl;
	cout << "����������6-5*(4-3+(2+1)) ����-14" << endl;
}


std::string TExpression::GetValue()
{
	std::string s;

	/* 1.�ַ���ת��Ϊin order���� */
	std::queue<Element> inorder, postorder;
	if (Str2Queue(&inorder, expression, err_info) != 0)
	{
		s.append("���ʽ�Ƿ���");
		s.append(err_info.data());
	}
	else
		/* 2.in order����ת��Ϊpost order���� */
		if (InQueue2PostQueue(&postorder, &inorder))
		{
			s.append("���Ų�ƥ�䡣");
		}
		else
		{
			/* 3.��post order���м����ֵ */
			std::ostringstream oss;
			oss << Calc(&postorder);
			s = oss.str();
		}
	return s;
}


/* �������������� */
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

/* ��������������ȼ� */
int TExpression::Rank(TCHAR s[])
{
	switch (s[0])
	{
	case '(':return 0;
	case ')':return 0;//�����������ȼ�С��Ϊ�˲��������κ����������
	case '+':
	case '-':return 5;//�����ȼ������������ȼ�
	case '*':
	case '/':return 10;
	case '^':return 11;
	case '&':
	case '|':return 12;
	case '%':return 15;//ȡ������
	}
	int err = -1;
	if (isDouble(s[0])) err = 0;
	return err;
}

/* ������� */
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

/* ��Ч�Լ�飨����0������쳣�ַ��� */
int TExpression::isLegal(TCHAR c)
{
	if (isDouble(c)) return 1;
	if (isOperator(c)) return 1;
	return 0;
}

/* �ַ���0-9��. */
int TExpression::isDouble(TCHAR c)
{
	if (c >= '0' && c <= '9' || c == '.')
		return 1;
	else
		return 0;
}

/* ��λ���õ�Ӧ����˵ı��� ��digit=2����100 */
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

/* �����ַ��� */
void TExpression::substr(TCHAR *Dest, const TCHAR *Source, int start, int count)
{
	int j = 0;

	for (int i = start; i<start + count; i++, j++)
	{
		Dest[j] = Source[i];
	}
	Dest[j] = '\0';
}

/*��in order���еõ�post order����*/
int TExpression::InQueue2PostQueue(std::queue<Element> *post, std::queue<Element> *in)//����0:���� 1:���Ų����
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
			if (in->front().operate[0] == '(') //������ֱ����ջ
			{
				temp.push(in->front());
				in->pop();
				sq_err++;
			}
			else
			{
				if (in->front().operate[0] == ')')//����������
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
				else//��������
				{
					if (temp.size()>0 && temp.top().left2right == true)//����
						while (temp.size()>0 && Rank(in->front().operate) <= Rank(temp.top().operate))//��ʱջ�����ݣ����½��������ȼ��ͣ��򼷳������ȼ���ͬ���ȼ�����
						{
							post->push(temp.top());//���Ž���post����
							temp.pop();
						}
					else//�ҽ��
						while (temp.size()>0 && Rank(in->front().operate)<Rank(temp.top().operate))//��ʱջ�����ݣ����½��������ȼ��ͣ��򼷳������ȼ�����������ͬ���ȼ����ţ���Ϊ�ҽ�ϣ�
						{
							post->push(temp.top());//���Ž���post����
							temp.pop();
						};
					temp.push(in->front());//�����ȼ���ȫ����������ǰ������ջ
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

/*���ַ������ʽ�õ�in order����*/
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
		if (type) a = i;//������
		while (isDouble(expression[i]) && i<(int)_tcslen(expression))
		{
			i++;
		}
		if (type)//������
		{
			b = i - a;//�õ�����λ��
			TCHAR newnum[32];
			substr(newnum, expression, a, b);
			num = _tcstod(newnum,(TCHAR **)(newnum+b));
			tempe.type = 0;
			tempe.value = num*sign;
			sign = 1;//sign����������
			tempe.operate[0] = '\0';
			(*inorder).push(tempe);
		}
		else//��������
		{
			if ((expression[i] == '-' && i == 0) || (expression[i] == '-' && isOperator(expression[i - 1])))//
			{
				sign *= -1;
			}
			else
			{
				/* �Ƿ����ж� */
				if (isLegal(expression[i]) != 1) { err = -1; error = "���ַǷ��ַ�"; break; }//
				if (i == 0 && isOperator(expression[i])) { err = -1; error = "���ַ����ַ�-�ķ���"; break; }//
				if (i + 1 < (int)_tcslen(expression) && isOperator(expression[i]) == 1 && expression[i + 1] == ')') { err = -1; error = "����*)��ʽ"; break; }//
				if (i - 1 >= 0 && isOperator(expression[i]) == 1 && (expression[i - 1] == '(' || isOperator(expression[i - 1]) == 1)) { err = -1; error = "����(*��ʽ��**��ʽ"; break; }//

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

/*�������c��a,b�����ֵ*/
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

/*��post order���м����ֵ*/
double TExpression::Calc(std::queue<Element> *post)
{
	Element tempe;
	std::stack<Element> temp;
	double a, b;
	while (post->size()>0)
	{
		if (post->front().type == 1)//�����
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