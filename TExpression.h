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
	/* ����Ԫ�� */
	struct Element
	{
		int type;//0:����  1:�����  2:����
		double value;//���ֵ�ֵ
		TCHAR operate[5];//�����
		int op_num;//����������
		bool left2right;//����˳������
	};

	/* ������Ϣ */
	std::string err_info;
	TCHAR *expression;

	/* �������������� */
	bool isLeft2Right(TCHAR c);

	/* ��������������ȼ� */
	int Rank(TCHAR s[]);

	/* ������� */
	int isOperator(TCHAR c);

	/* ��Ч�Լ�飨����0������쳣�ַ��� �����ֻ������ */
	int isLegal(TCHAR c);

	/* �ַ���0-9��. */
	int isDouble(TCHAR c);

	/* ��λ���õ�Ӧ����˵ı��� ��digit=2����100 */
	int digit2num(int digit);

	/* �����ַ��� */
	void substr(TCHAR *Dest, const TCHAR *Source, int start, int count);
	
	/*���ַ������ʽ�õ�in order����*/
	int Str2Queue(std::queue<Element> *inorder, TCHAR expression[], std::string error);

	/*��in order���еõ�post order����*/
	int InQueue2PostQueue(std::queue<Element> *post, std::queue<Element> *in);//����0:���� 1:���Ų����

	/*�������c��a,b�����ֵ*/
	double CalcNum(double a, double b, TCHAR c);

	/*��post order���м����ֵ*/
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

