#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_
#include <string>
#include <iostream>

class Exception {
public:
	Exception(std::string error, std::string code)
	{
		this->error = error;
		this->code = code;
	}
	void setLine(int line)
	{
		this->line = line;
	}
	void printError()
	{
		std::cerr << "line " << line << ":" << code << std::endl << "error: " 
			<< error << std::endl;
	}
	Exception(){}
	~Exception(){}
	static std::string ion;//illegal operation name
	static std::string irn;//illegal register name
	static std::string is;//illegal shamt
	static std::string ii;//illegal immediate
	static std::string it;//illegal target
	static std::string ge;//grammar error
	static std::string iud;//invalid use of dup

private:
	std::string error;
	std::string code;
	int line;
};

#endif