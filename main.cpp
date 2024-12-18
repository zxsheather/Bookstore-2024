//main.cpp
//The entrance of the program.

#include<iostream>
#include<stack>
#include<string>
#include<fstream>
#include <utility>
#include<vector>
#include<sstream>
#include "src/Account.hpp"
#include "src/Key-Value-Database.hpp"
#include "src/Exception.hpp"

std::vector<std::string> CommandParser(std::string command,char delimiter){
  std::vector<std::string> result;
  std::stringstream ss(command);
  std::string temp;
  while(getline(ss,temp,delimiter)){
    result.push_back(temp);
  }
  return result;
}

int main() {
  freopen("/Users/apple/Desktop/Programming/ACMOJ/BookStore/Bookstore-2024/bookstore-testcases/basic/testcase3.in","r",stdin);
  freopen("output.txt","w",stdout);
  std::stack <std::pair<std::string,int>> loginStack;
  bool loginFlag = false;
  int login_privilege = 0;
  std::string command;
  std::ifstream temp("account_file_index.dat", std::ios::binary);
  
  bool flag=false;
  if(!temp){
    flag=true;
  }else{
    if(temp.peek()==std::ifstream::traits_type::eof()){
      flag=true;
    }
    temp.close();
  }
  File_Storage<User_Info> account_file("account_file");
  account_file.Initialize();
  if(flag||true){
    User_Info user_info;
    user_info.privilege=7;
    strcpy(user_info.userid,"root");
    strcpy(user_info.password,"sjtu");
    strcpy(user_info.username,"root");
    account_file.Insert(user_info.userid,user_info);
  }
  while(true) {
    try{
      std::string line;
      getline(std::cin,line);
      std::vector<std::string> input=CommandParser(line,' ');
      std::string command=input[0];
      input.erase(input.begin());
      if(command=="quit"||command=="exit") {
        exit(0);
      }else if(command=="su"){
        if(input.size()>=3||input.size()==0){
          throw InvalidOpertionException();
        }
        std::string user_id=input[0];
        std::vector<User_Info> result=account_file.Find(user_id);
        if(result.size()==0){
          throw InvalidOpertionException();
        }else{
          User_Info user_info=result[0];
          if(login_privilege>user_info.privilege&&loginFlag){
            loginStack.push(std::make_pair(user_info.userid, user_info.privilege));
            loginFlag=true;
            login_privilege=user_info.privilege;
          }else {
            if(input.size()==1){
              throw InvalidOpertionException();
            }
            if(user_info.password==input[1]){
              loginStack.push(std::make_pair(user_info.userid, user_info.privilege));
              loginFlag=true;
              login_privilege=user_info.privilege;
            }else{
              throw InvalidOpertionException();
            }
          }
        } 
      }else if(command=="logout"){
        if(input.size()!=0){
          throw InvalidOpertionException();
        }
        if(loginStack.empty()){
          throw InvalidOpertionException();
        }else{
          loginStack.pop();
          if(loginStack.empty()){
            loginFlag=false;
            login_privilege=0;
          }else{
            login_privilege=loginStack.top().second;
          }
        }
      }else if(command=="register"){
        if(input.size()!=3){
          throw InvalidOpertionException();
        }
        User_Info user_info(input[0],input[1],input[2],1);
        account_file.Insert(user_info.userid,user_info);
      }else if(command=="passwd"){
        if(login_privilege==7&&input.size()==2){
          std::vector<User_Info> result=account_file.Find(input[0]);
          if(result.size()==0){
            throw InvalidOpertionException();
          }else{
            User_Info user_info=result[0];
            User_Info new_user_info(user_info.userid,input[1],user_info.username,user_info.privilege);
            account_file.Update(user_info.userid,user_info,new_user_info);
          }
        }else if(input.size()==3){
          std::vector<User_Info> result=account_file.Find(input[0]);
          if(result.size()==0){
            throw InvalidOpertionException();
          }else{
            User_Info user_info=result[0];
            if(user_info.password==input[1]){
              User_Info new_user_info(user_info.userid,input[2],user_info.username,user_info.privilege);
              account_file.Update(user_info.userid,user_info,new_user_info);
            }else{
              throw InvalidOpertionException();
            }
          }
        }else{
          throw InvalidOpertionException();
        }
      }else if(command=="useradd"){
        if(input.size()!=4){
          throw InvalidOpertionException();
        }else{
          std::vector<User_Info> result=account_file.Find(input[0]);
          if(result.size()!=0){
            throw InvalidOpertionException();
          }else{
            int privilege=std::stoi(input[2]);
            if(login_privilege<=privilege){
              throw InvalidOpertionException();
            }
            User_Info user_info(input[0],input[1],input[3],privilege);
            account_file.Insert(user_info.userid,user_info);
          }
        }
      }else if(command=="delete"){
        if(login_privilege!=7){
          throw InvalidOpertionException();
        }
        if(input.size()!=1){
          throw InvalidOpertionException();
        }else{
          std::vector<User_Info> result=account_file.Find(input[0]);
          if(result.size()==0){
            throw InvalidOpertionException();
          }else{
            std::stack<std::pair<std::string, int>> tempStack = loginStack;
            while (!tempStack.empty()) {
              auto it = tempStack.top();
              tempStack.pop();
              if(it.first==input[0]){
                throw InvalidOpertionException();
              }
            }
            account_file.Delete(input[0],result[0]);
          }
        }
      }else{
        throw InvalidOpertionException();
      }
    }catch(InvalidOpertionException &e){
      std::cout<<e.what()<<std::endl;
    }
  }
}
