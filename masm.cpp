#include "control.h"
#ifdef MASM
#include "instr.h"
#include "prehandle.h"
#include "exception.h"
#include "list.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>

#define MAXLEN 0xffff
using namespace std;

Instr *instrs[MAXLEN];
int labels[MAXLEN];
map<string, unsigned> label_map;

int main(int argc, char *args[])
{
	if (argc == 1)
	{
		cerr << "error: Please enter the file name" << endl;
		return 1;
	}
	string finname = args[1];
	string foutname = "a.asm";
	if (argc >= 3)
		foutname = args[2];
	ifstream fin(finname);
	char buffer[MAXLEN];
	string content;
	unsigned offset = 0;
	unsigned line = 0;
	unsigned lablenumber = 0;
	stringstream strstream;
	Prehandle prehandler;
	while (!fin.eof())
	{
		fin.getline(buffer, MAXLEN);
		strstream << prehandler.decode(buffer);
	}
	fin.close();
	List<Instr> instrs;
	bool has_error = false;
	while (!strstream.eof())
	{
		++line;
		strstream.getline(buffer,MAXLEN);
		Instr cur;
		try{
			cur.decode(buffer);
			instrs.push_back(cur);
		} catch (Exception e){
			e.setLine(line);
			e.printError();
			has_error = true;
		}
	}
	if (has_error)
		exit(1);
	Instr::sort(instrs);
	Instr::resolveAddr(instrs);
	Instr::resolveLabel(instrs);

	ofstream fout(foutname, ofstream::binary);
	Instr::outputInstrs(instrs, fout);
	fout.close();
	system("pause");
}

#endif