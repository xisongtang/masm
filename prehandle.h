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
	std::map <std::string, std::string> const_map;
	bool isincomment;
	bool isafterdata;
	bool hasdata;
	std::string multilinecomment;
	std::string multilinedata;
	std::string commentafterdata;
};

#endif