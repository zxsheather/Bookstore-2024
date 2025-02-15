// main.cpp
// The entrance of the program.

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cassert>
//#include <chrono>

#include "src/Data.hpp"
#include "src/Exception.hpp"
#include "src/Key-Value-Database.hpp"
#include "src/Utils.hpp"


int main() {
  // long long time1=0;
  // long long time2=0;
  // long long book_file_time=0;
  // long long keyword_file_time=0;
  // long long author_file_time=0;
  // long long name_file_time=0;
  // freopen(
  //     "/Users/apple/Desktop/Programming/ACMOJ/BookStore/Bookstore-2024/bookstore-testcases/basic/testcase4.in",
  //     "r", stdin);
  // freopen(
  //     "/Users/apple/Desktop/Programming/ACMOJ/BookStore/Bookstore-2024/"
  //     "output.txt",
  //     "w", stdout);
  

  std::vector<Login_Info> loginStack;
  bool loginFlag = false;
  int login_privilege = 0;
  int operator_time = 100000;
  std::string command;
  std::ifstream temp("account_file_index.dat", std::ios::binary);

  bool flag = false;
  if (!temp) {
    flag = true;
  } else {
    if (temp.peek() == std::ifstream::traits_type::eof()) {
      flag = true;
    }
    temp.close();
  }
  File_Storage<User_Info> account_file("account_file");
  if (flag) {
    User_Info user_info;
    user_info.privilege = 7;
    strcpy(user_info.userid, "root");
    strcpy(user_info.password, "sjtu");
    strcpy(user_info.username, "root");
    account_file.Insert(user_info.userid, user_info);
  }
  File_Storage<ISBN_String> name_file("name_file");
  File_Storage<ISBN_String> author_file("author_file");
  File_Storage<ISBN_String> keyword_file("keyword_file");
  File_Storage<Book_Info> book_file("book_file");
  File_Storage<Transaction_Info> transaction_file("transaction_file");
  File_Storage<Operation> operation_file("operation_file");
  File_Storage<Operation> employee_operation_file("employee_operation_file");

  std::string line;
  bool includelinenumber = true;
  int linenumber=0;
  bool flag1=true;
  std::vector<Operation> temp_vector=operation_file.FindFirstN(1,flag1);
  if(flag1){
    operator_time=std::stoi(temp_vector[0].time)-1;
  }
  while (getline(std::cin, line)) {
    try {
      linenumber++;
      // if(linenumber==389){
      //   int a=1;
      // }
      std::vector<std::string> input = CommandParser(line, ' ');
      if(input.size()==0){
        continue;
      }
      std::string command = input[0];
      input.erase(input.begin());
      Operation operation;
      strcpy(operation.time, intToFixedString(operator_time).c_str());
      strcpy(operation.userid, loginStack.empty() ? "" : loginStack.back().user_id.c_str());
      operation.privilege = loginStack.empty() ? 0 : loginStack.back().privilege;
      strcpy(operation.operation, line.c_str());
      operation_file.Insert(operation.time, operation);
      if(login_privilege==3){
        employee_operation_file.Insert(operation.time, operation);
      }
      operator_time--;
      if (command == "quit" || command == "exit") {
        if(input.size()!=0){
          throw InvalidOperationException();
        }
        // std::cout<<"Time1: "<<time1<<std::endl;
        // std::cout<<"Time2: "<<time2<<std::endl;
        // std::cout<<"Book File Time: "<<book_file_time<<std::endl;
        // std::cout<<"Keyword File Time: "<<keyword_file_time<<std::endl;
        // std::cout<<"Author File Time: "<<author_file_time<<std::endl;
        // std::cout<<"Name File Time: "<<name_file_time<<std::endl;
        exit(0);
      } else if (command == "su") {
        if (input.size() >= 3 || input.size() == 0) {
          throw InvalidOperationException();
        }
        std::string user_id = input[0];
        if(!is_userID_or_password(user_id)){
          throw InvalidOperationException();
        }
        std::vector<User_Info> result = account_file.Find(user_id);
        if(input.size()==2&&(!is_userID_or_password(input[1]))){
          throw InvalidOperationException();
        }
        if (result.size() == 0) {
          throw InvalidOperationException();
        } else {
          User_Info user_info = result[0];
          if (login_privilege > user_info.privilege && loginFlag) {
            if(input.size()==2&&input[1]!=user_info.password){
              throw InvalidOperationException();
            }
            loginStack.emplace_back(user_info.userid, user_info.privilege,"");
            loginFlag = true;
            login_privilege = user_info.privilege;
          } else {
            if (input.size() == 1) {
              throw InvalidOperationException();
            }
            if (user_info.password == input[1]) {
              loginStack.emplace_back(user_info.userid, user_info.privilege,"");
              loginFlag = true;
              login_privilege = user_info.privilege;
            } else {
              throw InvalidOperationException();
            }
          }
        }
        std::cout<<"Login Success.\n";
      } else if (command == "logout") {
        if(login_privilege==0){
          throw InvalidOperationException();
        }
        if (input.size() != 0) {
          throw InvalidOperationException();
        }
        if (loginStack.empty()) {
          throw InvalidOperationException();
        } else {
          loginStack.pop_back();
          if (loginStack.empty()) {
            loginFlag = false;
            login_privilege = 0;
          } else {
            login_privilege = loginStack.back().privilege;
          }
        }
      } else if (command == "register") {
        if (input.size() != 3) {
          throw InvalidOperationException();
        }
        if(!is_userID_or_password(input[0])||!is_userID_or_password(input[1])||!is_username(input[2])){
          throw InvalidOperationException();
        }
        std::vector<User_Info> result = account_file.Find(input[0]);
        if (result.size() != 0) {
          throw InvalidOperationException();
        }
        Operation operation;
        strcpy(operation.time, intToFixedString(operator_time).c_str());
        strcpy(operation.userid, loginStack.empty() ? "" : loginStack.back().user_id.c_str());
        operation.privilege = loginStack.empty() ? 0 : loginStack.back().privilege;
        strcpy(operation.operation, line.c_str());
        operation_file.Insert(operation.time, operation);
        operator_time--;
        User_Info user_info(input[0], input[1], input[2], 1);
        account_file.Insert(user_info.userid, user_info);
      } else if (command == "passwd") {
        if (login_privilege == 0) {
          throw InvalidOperationException();
        }

        if (login_privilege == 7 && input.size() == 2) {
          if(!is_userID_or_password(input[0])||!is_userID_or_password(input[1])){
            throw InvalidOperationException();
          }
          std::vector<User_Info> result = account_file.Find(input[0]);
          if (result.size() == 0) {
            throw InvalidOperationException();
          } else {
            User_Info user_info = result[0];
            User_Info new_user_info(user_info.userid, input[1],
                                    user_info.username, user_info.privilege);
            account_file.Update(user_info.userid, user_info, new_user_info);
          }
        } else if (input.size() == 3) {
          if(!is_userID_or_password(input[0])||!is_userID_or_password(input[1])||!is_userID_or_password(input[2])){
            throw InvalidOperationException();
          }
          std::vector<User_Info> result = account_file.Find(input[0]);
          if (result.size() == 0) {
            throw InvalidOperationException();
          } else {
            User_Info user_info = result[0];
            if (user_info.password == input[1]) {
              User_Info new_user_info(user_info.userid, input[2],
                                      user_info.username, user_info.privilege);
              account_file.Update(user_info.userid, user_info, new_user_info);
            } else {
              throw InvalidOperationException();
            }
          }
        } else {
          throw InvalidOperationException();
        }
        std::cout<<"Password changed.\n";
      } else if (command == "useradd") {
        if(login_privilege<3){
          throw InvalidOperationException();
        }
        if (input.size() != 4) {
          throw InvalidOperationException();
        }
        if(!is_userID_or_password(input[0])||!is_userID_or_password(input[1])||!is_privilege(input[2])||!is_username(input[3])){
          throw InvalidOperationException();
        }
        std::vector<User_Info> result = account_file.Find(input[0]);
        if (result.size() != 0) {
          throw InvalidOperationException();
        } else {
          int privilege = std::stoi(input[2]);
          if (login_privilege <= privilege) {
            throw InvalidOperationException();
          }
          User_Info user_info(input[0], input[1], input[3], privilege);
          account_file.Insert(user_info.userid, user_info);
        }
        std::cout<<"User added.\n";
      } else if (command == "delete") {
        if (login_privilege != 7) {
          throw InvalidOperationException();
        }
        if (input.size() != 1) {
          throw InvalidOperationException();
        }
        if(!is_userID_or_password(input[0])){
          throw InvalidOperationException();
        }
        std::vector<User_Info> result = account_file.Find(input[0]);
        if (result.size() == 0) {
          throw InvalidOperationException();
        } else {
          std::vector<Login_Info> tempStack = loginStack;
          for (auto &it : tempStack) {
            if (it.user_id == input[0]) {
              throw InvalidOperationException();
            }
          }
          account_file.Delete(input[0], result[0]);
        }
        std::cout<<"User deleted.\n";
      } else if (command == "show") {
        if(input.size()>=1&&input[0]=="finance"){
          if(login_privilege<7){
            throw InvalidOperationException();
          }
          if(input.size()==1){
            std::vector<Transaction_Info> result=transaction_file.FindAll();
            double revenue=0;
            double expense=0;
            for(auto &i:result){
              if(i.total_price>0){
                revenue+=i.total_price;
              }else{
                expense-=i.total_price;
              }
            }
            //std::cout<<"Line"<<linenumber<<": ";
            std::cout<<std::fixed<<std::setprecision(2)<<"+ "<<revenue<<" - "<<expense<<"\n";
          }else if(input.size()==2){
            if(!is_quantity(input[1])){
              throw InvalidOperationException();
            }
            if(input[1]=="0"){
              //std::cout<<"Line"<<linenumber<<": ";
              std::cout<<"\n";
            }else if(!is_positive_integer(input[1])){
              throw InvalidOperationException();
            }else{
              bool flag=true;
              std::vector<Transaction_Info> result=transaction_file.FindFirstN(std::stoi(input[1]),flag);
              if(!flag){
                throw InvalidOperationException();
              }
              double revenue=0;
              double expense=0;
              for(auto &i:result){
                if(i.total_price>0){
                  revenue+=i.total_price;
                }else{
                  expense-=i.total_price;
                }
              }
              //std::cout<<"Line"<<linenumber<<": ";
              std::cout<<std::fixed<<std::setprecision(2)<<"+ "<<revenue<<" - "<<expense<<"\n";
            }
          }else{
            throw InvalidOperationException();
          }

        }else{
          if (login_privilege < 1) {
            throw InvalidOperationException();
          }
          if (input.size() == 0) {
            std::vector<Book_Info> result = book_file.FindAll();
            if (result.size() == 0) {
              //std::cout<<"Line"<<linenumber<<": " ;
              std::cout<< "\n";
            } else {
              for (auto &i : result) {
                //std::cout<<"Line"<<linenumber<<": ";
                std::cout<< std::fixed << std::setprecision(2) << i.ISBN << '\t'
                          << i.bookname << '\t' << i.author << '\t' << i.keyword
                          << '\t' << i.price << '\t' << i.quantity << "\n";
              }
            }
          } else if (input.size() == 1) {
            std::string order = "";
            int index = 0;
            while (index<input[0].size()&&input[0][index] != '=') {
              order += input[0][index];
              index++;
            }
            if(index==input[0].size()||index==input[0].size()-1){
              throw InvalidOperationException();
            }
            input[0] = input[0].substr(index + 1);
            if (input[0] == "") {
              throw InvalidOperationException();
            }
            if (order == "-ISBN") {
              if(!is_ISBN(input[0])){
                throw InvalidOperationException();
              }
              std::vector<Book_Info> result = book_file.Find(input[0]);
              if (result.size() == 0) {
                //std::cout<<"Line"<<linenumber<<": ";
                std::cout<< "\n";
              } else {
                for (auto &i : result) {
                  //std::cout<<"Line"<<linenumber<<": ";
                  std::cout<< std::fixed << std::setprecision(2) << i.ISBN
                            << '\t' << i.bookname << '\t' << i.author << '\t'
                            << i.keyword << '\t' << i.price << '\t' << i.quantity
                            << "\n";
                }
              }
            } else if (order == "-name") {
              if(!is_name_or_author_or_keyword(remove_quote(input[0]))){
                throw InvalidOperationException();
              }
              std::vector<ISBN_String> ISBN_result =
                  name_file.Find(remove_quote(input[0]));
              if (ISBN_result.size() == 0) {
                //std::cout<<"Line"<<linenumber<<": ";
                std::cout<< "\n";
              } else {
                std::sort(ISBN_result.begin(), ISBN_result.end(),
                          [](ISBN_String a, ISBN_String b) {
                            return strcmp(a.ISBN, b.ISBN) < 0;
                          });
                for (auto &i : ISBN_result) {
                  std::vector<Book_Info> result = book_file.Find(i.ISBN);
                  for (auto &j : result) {
                    //std::cout<<"Line"<<linenumber<<": ";
                    std::cout<< std::fixed << std::setprecision(2) << j.ISBN
                              << '\t' << j.bookname << '\t' << j.author << '\t'
                              << j.keyword << '\t' << j.price << '\t'
                              << j.quantity << "\n";
                  }
                }
              }
            } else if (order == "-author") {
              if(!is_name_or_author_or_keyword(remove_quote(input[0]))){
                throw InvalidOperationException();
              }
              std::vector<ISBN_String> ISBN_result =
                  author_file.Find(remove_quote(input[0]));
              if (ISBN_result.size() == 0) {
                //std::cout<<"Line"<<linenumber<<": ";
                std::cout<<"\n";
              } else {
                std::sort(ISBN_result.begin(), ISBN_result.end(),
                          [](ISBN_String a, ISBN_String b) {
                            return strcmp(a.ISBN, b.ISBN) < 0;
                          });
                for (auto &i : ISBN_result) {
                  std::vector<Book_Info> result = book_file.Find(i.ISBN);
                  for (auto &j : result) {
                    //std::cout<<"Line"<<linenumber<<": ";
                    std::cout<<std::fixed << std::setprecision(2) << j.ISBN
                              << '\t' << j.bookname << '\t' << j.author << '\t'
                              << j.keyword << '\t' << j.price << '\t'
                              << j.quantity << "\n";
                  }
                }
              }
            } else if (order == "-keyword") {
              if(!is_name_or_author_or_keyword(remove_quote(input[0]))){
                throw InvalidOperationException();
              }
              std::vector<std::string> keyword;
              std::stringstream ss(remove_quote(input[0]));
              std::string temp="";
              int count=0;
              while (getline(ss, temp, '|')) {
                count++;
                if(temp.empty()){
                  throw InvalidOperationException();
                }
                keyword.push_back(temp);
                if(count>=2){
                  throw InvalidOperationException();
                }
              }
              if (keyword.size() >= 2) {
                throw InvalidOperationException();
              }
              if(keyword[0].empty()){
                throw InvalidOperationException();
              }
              std::vector<ISBN_String> ISBN_result =
                  keyword_file.Find(keyword[0]);
              if (ISBN_result.size() == 0) {
                //std::cout<<"Line"<<linenumber<<": ";
                std::cout<< "\n";
              } else {
                std::sort(ISBN_result.begin(), ISBN_result.end(),
                          [](ISBN_String a, ISBN_String b) {
                            return strcmp(a.ISBN, b.ISBN) < 0;
                          });
                for (auto &i : ISBN_result) {
                  std::vector<Book_Info> result = book_file.Find(i.ISBN);
                  for (auto &j : result) {
                    //std::cout<<"Line"<<linenumber<<": ";
                    std::cout<<std::fixed << std::setprecision(2) << j.ISBN
                              << '\t' << j.bookname << '\t' << j.author << '\t'
                              << j.keyword << '\t' << j.price << '\t'
                              << j.quantity << "\n";
                  }
                }
              }
            } else {
              throw InvalidOperationException();
            }
          } else {
            throw InvalidOperationException();
          }
        }
        std::cout<<"show success\n";
        
      } else if (command == "buy") {
        if (login_privilege < 1) {
          throw InvalidOperationException();
        }
        if (input.size() != 2) {
          throw InvalidOperationException();
        }
        if(!is_ISBN(input[0])){
          throw InvalidOperationException();
        }
        if (!is_quantity(input[1])) {
          throw InvalidOperationException();
        }
        std::vector<Book_Info> result = book_file.Find(input[0]);
        if (result.size() == 0) {
          throw InvalidOperationException();
        } else if (!is_positive_integer(input[1]) || std::stoi(input[1]) < 0 ||
                   result[0].quantity < std::stoi(input[1])) {
          throw InvalidOperationException();
        }
        Book_Info book_info = result[0];
        book_info.quantity -= std::stoi(input[1]);
        book_file.Update(book_info.ISBN, result[0], book_info);
        Transaction_Info transaction_info;
        strcpy(transaction_info.time, intToFixedString(operator_time).c_str());
        strcpy(transaction_info.userid, loginStack.back().user_id.c_str());
        strcpy(transaction_info.ISBN, book_info.ISBN);
        transaction_info.quantity = std::stoi(input[1]);
        transaction_info.total_price = book_info.price * transaction_info.quantity;
        transaction_file.Insert(transaction_info.time, transaction_info);
        //std::cout<<"Line"<<linenumber<<": ";
        std::cout<<std::fixed << std::setprecision(2)
                  << book_info.price * std::stoi(input[1]) << "\n";
      } else if (command == "select") {
        if (login_privilege < 3) {
          throw InvalidOperationException();
        }
        if (input.size() != 1) {
          throw InvalidOperationException();
        }
        if(!is_ISBN(input[0])){
          throw InvalidOperationException();
        }
        File_Storage<Book_Info> book_file("book_file");
        std::vector<Book_Info> result = book_file.Find(input[0]);
        Book_Info book_info;
        strcpy(book_info.ISBN, input[0].c_str());
        if (result.size() == 0) {
          book_file.Insert(input[0], book_info);
        }
        loginStack.back().selected_book = input[0];
        std::cout<<"Select success.\n";
      } else if (command == "modify") {
        if (login_privilege < 3) {
          throw InvalidOperationException();
        }
        std::string selected_book=loginStack.back().selected_book;
        if (selected_book == "") {
          throw InvalidOperationException();
        }
        int state = 0;
        std::vector<std::string> keyword;
        //auto start1 = std::chrono::system_clock::now();
        std::vector<std::pair<std::string, std::string>> order =
            Modification_Parser(input, selected_book,state,keyword);
        std::vector<Book_Info> result = book_file.Find(selected_book);
        Book_Info book_info=result[0];
        std::string temp_selected_book=selected_book;
        ISBN_String selected_book_ISBN, temp_selected_book_ISBN;
        strcpy(selected_book_ISBN.ISBN, selected_book.c_str());
        strcpy(temp_selected_book_ISBN.ISBN, temp_selected_book.c_str());
        for (int i = 0; i < order.size(); ++i) {
          if (order[i].first == "-ISBN") {
            std::vector<Book_Info> temp = book_file.Find(order[i].second);
            if (temp.size() != 0) {
              throw InvalidOperationException();
            }
            temp_selected_book = order[i].second; 
          } else if (order[i].first == "-name") {
            strcpy(book_info.bookname, order[i].second.c_str());
          } else if (order[i].first == "-author") {
            strcpy(book_info.author, order[i].second.c_str());
          } else if (order[i].first == "-keyword") {
            strcpy(book_info.keyword, order[i].second.c_str());
          } else if (order[i].first == "-price") {
            book_info.price = std::stod(order[i].second);
          }
        }
        //auto end1 = std::chrono::system_clock::now();
        //time1+=std::chrono::duration_cast<std::chrono::milliseconds>(end1-start1).count();
        //auto start2 = std::chrono::system_clock::now();
        strcpy(book_info.ISBN, temp_selected_book.c_str());
        //auto start3 = std::chrono::system_clock::now();
        if(selected_book!=temp_selected_book){
          for(int i=0;i<loginStack.size();i++){
            if(loginStack[i].selected_book==selected_book){
              loginStack[i].selected_book=temp_selected_book;
            }
          }
          book_file.Delete(selected_book, result[0]);
          book_file.Insert(temp_selected_book, book_info);
        }else{
          book_file.Update(selected_book, result[0], book_info);
        }
        //auto end3 = std::chrono::system_clock::now();
        //book_file_time+=std::chrono::duration_cast<std::chrono::milliseconds>(end3-start3).count();
        if(state&3){
          
          //auto start4 = std::chrono::system_clock::now();
          if((state&2)==0){
            name_file.Update(result[0].bookname, selected_book_ISBN, temp_selected_book_ISBN);
          }else{
            if(strlen(result[0].bookname)!=0){
              name_file.Delete(result[0].bookname, selected_book_ISBN);
            }
            name_file.Insert(book_info.bookname, temp_selected_book_ISBN);
          }
          //auto end4 = std::chrono::system_clock::now();
          //name_file_time+=std::chrono::duration_cast<std::chrono::milliseconds>(end4-start4).count();
        }
        if(state&5){
          //auto start5 = std::chrono::system_clock::now();
          if((state&4)==0){
            keyword_file.Update(result[0].author, selected_book_ISBN, temp_selected_book_ISBN);
          }else{
            if(strlen(result[0].author)!=0){
              author_file.Delete(result[0].author, selected_book_ISBN);
            }
            author_file.Insert(book_info.author, temp_selected_book_ISBN);
          }
          //auto end5 = std::chrono::system_clock::now();
          //author_file_time+=std::chrono::duration_cast<std::chrono::milliseconds>(end5-start5).count();
        }
        if(state&9){
          //auto start6 = std::chrono::system_clock::now();
          std::vector<std::string> keyword_ = CommandParser(result[0].keyword, '|');
          if((state&8)==0){
            for(auto &it:keyword_){
              keyword_file.Update(it, selected_book_ISBN, temp_selected_book_ISBN);
            }
          }else{
            for (auto &it : keyword_) {
              keyword_file.Delete(it, selected_book_ISBN);
            }
            for (auto &it : keyword) {
              keyword_file.Insert(it, temp_selected_book_ISBN);
            }
          }
          //auto end6 = std::chrono::system_clock::now();
          //keyword_file_time+=std::chrono::duration_cast<std::chrono::milliseconds>(end6-start6).count();
        }
        //auto end2 = std::chrono::system_clock::now();
        //time2+=std::chrono::duration_cast<std::chrono::milliseconds>(end2-start2).count();
        std::cout<<"Modify success.\n";
      } else if (command == "import") {
        if (login_privilege < 3) {
          throw InvalidOperationException();
        }
        std::string selected_book = loginStack.back().selected_book;
        if (selected_book == "") {
          throw InvalidOperationException();
        }
        if (input.size() != 2) {
          throw InvalidOperationException();
        }
        if (!is_quantity(input[0])) {
          throw InvalidOperationException();
        }
        if (!is_price(input[1])||std::stod(input[1])==0) {
          throw InvalidOperationException();
        }
        File_Storage<Book_Info> book_file("book_file");
        std::vector<Book_Info> result = book_file.Find(selected_book);
        Book_Info book_info = result[0];
        book_info.quantity += std::stoi(input[0]);
        book_file.Update(selected_book, result[0], book_info);
        Transaction_Info transaction_info;
        strcpy(transaction_info.time, intToFixedString(operator_time).c_str());
        strcpy(transaction_info.userid, loginStack.back().user_id.c_str());
        strcpy(transaction_info.ISBN, book_info.ISBN);
        transaction_info.quantity = std::stoi(input[0]);
        transaction_info.total_price = -std::stod(input[1]);
        transaction_file.Insert(transaction_info.time, transaction_info);
        std::cout<<"Import success.\n";
      } else if(command=="report"){
        if(login_privilege<7){
          throw InvalidOperationException();
        }
        if(input.size()!=1){
          throw InvalidOperationException();
        }
        if(input[0]=="finance"){
          std::vector<Transaction_Info> result=transaction_file.FindAll();
          for(auto &i:result){
            std::cout<<i.time<<"\t"<<i.userid<<"\t"<<i.ISBN<<"\t"<<i.quantity<<"\t"<<i.total_price<<"\n";
          }
        }else if(input[0]=="employee"){
          std::vector<Operation> result=employee_operation_file.FindAll();
          for(auto &i:result){
            std::cout<<i.time<<"\t"<<i.userid<<"\t"<<i.operation<<"\n";
          }
        }
        std::cout<<"Report success.\n";
      } else if(command=="log"){
        if(login_privilege<7){
          throw InvalidOperationException();
        }
        if(input.size()!=0){
          throw InvalidOperationException();
        }
        std::vector<Operation> result=operation_file.FindAll();
        for(auto &i:result){
          std::cout<<i.time<<"\t"<<i.userid<<"\t"<<i.privilege<<"\t"<<i.operation<<"\n";
        }
        std::cout<<"Log success.\n";
      } else {
        throw InvalidOperationException();
      }
    } catch (InvalidOperationException &e) {
      //std::cout<<"Line"<<linenumber<<": ";
      std::cout<<e.what() << "\n";
    }
  }
}
