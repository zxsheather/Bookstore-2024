//Utils.cpp
//The implementation of the functions in Utils.hpp.

#include "Utils.hpp"
#include "Exception.hpp"
#include<vector>
#include<string>
#include<sstream>

std::vector<std::string> CommandParser(std::string command,char delimiter){
  std::vector<std::string> result;
  std::stringstream ss(command);
  std::string temp;
  while(getline(ss,temp,delimiter)){
    result.push_back(temp);
  }
  return result;
}

bool is_integer(const std::string &s){
  for(int i=0;i<s.size();i++){
    if(i==0&&s[i]=='-'){
      continue;
    }
    if(s[i]<'0'||s[i]>'9'){
      return false;
    }
  }
  return true;
}

bool is_number(const std::string &s){
  bool point=false;
  for(int i=0;i<s.size();i++){
    if(i==0&&s[i]=='-'){
      continue;
    }
    if(s[i]=='.'&&!point){
      point=true;
      continue;
    }
    if(s[i]<'0'||s[i]>'9'){
      return false;
    }
  }
  return true;
}

bool is_positive_integer(const std::string &s){
	if(s[0]=='0'){
		return false;
	}
  for(int i=0;i<s.size();i++){
    if(s[i]<'0'||s[i]>'9'){
      return false;
    }
  }
  
  return true;
}

bool is_positive_number(const std::string &s){
  bool point=false;
	if(s[0]=='0'){
		return false;
	}
  for(int i=0;i<s.size();i++){
    if(s[i]=='.'&&!point&&i!=0){
      point=true;
      continue;
    }
    if(s[i]<'0'||s[i]>'9'){
      return false;
    }
  }
  return true;
}

bool is_privilege(const std::string &s){
  if(s.size()!=1){
    return false;
  }
  if(s[0]<'0'||s[0]>'9'){
    return false;
  }
  int privilege=std::stoi(s);
  if(privilege==1||privilege==3||privilege==7){
    return true;
  }
  return false;
}

std::vector<std::pair<std::string,std::string>> Modification_Parser(const std::vector<std::string> &input,const std::string &ISBN_){
	bool visit[5]={false,false,false,false,false};
	std::vector<std::pair<std::string,std::string>> result;
	for(int i=0;i<input.size();++i){
		int index=0;
		std::string order_="";
		while(index<input[i].size()&&input[i][index]!='='){
			order_+=input[i][index];
			index++;
		}
		if(index==input[i].size()||index==input[i].size()-1){
			throw InvalidOpertionException(); 
		}
		index++;
		
		if(order_=="-ISBN"){
			if(visit[0]||input[i].substr(index)==ISBN_){
				throw InvalidOpertionException();
			}
			visit[0]=true;
			result.push_back(std::make_pair(order_,input[i].substr(index)));
		}else if(order_=="-name"){
			if(visit[1]){
				throw InvalidOpertionException();
			}
			visit[1]=true;
			result.push_back(std::make_pair(order_,remove_quote(input[i].substr(index))));
		}else if(order_=="-author"){
			if(visit[2]){
				throw InvalidOpertionException();
			}
			visit[2]=true;
			result.push_back(std::make_pair(order_,remove_quote(input[i].substr(index))));
		}else if(order_=="-keyword"){
			if(visit[3]){
				throw InvalidOpertionException();
			}
			
			std::vector<std::string> keyword=CommandParser(remove_quote(input[i].substr(index)),'|');
			std::sort(keyword.begin(),keyword.end());
			for(int j=1;j<keyword.size();++j){
				if(keyword[j]==keyword[j-1]){
					throw InvalidOpertionException();
				}
			}
			visit[3]=true;
			result.push_back(std::make_pair(order_,remove_quote(input[i].substr(index))));
		}else if(order_=="-price"){
			if(visit[4]){
				throw InvalidOpertionException();
			}
			if(!is_positive_number(input[i].substr(index))){
				throw InvalidOpertionException();
			}
			visit[4]=true;
			result.push_back(std::make_pair(order_,input[i].substr(index)));
		}else{
			throw InvalidOpertionException();
		}
	}
	return result;
}

std::string remove_quote(const std::string &s){
  if(s.size()<2){
	throw InvalidOpertionException();
  }
  if(s[0]=='"'&&s[s.size()-1]=='"'){
	return s.substr(1,s.size()-2);
  }else{
	throw InvalidOpertionException();
  }
}

std::string intToFixedString(int value,int length){
  std::string result=std::to_string(value);
  while(result.size()<length){
	result="0"+result;
  }
  return result;
}

