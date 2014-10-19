#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_
#include <string>
#include <vector>
#include <deque>
#include <map>

struct mix{
	mix()
	{
		isnumber = false;
		isoperation = false;
	}
	mix(int number)
	{
		this->number = number;
		isnumber = true;
		isoperation = false;
	}
	mix(std::string operation)
	{
		this->operation = operation;
		isoperation = true;
		isnumber = false;
	}
	bool isoperation ;
	bool isnumber ;
	int number;
	std::string operation;
};

class Calculator{
public:
	Calculator();
	Calculator(std::string str);
	void decode(std::string str);
	int exec();
	
	static bool isequation(std::string str);
	static int toInt(std::string str);
private:
	std::string equation;
	std::deque<mix> mixqueue;
	std::map<std::string, int> level_map;
};

#endif 