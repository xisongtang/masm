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


int main(int argc, char *args[])
{
	if (argc == 1)
	{
		cerr << "error: Please enter the file name" << endl;
		return 1;
	}
	string finname = args[1];
	string foutname = "a.asm";
	string ftoutname = split(finname,".")[0] + ".obj";
	if (argc >= 3)
		foutname = args[2];
	
	char buffer[MAXLEN];
	string content;
	unsigned offset = 0;
	unsigned line = 0;
	unsigned lablenumber = 0;
	stringstream strstream;
	ifstream fin(finname);
	ofstream ftout(ftoutname);
	bool has_error = false;
	
	Prehandle prehandler;
	
	while (!fin.eof())
	{
		++line;
		fin.getline(buffer, MAXLEN);
		try
		{
			ftout << prehandler.decode(buffer);
		}
		catch (Exception &e)
		{
			e.setLine(line);
			e.printError();
			has_error = true;
		}
	}
	
	ftout.close();
	cout << "preproccess finished" << endl;
	if (has_error)
	{
		system("pause");
		exit(1);
	}
	fin.close();
	fin.open(ftoutname);
	
	List<Instr> instrs;
	
	while (!fin.eof())
	{
		fin.getline(buffer,MAXLEN);
		Instr cur;
		cur.decode(buffer);
		instrs.push_back(cur);
	}
	cout << "instrs decoding finished" << endl;
	Instr::sort(instrs);
	cout << "instrs' block sorting finished" << endl;
	Instr::resolveAddr(instrs);
	cout << "instrs' address resolved" << endl;
	try{
		Instr::resolveLabel(instrs);
		cout << "labels resolved" <<endl;
	}catch(Exception e)
	{
		e.printError();
		throw e;
	}

	ofstream fout(foutname, ofstream::binary);
	Instr::outputInstrs(instrs, fout);
	cout << "binary file output finished" << endl;
	fout.close();
	system("pause");
}

#endif