#include "control.h"
#ifdef DISASM
#include "instr.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
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
	unsigned int byte_code;
	char tmp[4];
	if (argc >= 3)
		foutname = args[2];
	ifstream fin(finname.c_str(), ifstream::binary);
	if (!fin)
	{
		cerr << "error: can't find the input file" << endl;
		system("pause");
		return 2;
	}
	ofstream fout(foutname.c_str());
	fin.seekg(0, fin.end);
	int length = fin.tellg();
	int instr_number = length / 4;
	fin.seekg(0, fin.beg);
	for (int i = 0; i != instr_number; ++i)
	{
		byte_code = 0;
		fin.read(tmp, 4);
		byte_code |= ((unsigned int)tmp[3] & 255);
		byte_code |= ((unsigned int)tmp[2] & 255) << 8;
		byte_code |= ((unsigned int)tmp[1] & 255) << 16;
		byte_code |= ((unsigned int)tmp[0] & 255) << 24;
		Instr instr(byte_code);
		//cout << hex << byte_code << endl;
		fout << instr.getAsmCode() << endl;
	}
	system("pause");
}
#endif