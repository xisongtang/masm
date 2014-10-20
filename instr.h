#ifndef _INSTR_H_
#define _INSTR_H_
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "list.h"

struct rule
{
	rule(std::string operation, unsigned int opcode, unsigned func, std::string formal, char type)
	{
		this->operation = operation;
		this->opcode = opcode;
		this->func = func;
		this->formal = formal;
		this->type = type;
	}
	char type;
	std::string operation;
	unsigned int opcode;
	unsigned int func;
	std::string formal;
};

class Instr{
public:
	Instr();
	Instr(std::string) ;
	Instr(unsigned int);
	void decode (std::string) ;
	void decode (unsigned int);

	std::string getAsmCode() const;
	unsigned int getByteCode();
	static void sort(List<Instr> &);
	static void resolveAddr(List<Instr> &);
	static void resolveLabel(List<Instr> &) throw(...);
	static void outputInstrs(List<Instr> &, std::ofstream &);
private:
	static std::map<std::string, rule*> asm_map;
	static std::map<unsigned int, rule*> disasm_map;
	static std::map<unsigned int, rule*> disfuncasm_map;
	static std::map<std::string, std::string> reg_map;
	static std::string cursect;
	static std::vector<std::string> sections;

	void _initialize();
	bool _mayhavelable();
	static void initialize_map();
	static bool is_map_initialized;
	static void _resolvelabel(Instr &instr, Instr &haslabel);

	unsigned int opcode;
	char type;
	unsigned int rs;
	unsigned int rt;
	unsigned int rd;
	unsigned int shamt;
	unsigned int func;
	unsigned int byte_code;
	int imme;
	int target;
	bool has_label;
	bool is_end;
	bool is_align;
	bool is_instr;
	bool is_label;
	bool is_origin;
	bool is_data;//.align is also a kind of data
	int data_length;
	std::string data_type;
	std::string section;
	int address;
	std::string operation;
	std::string label;
	std::string asm_code;
	std::string comment;
	std::vector<std::string> data_table;
	std::map<std::string, int>label_map;
	int label_count;
};

#endif 