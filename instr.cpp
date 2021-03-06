#include "instr.h"
#include "exception.h"
#include "util.h"
#include "list.h"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>
#include <cassert>
#include <cstdlib>
#include <cctype>

using namespace std;

bool Instr::is_map_initialized = false;
map<unsigned int, rule*> Instr::disasm_map;
map<unsigned int, rule*> Instr::disfuncasm_map;
map<string, rule*> Instr::asm_map;
map<string, string> Instr::reg_map;
vector<string> Instr::sections;

string Instr::cursect = "";

void Instr::_initialize()
{
	opcode = type = rs = rt = rd = shamt = func = imme = target = byte_code = 
		address = data_length = 0;
	has_label = is_label = is_origin = is_data = is_align = 
		is_instr = is_end = false;
	label = asm_code = operation = comment = section = "";
}

bool Instr::_mayhavelable()
{
	if (type == 'i' || type == 'j')
		return true;
	return false;
}

void Instr::initialize_map()
{
	if (is_map_initialized)
		return;
	//i-type
	asm_map["addi"] = disasm_map[0x08] = new rule("addi", 0x08,0, "rt,rs,imme", 'i');
	asm_map["addiu"] = disasm_map[0x09] = new rule("addiu", 0x09,0, "rt,rs,imme", 'i');
	asm_map["andi"] = disasm_map[0x0c] = new rule("andi", 0x0c,0, "rt,rs,imme", 'i');
	asm_map["beq"] = disasm_map[0x04] = new rule("beq", 0x04,0, "rs,rt,imme", 'i');
	asm_map["bgez"] = disasm_map[0x01] = new rule("bgez", 0x01,0, "rs,imme", 'i');
	//asm_map["bgezal"] = disasm_map[0x01] = new rule("bgezal", 0x01,0, "rs,imme", 'i');
	asm_map["bgtz"] = disasm_map[0x07] = new rule("bgtz", 0x07,0, "rs,imme", 'i');
	asm_map["blez"] = disasm_map[0x06] = new rule("blez", 0x06,0, "rs,imme", 'i');
	asm_map["bltz"] = disasm_map[0x09] = new rule("bltz", 0x01,0, "rs,imme", 'i');
	//asm_map["bltzal"] = disasm_map[0x01] = new rule("bltzal", 0x01,0, "rs,imme", 'i');
	asm_map["bne"] = disasm_map[0x05] = new rule("bne", 0x05,0, "rs,rt,imme", 'i');
	asm_map["lh"] = disasm_map[0x21] = new rule("lh", 0x21,0, "rt,imme(rs)", 'i');
	asm_map["lw"] = disasm_map[0x23] = new rule("lw", 0x23,0, "rt,imme(rs)", 'i');
	asm_map["ori"] = disasm_map[0x0D] = new rule("ori", 0x0D,0, "rt,rs,imme", 'i');
	asm_map["sh"] = disasm_map[0x29] = new rule("sh", 0x29,0, "rt,imme(rs)", 'i');
	asm_map["slti"] = disasm_map[0x0a] = new rule("slti", 0x0a,0, "rt,rs,imme", 'i');
	asm_map["sw"] = disasm_map[0x2b] = new rule("sw", 0x2b,0, "rt,imme(rs)", 'i');
	asm_map["xori"] = disasm_map[0x0E] = new rule("xori", 0x0E,0, "rt,rs,imme", 'i');
	asm_map["div"] = disasm_map[0x1A] = new rule("div", 0x1A, 0, "rs", 'i');
	asm_map["mul"] = disasm_map[0x1C] = new rule("mul", 0x1C, 0, "rs,rt", 'i');
	asm_map["int"] = disasm_map[0x3F] = new rule("int", 0x3F, 0, "imme", 'i');

	asm_map["j"] = disasm_map[0x02] = new rule("j", 0x02,0, "target", 'j');
	asm_map["jal"] = disasm_map[0x03] = new rule("jal", 0x03,0, "target", 'j');

	asm_map["add"] = disfuncasm_map[0x20] = new rule("add", 0,0x20, "rd,rs,rt", 'r');
	asm_map["addu"] = disfuncasm_map[0x21] = new rule("addu", 0,0x21, "rd,rs,rt", 'r');
	asm_map["and"] = disfuncasm_map[0x24] = new rule("and", 0,0x24, "rd,rs,rt", 'r');
	asm_map["jalr"] = disfuncasm_map[0x09] = new rule("jalr", 0,0x09, "rs,rd", 'r');
	asm_map["jr"] = disfuncasm_map[0x08] = new rule("jr", 0,0x08, "rs", 'r');
	asm_map["mfhi"] = disfuncasm_map[0x10] = new rule("mfhi", 0,0x10, "rd", 'r');
	asm_map["mflo"] = disfuncasm_map[0x12] = new rule("mflo", 0,0x12, "rd", 'r');
	asm_map["mthi"] = disfuncasm_map[0x11] = new rule("mthi", 0,0x11, "rs", 'r');
	asm_map["mtlo"] = disfuncasm_map[0x13] = new rule("mtlo", 0,0x13, "rs", 'r');
	asm_map["or"] = disfuncasm_map[0x25] = new rule("or", 0,0x25, "rd,rs,rt", 'r');
	asm_map["sll"] = disfuncasm_map[0x00] = new rule("sll", 0,0x00, "rd,rt,shamt", 'r');
	asm_map["sllv"] = disfuncasm_map[0x04] = new rule("sllv", 0,0x04, "rd,rt,rs", 'r');
	asm_map["slt"] = disfuncasm_map[0x2A] = new rule("slt", 0,0x2A, "rd,rt,rt", 'r');
	asm_map["sltu"] = disfuncasm_map[0x2B] = new rule("sltu", 0,0x2B, "rd,rs,rt", 'r');
	asm_map["sra"] = disfuncasm_map[0x03] = new rule("sra", 0,0x03, "rd,rt,shamt", 'r');
	asm_map["srav"] = disfuncasm_map[0x07] = new rule("srav", 0,0x07, "rd,rt,rs", 'r');
	asm_map["src"] = disfuncasm_map[0x01] = new rule("src", 0,0x01, "rd,rt,shamt", 'r');
	asm_map["srcv"] = disfuncasm_map[0x05] = new rule("srcv", 0,0x05, "rd,rt,rs", 'r');
	asm_map["srl"] = disfuncasm_map[0x02] = new rule("srl", 0,0x02, "rd,rt,shamt", 'r');
	asm_map["srlv"] = disfuncasm_map[0x06] = new rule("srlv", 0,0x06, "rd,rt,rs", 'r');
	asm_map["sub"] = disfuncasm_map[0x22] = new rule("sub", 0,0x22, "rd,rs,rt", 'r');
	asm_map["subu"] = disfuncasm_map[0x23] = new rule("subu", 0,0x23, "rd,rs,rt", 'r');
	asm_map["xor"] = disfuncasm_map[0x26] = new rule("xor", 0,0x26, "rd,rs,rt", 'r');
	asm_map["syscall"] = disfuncasm_map[0x0c] = new rule("syscall", 0, 0x0c, "", 'r');

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
	sections.push_back("");
	sections.push_back("text");
	sections.push_back("data");
	is_map_initialized = true;
}

Instr::Instr()
{
	_initialize();
}

void Instr::decode(unsigned int byte_code)
{
	initialize_map();
	_initialize();
	int int_length = sizeof(int) * 8;
	this->byte_code = byte_code;
	type = 0;
	opcode = byte_code >> (int_length - 6);
	byte_code <<= 6;
	target = ((int)byte_code) >> 6;
	rs = byte_code >> (int_length - 5);
	byte_code <<= 5;
	rt = byte_code >> (int_length - 5);
	
	byte_code <<= 5;
	imme = ((int)byte_code) >> (int_length - 16);
	rd = byte_code >> (int_length - 5);
	byte_code <<= 5;
	shamt = (byte_code >> (int_length - 5));
	byte_code <<= 5;
	func = byte_code >> (int_length - 6);
	map<unsigned int, rule*> *pointer_map = &disasm_map;
	int map_index = opcode;
	if (opcode == 0)
	{
		type = 'r';
		pointer_map = &disfuncasm_map;
		map_index = func;
	}
	else if (opcode == 2 || opcode == 3)
		type = 'j';
	else 
		type = 'i';

	map<unsigned int, rule*> &ref_map = *pointer_map;
	stringstream strstream;
	string tmp = "";
	string formal;
	if (ref_map[map_index] == NULL)
	{
		asm_code = "sll $0,$0,0";
		return ;
	}
	operation = ref_map[map_index]->operation;
	asm_code = operation + " ";

	if (ref_map[map_index] == NULL)
	{
		cerr << "error: The bytecode may not meet our demands" << endl;
		exit(1);
	}
	formal = ref_map[map_index]->formal;
	int index = formal.find("rs");
	if (index != string::npos)
	{
		strstream << "$" << rs;
		strstream >> tmp;
		strstream.clear();
		formal.replace(index, 2, tmp);
	}
	index = formal.find("rt");
	if (index != string::npos)
	{
		strstream << "$" << rt;
		strstream >> tmp;
		strstream.clear();
		formal.replace(index, 2, tmp);
	}
	index = formal.find("rd");
	if (index != string::npos)
	{
		strstream << "$" << rd;
		strstream >> tmp;
		strstream.clear();
		formal.replace(index, 2, tmp);
	}
	index = formal.find("shamt");
	if (index != string::npos)
	{
		strstream << shamt;
		strstream >> tmp;
		strstream.clear();
		formal.replace(index, 5, tmp);
	}
	index = formal.find("imme");
	if (index != string::npos)
	{
		strstream << imme;
		strstream >> tmp;
		strstream.clear();
		formal.replace(index, 4, tmp);
	}
	index = formal.find("target");
	if (index != string::npos)
	{
		strstream << target;
		strstream >> tmp;
		strstream.clear();
		formal.replace(index, 6, tmp);
	}
	asm_code += formal;
}

Instr::Instr(unsigned int byte_code)
{
	decode(byte_code);
}

void Instr::decode(string asm_code)
{
	initialize_map();
	_initialize();
	int ind;
	stringstream strstream;
	asm_code = trim(asm_code);
	if (asm_code.size() == 0)
	{
		section = "__empty";
		return;
	}
	section =  cursect;
	this->asm_code = asm_code;
	if ( asm_code[0] == '.' )
	{
		if ((ind = asm_code.find(".origin")) != string::npos)
		{
			is_origin = true;
			strstream << asm_code.substr(ind + 7) << endl;
			strstream >> address;
			strstream.clear();
			return;
		}
		if ((ind = asm_code.find(".end")) != string::npos)
		{
			is_end = true;
			string tmp = trim(asm_code.substr(ind + 4));
			assert(islegallabel(tmp));
			label = tmp;
			has_label = true;
			return;
		}
		if ((ind = asm_code.find(".align")) != string::npos)
		{
			label = ".align";
			is_align = true;
			string tmp = trim(asm_code.substr(ind + 6));
			strstream << tmp;
			int align;
			strstream >> align;
			strstream.clear();
			data_length = 1;
			while (align)
			{
				--align;
				data_length <<= 1;
			}
			return;
		}
		string sec = asm_code.substr(1);
		if (find(sections.cbegin(), sections.cend(), sec) == sections.cend())
			sections.push_back(sec);
		cursect = section = sec;
		return ;
	}

	if ( (ind = asm_code.find(".2byte")) != string::npos)
	{
		label = trim(asm_code.substr(0, ind));
		string tmp = trim(asm_code.substr(ind + 6));
		data_table = split(tmp);
		data_type = ".2byte";
		data_length = 2;
		is_data = true;
		is_label = true;
	}
	else if((ind = asm_code.find(".4byte")) != string::npos)
	{
		label = trim(asm_code.substr(0, ind));
		string tmp = trim(asm_code.substr(ind + 6));
		data_table = split(tmp);
		data_type = ".4byte";
		data_length = 4;
		is_data = true;
		is_label = true;
	}
	else if	((ind = asm_code.find(".8byte")) != string::npos )
	{
		label = trim(asm_code.substr(0, ind));
		string tmp = trim(asm_code.substr(ind + 6));
		data_table = split(tmp);
		data_type = ".8byte";
		data_length = 8;
		is_data = true;
		is_label = true;
	}
	else if ((ind = asm_code.find(".space")) != string::npos )
	{
		label = trim(asm_code.substr(0, ind));
		string tmp = trim(asm_code.substr(ind + 6));
		strstream << tmp;
		int times;
		strstream >> times;
		strstream.clear();
		for (int i = 0; i != times; ++i)
			data_table.push_back("0");
		data_length = 2;
		is_data = true;
		is_label = true;
	}

	if (is_data && is_label)
	{
		label_count = 0;
		for (vector<string>::iterator it = data_table.begin(); it != data_table.end();
			++it)
		{
			string tmp = *it;
			if (!isnumber(tmp))
			{
				++label_count;
				label_map[tmp]++;
				has_label = true;
			}
		}
		return ;
	}

	if ( (ind = asm_code.find(":")) != string::npos )
	{
		is_label = true;
		label = asm_code.substr(0, ind);
		return;
	}
	
	is_instr = true;
	string operater;
	strstream << asm_code;
	strstream >> operation;
	operater = dump(strstream);
	strstream.clear();

	assert (asm_map[operation] != NULL);
	type = asm_map[operation]->type;
	if (type == 'r')
	{
		opcode = 0x00;
		func = asm_map[operation]->func;
	}
	else
		opcode = asm_map[operation]->opcode;

	string formal = asm_map[operation]->formal;
	string ftmp;
	string otmp;
	string tmp;
	int lfindex = 0;
	int loindex = 0;
	int findex;
	int oindex;
	operater = replace(operater, " ", "");
	operater = replace(operater, "(", ",");
	formal = replace(formal, "(", ",");
	operater = replace(operater, ")", "");
	formal = replace(formal, ")", "");
	operater += ",";
	formal += ",";
	while ((findex = formal.find(',',lfindex)) != string::npos)
	{
		oindex = operater.find(',', loindex);
		assert (oindex != string::npos);

		ftmp = formal.substr(lfindex, findex - lfindex);
		otmp = operater.substr(loindex, oindex - loindex);
		if (ftmp == "rs")
		{
			assert(!(otmp[0] != '$' || reg_map[otmp] == ""));
			otmp = reg_map[otmp];
			strstream << otmp.substr(1);
			strstream >> rs;
			strstream.clear();
		}
		else if (ftmp == "rt")
		{
			assert(!(otmp[0] != '$' || reg_map[otmp] == ""));
			otmp = reg_map[otmp];
			strstream << otmp.substr(1);
			strstream >> rt;
			strstream.clear();
		}
		else if (ftmp == "rd")
		{
			assert(!(otmp[0] != '$' || reg_map[otmp] == ""));
			otmp = reg_map[otmp];
			strstream << otmp.substr(1);
			strstream >> rd;
			strstream.clear();
		}
		else if (ftmp == "shamt")
		{
			assert(isnumber(otmp));
			strstream << otmp;
			strstream >> shamt;
			strstream.clear();
		}
		else if (ftmp == "imme")
		{
			if (islegallabel(otmp))
			{
				label = otmp;
				has_label = true;
			}
			else
			{
				assert(isnumber(otmp));
				strstream << otmp;
				strstream >> imme;
				strstream.clear();
			}
		}
		else if (ftmp == "target")
		{
			if (islegallabel(otmp))
			{
				label = otmp;
				has_label = true;
			}
			else
			{
				assert(isnumber(otmp));
				strstream << otmp;
				strstream >> imme;
				strstream.clear();
			}
		}
		else if (operation != "syscall")
			assert(1 == 0);
		lfindex = findex + 1;
		loindex = oindex + 1;
	}
}

Instr::Instr(string asm_code)
{
	decode(asm_code);
}

string Instr::getAsmCode() const 
{
	return asm_code;
}

unsigned int Instr::getByteCode()
{
	byte_code = 0;
	byte_code |= (opcode & 63) << 26;
	switch (type)
	{
	case 'r':
		byte_code |= (rs & 31) << 21;
		byte_code |= (rt & 31) << 16;
		byte_code |= (rd & 31) << 11;
		byte_code |= (shamt & 31)<< 6;
		byte_code |= (func & 63) << 0;
		break;
	case 'i':
		byte_code |= (rs & 31) << 21;
		byte_code |= (rt & 31) << 16;
		byte_code |= (imme & ((1 << 16) - 1));
		break;
	case 'j':
		byte_code |= (target & ((1 << 26) - 1));
		break;
	default:
		break;
	}
	return byte_code;
}

void Instr::sort(List<Instr> &list)
{
	List<Instr> classes[10];
	List<Instr>::iterator lit;
	sections.push_back("__empty");
	for (lit = list.begin(); lit != list.end() ; )
	{
		for (int i = 0; i != sections.size(); ++i)
		{
			if (sections[i] == lit->elem.section)
			{
				classes[i].push_back(lit);
				break;
			}
		}
		
	}
	for (int i = 0; i != sections.size() - 1; ++i)
	{
		list.push_back(classes[i]);
	}
}

void Instr::resolveAddr(List<Instr> & list)
{
	bool isinend = false;
	string endlabel = "";
	List<Instr>::iterator it = list.begin();
	unsigned int curAddr = 0;
	while (it != list.end())
	{
		Instr &instr = it->elem;
		if (isinend)
		{
			if (instr.is_label && endlabel == instr.label)
				isinend = false;
			else
			{
				list.erase(it);
				continue;
			}
		}
		if (instr.is_end)
		{
			endlabel = instr.label;
			isinend = true;
			list.erase(it);
			continue;
		}
		if (instr.is_data)
			instr.address = curAddr;
		else if (instr.is_instr || instr.is_label)
			instr.address = curAddr / 4 * 4 + ((curAddr % 4)? 4: 0);

		if (instr.is_origin)
			curAddr = instr.address;
		else if (instr.is_data)
			curAddr += instr.data_length * instr.data_table.size();
		else if (instr.is_align)
				curAddr = curAddr / instr.data_length * instr.data_length 
				+ ((curAddr % instr.data_length)? instr.data_length: 0);
		else if (instr.is_instr)
			curAddr += 4;
		it = it->next;
	}
}

void Instr::_resolvelabel(Instr &label, Instr &haslabel)
{
	if (haslabel.is_instr)
	{
		if (label.is_data)
		{
			assert(haslabel.type == 'i');
			haslabel.imme = label.address;
		}
		else
		{
			if (haslabel.operation[0] == 'b')//branch instrs 
			{
				haslabel.imme = (label.address - haslabel.address) / 4 - 1;
				assert((label.address - haslabel.address) % 4 == 0);
			}
			else if (haslabel.type == 'i')//i-type instrs except branch
			{
				haslabel.imme = label.address / 2;
			}
			else if(haslabel.type == 'j' )//j-type
			{
				haslabel.target = label.address / 2;
			}
		}
	}
	else if (haslabel.is_data)
	{
		for (auto it = haslabel.label_map.begin();
			it != haslabel.label_map.end(); ++it)
		{
			if (it->first == label.label)
			{
				haslabel.label_count -= it->second;
				it->second = label.address;
			}
		}
	}
}

void Instr::resolveLabel(List<Instr> &list)
{
	List<Instr *> labels;
	List<Instr *> haslabels;
	List<Instr>::iterator it = list.begin();
	List<Instr *>::iterator lit, hit;
	for ( ; it != list.end(); it = it->next)
	{
		Instr &instr = it->elem;
		if (instr.is_label)
		{
			labels.push_back(&instr);
			for (hit = haslabels.begin(); hit != haslabels.end(); )
			{
				Instr &haslabel = *(hit->elem);
				if (( haslabel.is_instr && haslabel.label == instr.label )||( haslabel.is_data 
					&& haslabel.label_map.find(instr.label) != haslabel.label_map.end() ))
				{
					_resolvelabel(instr, haslabel);
					if (haslabel.is_instr || haslabel.is_data && haslabel.label_count == 0)
						haslabels.erase(hit);
					else 
						hit = hit->next;
				}
				else
					hit = hit->next;
			}
		}
		if (instr.has_label)
		{
			for (lit = labels.begin(); lit != labels.end(); lit = lit->next)
			{
				Instr &label = *(lit->elem);
				if (instr.is_instr && label.label == instr.label || instr.is_data 
					&& instr.label_map.find(instr.label) != instr.label_map.end())
				{
					_resolvelabel(label, instr);
					if (instr.is_instr || instr.is_data && instr.label_count == 0)
						break;
				}
			}
			if (lit == labels.end())
				haslabels.push_back(&instr);
		}
		//cout << instr.asm_code << endl;
		assert(haslabels.check() == true);
	}
	if (!haslabels.isempty())
	{
		string error;
		for (hit = haslabels.begin(); hit != haslabels.end(); hit = hit->next)
		{
			error += "label \"" + hit->elem->label + "\" in \"" + hit->elem->asm_code 
				+ " cannot be resolved\n";
		}
		throw Exception(error, "");
	}
}

void Instr::outputInstrs(List<Instr> &list, ofstream & fout)
{
	List<Instr>::iterator it = list.begin();
	for ( ; it != list.end(); it = it->next)
	{
		Instr &instr = it->elem;
		if (instr.is_instr)
		{
			fout.seekp(instr.address, fstream::beg);
			unsigned byte_code = instr.getByteCode();
			char *b = (char *)&byte_code;
			fout << b[3] << b[2] << b[1] << b[0];
		}
		else if (instr.is_data)
		{
			fout.seekp(instr.address, fstream::beg);
			for (vector<string>::iterator it = instr.data_table.begin();
				it != instr.data_table.end(); ++it)
			{
				string tmp = *it;
				long long t;
				if (isnumber(tmp))
				{
					stringstream strstream;
					strstream << tmp;
					strstream >> t;
				}
				else
				{
					t = instr.label_map[tmp];
				}
				char *b = (char *)&t;
				for (int i = 0; i != instr.data_length; ++i)
				{
					fout << b[instr.data_length - i - 1];
				}	
			}
		}
	}
}