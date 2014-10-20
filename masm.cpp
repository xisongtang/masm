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
	bool tmpoutput = false;
	bool finnamein = false;
	string finname = "", foutname = "a.exe";
	for (int i = 1; i != ((argc > 4)? 4: argc); ++i)
	{
		string tmp = args[i];
		if (tmp.size() > 1 && tmp[0] == '-')
		{
				if (tmp == "-t" || tmp == "-temp" )
					tmpoutput = true;
				else 
				{
					cerr << "unknown option: " + tmp << endl;
					return 0;
				}
		}
		else if (finnamein)
			foutname = tmp;
		else 
		{
			finnamein = true;
			finname = tmp;
		}
	}
	string ftoutname = split(finname,".")[0] + ".obj";
	if (!finnamein)
	{
		cerr << "no input file" << endl;
		return 0;
	}
	char buffer[MAXLEN];
	string content;
	unsigned offset = 0;
	unsigned line = 0;
	unsigned lablenumber = 0;
	stringstream strstream;
	ifstream fin(finname);
	ofstream ftout;
	if (tmpoutput)
		ftout.open(ftoutname);
	bool has_error = false;
	
	Prehandle prehandler;
	
	while (!fin.eof())
	{
		++line;
		fin.getline(buffer, MAXLEN);
		try
		{
			if (tmpoutput)
				ftout << prehandler.decode(buffer);
			else 
				strstream << prehandler.decode(buffer);
		}
		catch (Exception &e)
		{
			e.setLine(line);
			e.printError();
			if (tmpoutput)
				ftout << buffer << "#error: " << e.getError() << endl;
			has_error = true;
		}
	}
	if (tmpoutput)
		ftout.close();
	cout << "preproccess finished" << endl;
	if (has_error)
	{
		system("pause");
		exit(1);
	}
	fin.close();
	if (tmpoutput)
		fin.open(ftoutname);
	
	List<Instr> instrs;
	if (tmpoutput)
		while (!fin.eof())
		{
			fin.getline(buffer,MAXLEN);
			Instr cur;
			cur.decode(buffer);
			instrs.push_back(cur);
		}
	else
		while (!strstream.eof())
		{
			strstream.getline(buffer,MAXLEN);
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
		system("pause");
		exit(1);
	}

	ofstream fout(foutname, ofstream::binary);
	Instr::outputInstrs(instrs, fout);
	cout << "binary file output finished" << endl;
	fout.close();
	system("pause");
	return 0;
}

#endif