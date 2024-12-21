//Utils.hpp
//Include some useful tools.

#ifndef UTILS_HPP
#define UTILS_HPP
#include<vector>
#include<string>

bool is_number(const std::string &s);

bool is_integer(const std::string &s);

bool is_positive_number(const std::string &s);

bool is_positive_integer(const std::string &s);

bool is_privilege(const std::string &s);

std::vector<std::string> CommandParser(std::string command,char delimiter);

std::vector<std::pair<std::string,std::string>> Modification_Parser(const std::vector<std::string> &input,const std::string &ISBN);

std::string remove_quote(const std::string &s);

std::string intToFixedString(int value,int length=6);

#endif