#include "list.h"
#include "control.h"
#include <regex>
using namespace std;
#ifdef TEST

int main()
{
	regex re("\\.([248]byte)?(ascii(?:z))?(space)?");
	smatch sm;
	string str = "12123";
	regex_search(str, sm, re);
	return 0;
}
#endif