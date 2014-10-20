#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_
#include "util.h"
#include <string>
#include <iostream>
#include <sstream>

class Exception {
public:
	Exception(std::string error, std::string code)
	{
		this->error = replace(error, "\n", "\n\t");
		this->code = code;
		this->line = -1;
	}
	void setLine(int line)
	{
		this->line = line;
	}
	void printError()
	{
		std::stringstream strstream;
		strstream << "line " << line << ":";
		std::string lstr = strstream.str();
		lstr = dump(strstream);
		strstream.clear();
		strstream << code << "\n";
		std::string cstr = dump(strstream);
		strstream.clear();

		std::cerr << ((line != -1)?lstr:"") << ((code != "")?cstr:"") << "error:"
			<< error << std::endl;
	}
	std::string getError()const
	{
		return error;
	}
	Exception(){}
	~Exception(){}
	static std::string ion;//illegal operation name
	static std::string irn;//illegal register name
	static std::string is;//illegal shamt
	static std::string ii;//illegal immediate
	static std::string it;//illegal target
	static std::string ge;//grammar error
	static std::string iud;//invalid use of dump

private:
	std::string error;
	std::string code;
	int line;
};

#endif