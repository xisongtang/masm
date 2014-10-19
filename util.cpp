#include "util.h"
#include "calculator.h"
#include <string>
#include <sstream>
#include <regex>
#include <vector>
using namespace std;

#define MAXLEN 0xff

string replace(string source, string e, string c)
{
	int index = -1;
	while ( (index = source.find(e, index + 1)) != string::npos)
	{
		source.replace(index, e.size(), c);
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
	int scale = 10;
	if (_number.size() > 1 && (_number[0] == '-' || _number[0] == '+'))
		_number = _number.substr(1);
	if (_number.size() > 2 && _number[0] == '0' && _number[1] == 'x')
	{
		scale = 16;
		_number = _number.substr(2);
	}
	else if (_number.size() > 1 && _number[0] == '0')
	{
		scale = 8;
		_number = _number.substr(1);
	}
	bool result;
	if (scale == 16)
	{
		regex re("[0-9A-Ba-b]+");
		result = regex_match(_number, re);
	}
	else if (scale == 10)
	{
		regex re("[0-9]+");
		result = regex_match(_number, re);
	}
	else 
	{
		regex re("[0-7]+");
		result = regex_match(_number, re);
	}
	return result;
}

bool islegallabel(const string &label)
{
	return !isdigit(label[0]) && label[0] != '-';
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

vector<string> split(string src, string s)
{
	vector<string> vs;
	string tmp = "(.*?)\\" + s;
	if (trim(src).size() == 0)
	{
		vs.push_back("0");
		return vs;
	}
	regex re(tmp);
	smatch sm;
	while (regex_search(src, sm, re))
	{
		vs.push_back(sm[1]);
		src = sm.suffix();
	}
	vs.push_back(src);
	return vs;
}

void find(const string &str, string restr, smatch &sm)
{
	regex re(restr, regex::icase);
	regex_search(str, sm, re);
}

int find(const string &str, string restr)
{
	regex re(restr, regex::icase);
	smatch sm;
	regex_search(str, sm, re);
	if (sm.empty())
		return -1;
	return sm.prefix().length();
}

string decodesuffix(string str)
{
	regex re("\"(.*?)\"");
	smatch sm;
	stringstream strstream;
	while (regex_search(str, sm, re))
	{
		string rep = sm[1];
		string des = "";
		for (int i = 0; i != rep.size(); ++i)
		{
			string tmp;
			strstream << (int)rep[i];
			strstream >> tmp;
			des += tmp;
			strstream.clear();
			if (i != rep.size() - 1)
				des += ",";
		}
		str = sm.prefix().str() + des + sm.suffix().str();
	}
	str = replace(str, " ", "");

	vector<string> vs = split(str);
	str = "";
	for (vector<string>::iterator it = vs.begin(); it != vs.end();
			++it)
	{
		string tmp = *it;
		if (!Calculator::isequation(tmp))
			;
		else 
		{
			Calculator calc(tmp);
			strstream << calc.exec();
			strstream >> tmp;
		}
		str += tmp;
		if (it + 1 != vs.end())
			str += ",";
	}
	return str;
}


