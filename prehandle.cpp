#include "prehandle.h"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <regex>
#include "util.h"
#include "exception.h"
using namespace std;

Prehandle::Prehandle()
{
	isincomment = isafterdata = hasdata = false;
	multilinecomment = "";
}

string Prehandle::decode(string str) throw(...)
{
	string org_str = str;
	int index;
	bool iscommentend = false;
	if (isincomment)
	{
		string t = "#";
		if ((index = str.find("*/")) != string::npos)
		{
			string tmp = str.substr(0, index);
			str = str.substr(index + 2);
			tmp = trim(tmp);
			tmp = trim(tmp, "\\*");
			tmp = t + tmp;
			multilinecomment += tmp;
			isincomment = false;
			iscommentend = true;
		}
		else
		{
			str = trim(str);
			str = trim(str, "\\*");
			str = t + str + "\n";
			multilinecomment += str;
			return "";
		}
	}
	if ((index = str.find("/*")) != string::npos)
	{
		isincomment = true;
		multilinecomment = "#";
		multilinecomment += str.substr(index + 2);
		str = str.substr(0, index);
		multilinecomment = trim(multilinecomment);
		multilinecomment = trim(multilinecomment, "\\*");
		multilinecomment += "\n";
	}
	str = replace(str, "//", "#");
	index = str.find('#');
	string cmt_str;
	if (index != string::npos)
	{
		cmt_str = trim(str.substr(index));
		str = trim(str.substr(0, index));
	}
	str = tolower(str);
	index = str.find("equ");
	if (index != string::npos)
	{
		string name;
		string value;
		name = trim(str.substr(0, index));
		value = trim(str.substr(index+4));
		const_map[name] = value;
		return cmt_str + "\n";
	}
	while (true)
	{
		stringstream strstream;
		if (!has(str, "dup"))
			break;
		regex re("([0-9]*) dup\\((.*?)\\)");
		smatch sm;
		regex_search(str, sm, re);
		if (sm.empty())
			throw Exception(Exception::iud, org_str);
		int times;
		strstream << sm[1];
		strstream >> times;
		string tmp = "";
		for (int i = 0 ; i != times; ++i)
		{
			tmp += sm[2];
			if (i != times - 1)
				tmp += ',';
		}
		str = sm.prefix().str() + tmp + sm.suffix().str();
	}
	for (map<string, string>::iterator it = const_map.begin(); it != const_map.end(); ++it)
	{
		if (has(str, it->first))
			str = replace(str, it->first, it->second);
	}
	smatch sm = find(str, "\\.([248]byte)?(ascii(?:z))?(space)?");
	string suffix = trim(sm.suffix());
	string prefix = trim(sm.prefix());
	if (isafterdata)
	{
		if (!sm.empty() && prefix.size() == 0)
		{
			suffix = decodesuffix(suffix);
			multilinedata += suffix;
			return cmt_str + "\n";
		}
		multilinedata += commentafterdata + "\n";
		isafterdata == false;
	}
	if (!sm.empty())
	{
		isafterdata == true;
		hasdata = true;
		multilinedata = prefix + sm[0].str() + decodesuffix(suffix);
		commentafterdata = cmt_str;
		return 
	}

	if (iscommentend)
	{
		return multilinecomment + str + cmt_str + "\n";
	}
	if (isincomment)
		return str + cmt_str;
	else 
		return str + cmt_str + "\n";
}