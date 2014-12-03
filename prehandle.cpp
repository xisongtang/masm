#include "prehandle.h"
#include "calculator.h"
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <regex>
#include <cassert>
#include "util.h"
#include "exception.h"
using namespace std;

Prehandle::Prehandle()
{
	reg_map["$zero"] = "$0";
	reg_map["$at"] = "$1";
	reg_map["$v0"] = "$2";
	reg_map["$v1"] = "$3";
	reg_map["$a0"] = "$4";
	reg_map["$a1"] = "$5";
	reg_map["$a2"] = "$6";
	reg_map["$a3"] = "$7";
	reg_map["$t0"] = "$8";
	reg_map["$t1"] = "$9";
	reg_map["$t2"] = "$10";
	reg_map["$t3"] = "$11";
	reg_map["$t4"] = "$12";
	reg_map["$t5"] = "$13";
	reg_map["$t6"] = "$14";
	reg_map["$t7"] = "$15";
	reg_map["$s0"] = "$16";
	reg_map["$s1"] = "$17";
	reg_map["$s2"] = "$18";
	reg_map["$s3"] = "$19";
	reg_map["$s4"] = "$20";
	reg_map["$s5"] = "$21";
	reg_map["$s6"] = "$22";
	reg_map["$s7"] = "$23";
	reg_map["$t8"] = "$24";
	reg_map["$t9"] = "$25";
	reg_map["$k0"] = "$26";
	reg_map["$k1"] = "$27";
	reg_map["$gp"] = "$28";
	reg_map["$sp"] = "$29";
	reg_map["$fp"] = "$30";
	reg_map["$ra"] = "$31";
	reg_map["$0"] = "$0";
	reg_map["$1"] = "$1";
	reg_map["$2"] = "$2";
	reg_map["$3"] = "$3";
	reg_map["$4"] = "$4";
	reg_map["$5"] = "$5";
	reg_map["$6"] = "$6";
	reg_map["$7"] = "$7";
	reg_map["$8"] = "$8";
	reg_map["$9"] = "$9";
	reg_map["$10"] = "$10";
	reg_map["$11"] = "$11";
	reg_map["$12"] = "$12";
	reg_map["$13"] = "$13";
	reg_map["$14"] = "$14";
	reg_map["$15"] = "$15";
	reg_map["$16"] = "$16";
	reg_map["$17"] = "$17";
	reg_map["$18"] = "$18";
	reg_map["$19"] = "$19";
	reg_map["$20"] = "$20";
	reg_map["$21"] = "$21";
	reg_map["$22"] = "$22";
	reg_map["$23"] = "$23";
	reg_map["$24"] = "$24";
	reg_map["$25"] = "$25";
	reg_map["$26"] = "$26";
	reg_map["$27"] = "$27";
	reg_map["$28"] = "$28";
	reg_map["$29"] = "$29";
	reg_map["$30"] = "$30";
	reg_map["$31"] = "$31";

	pseudo_map["move"] = new PseudoRule("rt,rs", "add:rt,rs,$0");
	pseudo_map["clear"] = new PseudoRule("rt", "add:rt,$0,$0");
	pseudo_map["not"] = new PseudoRule("rt,rs", "nor:rt,rs,$0");
	pseudo_map["la"] = new PseudoRule("rd,imme", "ori:rd,$0,imme");
	pseudo_map["li"] = new PseudoRule("rd,imme", "ori:rd,$0,imme");
	pseudo_map["b"] = new PseudoRule("imme", "beq:$0,$0,imme");
	pseudo_map["bal"] = new PseudoRule("imme", "bgezal:$0,$0,imme");
	pseudo_map["bgt"] = new PseudoRule("rs,rt,imme", "slt:$1,rt,rs;bne:$1,$0,imme");
	pseudo_map["blt"] = new PseudoRule("rs,rt,imme", "slt:$1,rs,rt;bne:$1,$0,imme");
	pseudo_map["bge"] = new PseudoRule("rs,rt,imme", "slt:$1,rs,rt;beq:$1,$0,imme");
	pseudo_map["ble"] = new PseudoRule("rs,rt,imme", "slt:$1,rt,rs;beq:$1,$0,imme");
	pseudo_map["bgtu"] = new PseudoRule("rs,rt,imme", "sltu:$1,rt,rs;bne:$1,$0,imme");
	pseudo_map["bgtz"] = new PseudoRule("rs,imme", "slt:$1,$0,rs;bne:$1,$0,imme");
	pseudo_map["beqz"] = new PseudoRule("rs,imme", "beq:rs,$0,imme");
	pseudo_map["bnez"] = new PseudoRule("rs,imme", "bne:rs,$0,imme");
	pseudo_map["rem"] = new PseudoRule("rd,rs,rt", "div:rs,rt;mfhi:rd");
	pseudo_map["push"] = new PseudoRule("rs", "sw:rs,imme($29);");
	pseudo_map["pop"] = new PseudoRule("rs", "lw:rs,imme($29);");
	pseudo_map["lui"] = new PseudoRule("rd,imme", "addi:$1,$0,imme;sll:rd,$1,16");
	pseudo_map["beqi"] = new PseudoRule("rs,imme,imme1", "ori:$1,$0,imme;beq:rs,$1,imme1");
	pseudo_map["bnei"] = new PseudoRule("rs,imme,imme1", "ori:$1,$0,imme;bne:rs,$1,imme1");
	pseudo_map["muli"] = new PseudoRule("rs,imme", "ori:$1,$0,imme;mul:rs,$1");
	pseudo_map["divi"] = new PseudoRule("rs,imme", "ori:$1,$0,imme;div:rs,$1");

	format_map["addi"] = "rt,rs,imme";
	format_map["addiu"] = "rt,rs,imme";
	format_map["andi"] = "rt,rs,imme";
	format_map["beq"] = "rt,rs,imme";
	format_map["bgez"] = "rs,imme";
	format_map["bgezal"] = "rs,imme";
	format_map["bgtz"] = "rs,imme";
	format_map["blez"] = "rs,imme";
	format_map["bltz"] = "rs,imme";
	format_map["bltzal"] = "rs,imme";
	format_map["bne"] = "rt,rs,imme";
	format_map["lh"] = "rt,imme(rs)";
	format_map["lw"] = "rt,imme(rs)";
	format_map["ori"] = "rt,rs,imme";
	format_map["sh"] = "rt,imme(rs)";
	format_map["slti"] = "rt,rs,imme";
	format_map["sw"] = "rt,imme(rs)";
	format_map["xori"] = "rt,rs,imme";
	format_map["j"] = "target";
	format_map["jal"] = "target";
	format_map["add"] = "rd,rs,rt";
	format_map["addu"] = "rd,rs,rt";
	format_map["and"] = "rd,rs,rt";
	format_map["div"] = "rs,rt";
	format_map["jalr"] = "rs,rd";
	format_map["jr"] = "rs";
	format_map["mfhi"] = "rd";
	format_map["mflo"] = "rd";
	format_map["mthi"] = "rs";
	format_map["mtlo"] = "rs";
	format_map["mul"] = "rs,rt";
	format_map["or"] = "rd,rs,rt";
	format_map["sll"] = "rd,rs,shamt";
	format_map["sllv"] = "rd,rt,rs";
	format_map["slt"] = "rd,rs,rt";
	format_map["sltu"] = "rd,rs,rt";
	format_map["sra"] = "rd,rs,shamt";
	format_map["srav"] = "rd,rs,rt";
	format_map["srl"] = "rd,rs,shamt";
	format_map["srlv"] = "rd,rs,rt";
	format_map["sub"] = "rd,rs,rt";
	format_map["subu"] = "rd,rs,rt";
	format_map["xor"] = "rd,rs,rt";
	format_map["move"] = "rt,rs";
	format_map["clear"] = "rt";
	format_map["not"] = "rt,rs";
	format_map["la"] = "rd,imme";
	format_map["li"] = "rd,imme";
	format_map["b"] = "imme";
	format_map["bal"] = "imme";
	format_map["bgt"] = "rs,rt,imme";
	format_map["blt"] = "rs,rt,imme";
	format_map["bge"] = "rs,rt,imme";
	format_map["ble"] = "rs,rt,imme";
	format_map["bgtu"] = "rs,rt,imme";
	format_map["bgtz"] = "rs,imme";
	format_map["beqz"] = "rs,imme";
	format_map["bnez"] = "rs,imme";
	format_map["rem"] = "rd,rs,rt";
	format_map["syscall"] = " ";
	format_map["push"] = "rs";
	format_map["pop"] = "rs";
	format_map["lui"] = "rd,imme";

	isincomment = isafterdata = hasdata = false;
	const_map["lb"] = "lh";
	const_map["sb"] = "sh";

}

string Prehandle::decode(string str) throw(...)
{
	string org_str = str;
	string label = "";
	int index;
	//deal with muliline comment
	if (isincomment)
	{
		if ((index = str.find("*/")) != string::npos)
		{
			str = str.substr(index + 2);
			isincomment = false;
		}
		else
			return "";
	}
	if ((index = str.find("/*")) != string::npos)
	{
		isincomment = true;
		str = str.substr(0, index);
	}
	//deal with c-style one line comment
	str = replace(str, "//", "#");
	index = str.find('#');
	string cmt_str;
	//divide the comment from the instruction
	if (index != string::npos)
	{
		cmt_str = trim(str.substr(index));
		str = trim(str.substr(0, index));
	}
	//resolve the label expression
	if ((index = str.find(":")) != string::npos)
	{
		label = str.substr(0, index + 1) + "\n";
		str = str.substr(index + 1);
	}
	str = trim(str);
	str = replace(str, "\t", " ");

	//change the char to ascii number
	while (true)
	{
		stringstream strstream;
		regex re("'(.*?)'", regex::icase);
		smatch sm;
		regex_search(str, sm, re);
		if (sm.empty())
			break;
		if (sm[1].length() > 1)
		{
			if (sm[1].str()[0] < 0 && sm[1].length() == 2)
				strstream << (int)(sm[1].str()[0] & 0xff) << (int)(sm[1].str()[1] & 0xff);
			else
				throw Exception("Invalid character format: it should be only one character", org_str);
		}
		else
			strstream << (int)(sm[1].str()[0]);
		string ascii;
		strstream >> ascii;
		str = sm.prefix().str() + ascii + sm.suffix().str();
	}
	//take the equ expression's information
	index = find(str, "equ");
	if (index != string::npos)
	{
		string name;
		string value;
		name = trim(str.substr(0, index));
		value = trim(str.substr(index+4));
		if (value.size() == 0 || name.size() == 0)
			throw Exception("Invalid use of equ expression", org_str);
		if (Calculator::isequation(value))
		{
			Calculator calc(value);
			stringstream strstream;
			strstream << calc.exec();
			strstream >> value;
		}
		const_map[name] = value;
		return "";
	}

	//resolve the dup expression
	while (true)
	{
		stringstream strstream;
		if (!has(str, "dup"))
			break;
		regex re("([0-9]*) dup\\((.*?)\\)", regex::icase);
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

	//resolve the value symlized by equ expressiona
	for (map<string, string>::iterator it = const_map.begin(); it != const_map.end(); ++it)
	{
		if (has(str, it->first))
			str = replace(str, it->first, it->second, true);
	}

	//resolve the multiline data
	smatch sm;
	find(str, "\\.([248]byte|byte|ascii|asciiz|space|half|word|dword)\\b", sm);
	string suffix = trim(sm.suffix().str());
	string prefix = trim(sm.prefix().str());
	string matched = sm[0];
	//if the line before contains data definition
	if (isafterdata)
	{
		// if this line follows the past definition
		if (!sm.empty() && prefix.size() == 0)
		{
			if (multilinetype != matched)
				throw Exception("Invalid format of multi-line data definition", org_str);
			suffix = decodesuffix(suffix);
			multilinedata += "," + suffix;
			return "";
		}
		// if not
		multilinedata += "\n";
		isafterdata = false;
	}
	//if not after data or after data but not follows the past definition
	if (!sm.empty())
	{
		//if after but not follows the past definition
		if (matched == ".byte")
			matched = ".2byte";
		else if (matched == ".ascii")
			matched = ".2byte";
		else if (matched == ".word")
			matched = ".4byte";
		else if (matched == ".half")
			matched = ".2byte";
		else if (matched == ".dword")
			matched = ".8byte";
		else if (matched == ".asciiz")
		{
			matched = ".2byte";
			suffix += ",0";
		}
		if (!isafterdata && hasdata)
		{
			isafterdata = true;
			hasdata = true;
			string tmp = multilinedata;
			multilinetype = matched;
			multilinedata = prefix + " " + matched + " " + decodesuffix(suffix);
			return tmp;
		}
		//if not after
		isafterdata = true;
		hasdata = true;
		multilinetype = matched;
		multilinedata = prefix + " " + matched + " " + decodesuffix(suffix);
		return "";
	}

	//resolve the .origin expression
	if ((index = str.find(".align"))  != string::npos)
	{
		string num = trim(str.substr(6));
		if (index > 0)
			throw Exception("Invalid use of '.origin'", org_str);
		if (num.size() == 0)
			str += " 2";
	}else if ((index = str.find(".origin")) != string::npos)
	{
		string num = trim(str.substr(7));
		if (index > 0)
			throw Exception("Invalid use of '.origin'", org_str);
		if (num.size() == 0)
			throw Exception("Invalid use of '.origin', lack of a number", org_str);
		if (!isnumber(num))
			throw Exception("Invalid use of '.origin', invalid number '" + num + "' after it", org_str);
	}else if ((index = str.find(".end")) != string::npos)
	{
		string label = str.substr(5);
		if (index > 0)
			throw Exception("Invalid use of '.end'", org_str);
		if (label.size() == 0)
			throw Exception("Invalid use of '.end', lack of a label after it", org_str);
		if (!islegallabel(label))
			throw Exception("Invalid use of '.end', invalid label '" + label + "' after it", org_str);
	}
	//resolve the section expression
	else if((index = str.find(".")) != string::npos)
	{
		if (index != 0)
			throw Exception("unaccepted data unit", org_str);
		stringstream strstream;
		string label, suffix;
		strstream << str;
		strstream >> label >> suffix;
		if (suffix.size() != 0)
			throw Exception("Invalid use of section: " + label, org_str);
	}
	//the instructions;
	else if (str.size())
	{
		stringstream strstream;
		strstream << str;
		string operation, operater;
		strstream >> operation;
		operater = dump(strstream);
		strstream.clear();

		if (format_map[operation] == "")
			throw Exception(Exception::ion + ":" + operation, org_str);
		string format;
		string regexstr;
		
		if (operation == "pop" || operation == "push")
		{
			vector<string> vs = split(operater);
			int len = vs.size();
			regexstr = "";
			format = "";
			for (int i = 0; i != len; ++i)
			{
				regexstr += "(.*)";
				format += "rs";
				if (i != len - 1)
				{
					regexstr += ",";
					format += ",";
				}
			}
		}
		else
		{
			format = format_map[operation];
			regexstr = format;
			regexstr = replace(regexstr, "\t", "");
			regexstr = replace(regexstr, " ", "");
			regexstr = replace(regexstr, "(", "\\(");
			regexstr = replace(regexstr, ")", "\\)");
			regexstr = replace(regexstr, "rs", "(.*?)");
			regexstr = replace(regexstr, "rt", "(.*?)");
			regexstr = replace(regexstr, "rd", "(.*?)");
			regexstr = replace(regexstr, "imme", "(.*?)");
			regexstr = replace(regexstr, "target", "(.*?)");
			regexstr = replace(regexstr, "shamt", "(.*?)");
		}
		regex re(regexstr);
		smatch opsm, fmsm;
		if (!regex_match(operater, opsm, re))
			throw Exception("Invalid operation format", org_str);
		regex_match(format, fmsm, re);
		for (int i = 1; i != opsm.size(); ++i)
		{
			string otmp, ftmp;
			otmp = tolower(trim(opsm[i].str()));
			ftmp = trim(fmsm[i].str());
			if (ftmp == "rs")
			{
				if (otmp[0] != '$' || reg_map[otmp] == "")
					throw Exception(Exception::irn + ": " + otmp, org_str);
			}
			else if (ftmp == "rt")
			{
				if (otmp[0] != '$' || reg_map[otmp] == "")
					throw Exception(Exception::irn + ": " + otmp, org_str);
			}
			else if (ftmp == "rd")
			{
				if (otmp[0] != '$' || reg_map[otmp] == "")
					throw Exception(Exception::irn + ": " + otmp, org_str);
			}
			else if (ftmp == "shamt")
			{
				if (Calculator::isequation(otmp))
				{
					string tmp;
					Calculator calc(otmp);
					strstream << calc.exec();
					strstream >> tmp;
					str = replace(str, otmp, tmp);
				}
				else if (!isnumber(otmp))
					throw Exception(Exception::is, org_str);
			}
			else if (ftmp == "imme")
			{
				if (Calculator::isequation(otmp))
				{
					string tmp;
					Calculator calc(otmp);
					strstream << calc.exec();
					strstream >> tmp;
					str = replace(str, otmp, tmp);
				}
				else if (islegallabel(otmp))
					;
				else if (!isnumber(otmp))
					throw Exception(Exception::ii, org_str);
			}
			else if (ftmp == "imme1")
			{
				if (Calculator::isequation(otmp))
				{
					string tmp;
					Calculator calc(otmp);
					strstream << calc.exec();
					strstream >> tmp;
					str = replace(str, otmp, tmp);
				}
				else if (islegallabel(otmp))
					;
				else if (!isnumber(otmp))
					throw Exception(Exception::ii, org_str);
			}
			else if (ftmp == "target")
			{
				if (Calculator::isequation(otmp))
				{
					string tmp;
					Calculator calc(otmp);
					strstream << calc.exec();
					strstream >> tmp;
					str = replace(str, otmp, tmp);
				}
				else if (islegallabel(otmp))
					;
				else if (!isnumber(otmp))
					throw Exception(Exception::it, org_str);
			}
			else if (operation != "syscall")
				assert(1 == 0);
		}
		//translate the pseudo codes to basic codes
		if (pseudo_map[operation])
		{
			string format = pseudo_map[operation]->pseudo;
			string des = pseudo_map[operation]->format;
			string ftmp;
			string otmp;
			string tmp;
			int lfindex = 0;
			int loindex = 0;
			int findex;
			int oindex;
			format = replace(format, "(", ",");
			format = replace(format, ")", "");
			format += ",";
			des = replace(des, ":", " ");
			des = replace(des, ";", "\n");
			str = "";
			operater += ",";
			int offset = 0;
			while ((findex = format.find(',',lfindex)) != string::npos)
			{
				oindex = operater.find(',', loindex);
				if (oindex == string::npos)
					break;
				ftmp = trim(format.substr(lfindex, findex - lfindex));
				otmp = trim(operater.substr(loindex, oindex - loindex));
				if (operation == "push" || operation == "pop")
				{
					strstream << offset;
					str += replace(des, ftmp, otmp);
					strstream >> otmp;
					strstream.clear();
					str = replace(str, "imme", otmp);
					offset += 2;
					lfindex = 0;
				}
				else
				{
					des = replace(des, ftmp, otmp);
					lfindex = findex + 1;
				}
				loindex = oindex + 1;
			}
			if (operation == "push" || operation == "pop")
			{
				string tmp;
				strstream << offset;
				strstream >> tmp;
				str = str.substr(0, str.size() - 1);
				if (operation == "push")
					str = "addi $29,$29,-" + tmp + "\n" + str;
				else
					str += "\naddi $29,$29," + tmp;
			}
			else 
				str = des;
		}
	}
	if (str.size() != 0)
		str += "\n";
	if(hasdata)
	{
		hasdata = false;
		return multilinedata + label + str;
	}
	return label + str;
}