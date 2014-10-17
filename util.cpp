#include "util.h"
#include <string>
#include <sstream>
#include <regex>
#include <vector>
using namespace std;

#define MAXLEN 0xff

string replace(string source, string e, string c)
{
	int index = 0;
	while ( (index = source.find(e, index)) != string::npos)
	{
		source.replace(index, e.length(), c);
	}
	return source;
}

string dump(stringstream &strstream)
{
	string ret;
	char tmp[MAXLEN];
	memset(tmp, 0, MAXLEN);
	strstream.read(tmp, MAXLEN);
	ret = tmp;
	return ret;
}

bool isnumber(const string &number)
{
	string _number = number;
	if (_number.size() > 1 && _number[0] == '-')
		_number = _number.substr(1);
	if (_number.size() > 2 && _number[0] == '0' && _number[1] == 'x')
		_number = _number.substr(2);
	for (string::const_iterator it = _number.begin(); it != _number.end(); ++it)
	{
		if (!isdigit(*it))
			return false;
	}
	return true;
}

bool islegallabel(const string &label)
{
	return !isdigit(label[0]);
}

string tolower(string str)
{
	for (int i = 0; i != str.size(); ++i)
	{
		if (str[i] <= 'Z' && str[i] >= 'A')
			str[i] = str[i] - 'A' + 'a';
	}
	return str;
}

bool has(string src, string des, bool whole)//the des shouldn't contain any regular special character
{
	if (whole)
	{
		string tmp = "\\b";
		des = tmp + des + tmp;
	}
	regex re(des);
	return regex_search(src, re);
}

string trim(string str, string reg)
{
	string tmp = "^";
	tmp += reg + "+";
	regex re(tmp);
	tmp = reg + "+$";
	regex re1(tmp);
	str = regex_replace(str, re, "");
	str = regex_replace(str, re1, "");
	return str;
}

vector<long long> split(string src, string s)
{
	vector<long long> vs;
	string tmp = "(.*?)\\" + s;
	regex re(tmp);
	smatch sm;
	int i;
	stringstream strstream;
	while (regex_search(src, sm, re))
	{
		strstream << sm[1];
		src = sm.suffix();
		strstream >> i;
		strstream.clear();
		vs.push_back(i);
	}
	strstream << src;
	strstream >> i;
	vs.push_back(i);
	return vs;
}

smatch find(string str, string restr)
{
	regex re(restr);
	smatch sm;
	regex_search(str, sm, re);
	return sm;
}

string decodesuffix(string str)
{
	regex re("\"(.*?)\"");
	smatch sm;
	while (regex_search(str, sm, re))
	{
		string rep = sm[1];
		string des = "";
		for (int i = 0; i != rep.size(); ++i)
		{
			des += int(rep[i]);
			if (i != rep.size() - 1)
				des += ",";
		}
		str = sm.prefix().str() + des + sm.suffix().str();
	}
	return str;
}