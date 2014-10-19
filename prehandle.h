#ifndef _PREHANDLE_H_
#define _PREHANDLE_H_
#include <string>
#include <map>

//mainly deal with dup, equ, empty line;
class Prehandle{
public:
	Prehandle();
	std::string decode(std::string) throw(...);
private:
	struct PseudoRule{
		PseudoRule(std::string pseudo, std::string format)
		{
			this->pseudo = pseudo;
			this->format = format;
		}
		std::string pseudo;
		std::string format;
	};
	std::map <std::string, std::string> reg_map;
	std::map <std::string, std::string> format_map;
	std::map <std::string, PseudoRule *> pseudo_map;
	std::map <std::string, std::string> const_map;
	bool isincomment;
	bool isafterdata;
	bool hasdata;
	std::string multilinedata;
};

#endif