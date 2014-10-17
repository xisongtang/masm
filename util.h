#ifndef _UTIL_H_
#define _UTIL_H_
#include <string>
#include <sstream>
#include <vector>
#include <regex>

std::string replace(std::string source, std::string e, std::string c);
std::string dump(std::stringstream &strstream);
bool isnumber(const std::string &number);
bool islegallabel(const std::string &label);
std::string tolower(std::string);
bool has(std::string, std::string, bool whole = true);
std::string trim(std::string, std::string str="\\s");
std::vector<long long> split(std::string, std::string str=",");
std::smatch find(std::string, std::string);
std::string decodesuffix(std::string str);
#endif