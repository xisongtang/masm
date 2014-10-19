#include "calculator.h"
#include "util.h"
#include "exception.h"

#include <string>
#include <vector>
#include <regex>

using namespace std;

Calculator::Calculator()
{
	level_map["~"] = 5;
	level_map["*"] = level_map["/"] = 4;
	level_map["+"] = level_map["-"] = 3;
	level_map["<<"] = level_map[">>"] = level_map[">>>"] = 2;
	level_map["&"] = level_map["|"] = level_map["^"] = 1;
	level_map["#"] = 0;
}

Calculator::Calculator(string str)
{
	level_map["~"] = 5;
	level_map["*"] = level_map["/"] = 4;
	level_map["+"] = level_map["-"] = 3;
	level_map["<<"] = level_map[">>"] = level_map[">>>"] = 2;
	level_map["&"] = level_map["|"] = level_map["^"] = 1;
	level_map["#"] = 0;
	decode(str);
}

void Calculator::decode(string str)
{
	equation = str;
	vector<string> opstack;
	opstack.push_back("#");
	string num;
	string op; 
	str = trim(str);
	str = replace (str, "\t", "");
	str = replace (str, " ", "");
	str = replace (str, "<<", "z");
	str = replace (str, ">>>", "s");
	str = replace (str, ">>", "y");
	str += "#";
	string otmp;
	mix mtmp;
	int i = 0;
	while (true)
	{
		if (str[i] <= '9' && str[i] >= '0' || str[i] <= 'F' && str[i] >= 'A' ||
			str[i] <= 'f' && str[i] >= 'a' || str[i] == 'x' || str[i] == 'X' ||
			((str[i] == '-' || str[i] == '+') && (i == 0 || str[i] == '(')))
		{
			while (str[i] <= '9' && str[i] >= '0' || str[i] <= 'F' && str[i] >= 'A' ||
			str[i] <= 'f' && str[i] >= 'a' || str[i] == 'x' || str[i] == 'X' ||
			((str[i] == '-' || str[i] == '+') && (i == 0 || str[i] == '(')))
				num += str[i++];
			mix mtmp(toInt(num));
			mixqueue.push_back(mtmp);
			num = "";
			continue;
		}
		op = str[i++];
		if (op == "")
			throw Exception("Invalid equation", equation);
		regex re("[+\\-\\*/%~^&\\|\\(\\)zys#]");
		if (!regex_match(op, re))
			throw Exception("Invalid operation of in the equation:" + op, equation);
		if (op == ")")
		{
			while (opstack.back() != "(")
			{
				if (opstack.back() == "#")
					throw Exception("Unmatched parentheses in equation", str);
				mix mtmp(opstack.back());
				mixqueue.push_back(mtmp);
				opstack.pop_back();
			}
			opstack.pop_back();
		}
		else if (opstack.back() == "(" || op == "(" )
			opstack.push_back(op);
		else if(level_map[op] <= level_map[opstack.back()] && opstack.back() != "#")
		{
			do
			{
				mix mtmp(opstack.back());
				mixqueue.push_back(mtmp);
				opstack.pop_back();
			}while (level_map[op] <= level_map[opstack.back()] && 
				opstack.back() != "#" && opstack.back() != "(");
			if (op == "#" && opstack.back() == "#")
				break;
			opstack.push_back(op);
		}
		else 
			opstack.push_back(op);
	}
}

int Calculator::exec()
{
	vector<int> stack;
	while (!mixqueue.empty())
	{
		if (mixqueue.front().isnumber)
		{
			stack.push_back(mixqueue.front().number);
			mixqueue.pop_front();
		}
		else 
		{
			char op = mixqueue.front().operation[0];
			mixqueue.pop_front();
			int op1,op2;
			if (stack.empty())
				throw Exception("Invalid Equation", equation);
			op2 = stack.back();
			stack.pop_back();
			if (stack.empty())
				throw Exception("Invalid Equation", equation);
			op1 = stack.back();
			stack.pop_back();
			switch(op)
			{
			case '+':stack.push_back(op1 + op2);break;
			case '-':stack.push_back(op1 - op2);break;
			case '*':stack.push_back(op1 * op2);break;
			case '/':stack.push_back(op1 / op2);break;
			case '%':stack.push_back(op1 % op2);break;
			case '~':stack.push_back(op1);stack.push_back(~op2);break;
			case 'z':stack.push_back(op1 << op2);break;
			case 'y':stack.push_back((unsigned)op1 >> op2);break;
			case 's':stack.push_back(op1 >> op2);break;
			case '&':stack.push_back(op1 & op2);break;
			case '|':stack.push_back(op1 | op2);break;
			case '^':stack.push_back(op1 ^ op2);break;
			default:break;
			}
			/*
				cout << op << ":";
				for (int i = 0; i != stack.size(); ++i)
					cout << stack[i] << " ";
				cout << endl;
				*/
		}
	}
	return stack[0];
}

int Calculator::toInt(string str)
{
	string org_str = str;
	int scale = 10;
	int sign = 1;
	str = trim(str);
	if (!isnumber(str))
		throw Exception("Invalid number format:" + org_str, "");
	if (str[0] == '-')
	{
		sign = -1;
		str = str.substr(1);
	}
	else if (str[0] == '+')
		str = str.substr(1);

	if (str.substr(0,2) == "0x")
	{
		str = str.substr(2);
		scale = 16;
	}
	else if(str.substr(0,1) == "0")
	{
		str = str.substr(1);
		scale = 8;
	}
	else 
		;
	int number = 0;
	for (int i = 0; i != str.size(); ++i)
	{
		number *= scale;
		int tmp = 0;
		if (str[i] >= 'A' && str[i] <= 'F')
			tmp = str[i] - 'A' + 10;
		else if (str[i] >= 'a' && str[i] <= 'z')
			tmp = str[i] - 'a' + 10;
		else if (str[i] >= '0' && str[i] <= '9')
			tmp = str[i] - '0';
		number += tmp;
	}
	return number * sign;
}

bool Calculator::isequation(string str)
{
	return has(str, "[+\\-\\*/%~&\\|^<>\\(\\)]");
}