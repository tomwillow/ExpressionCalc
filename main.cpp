#include <iostream>
#include "TExpression.h"

using namespace std;

int main()
{
	TExpression Expression(100);
	Expression.OutputIntro();
	while (1)
	{
		cout << ">>";
		cin >> Expression.GetExpression();
		cout << Expression.GetValue();
		cout << endl;
	}

}