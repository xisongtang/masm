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
	string operater = "$t1,$t1,0+1 * 2 + ~5*(3+4-2)";
	string format = "rd,rs,imme";
	string regexstr = format;
	regexstr = replace(regexstr, "\t", "");
	regexstr = replace(regexstr, " ", "");
	regexstr = replace(regexstr, "(", "\\(");
	regexstr = replace(regexstr, ")", "\\)");
	regexstr = replace(regexstr, "rs", "(.*)");
	regexstr = replace(regexstr, "rt", "(.*)");
	regexstr = replace(regexstr, "rd", "(.*)");
	regexstr = replace(regexstr, "imme", "(.*)");
	regexstr = replace(regexstr, "target", "(.*)");
	regexstr = replace(regexstr, "shamt", "(.*)");
	regex re(regexstr);
	smatch sm, fm;
	regex_match(operater, sm, re);
	regex_match(format, fm, re);
	cout << sm[1] << sm[2] << sm[3] << endl;
	cout << fm[1] << fm[2] << fm[3] << fm.size() <<  endl;
	cout << regexstr << endl;
	system("pause");
}
#endif