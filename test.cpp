#include "control.h"
#include "util.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "calculator.h"
using namespace std;
#ifdef TEST

int main()
{
	Calculator calc("(-1)*32767");
	cout << calc.exec();
	system("pause");
}
#endif