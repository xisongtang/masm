#include "control.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "calculator.h"
using namespace std;
#ifdef TEST

int main()
{
	Calculator calc("-1 >> 33");
	cout << calc.exec();
	system("pause");
}
#endif