// main.cpp
// The entrance of the program.

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "src/Data.hpp"
#include "src/Exception.hpp"
#include "src/Key-Value-Database.hpp"
#include "src/Utils.hpp"

int main() {
  freopen(
      "/Users/apple/Desktop/Programming/ACMOJ/BookStore/Bookstore-2024/"
      "bookstore-testcases/basic/testcase5.in",
      "r", stdin);
  freopen(
      "/Users/apple/Desktop/Programming/ACMOJ/BookStore/Bookstore-2024/"
      "output.txt",
      "w", stdout);
  std::stack<std::pair<std::string, int>> loginStack;
  bool loginFlag = false;
  int login_privilege = 0;
  std::string selected_book = "";
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
  account_file.Initialize();
  if (flag || true) {
    User_Info user_info;
    user_info.privilege = 7;
    strcpy(user_info.userid, "root");
    strcpy(user_info.password, "sjtu");
    strcpy(user_info.username, "root");
    account_file.Insert(user_info.userid, user_info);
  }
  File_Storage<std::string> name_file("name_file");
  name_file.Initialize();
  File_Storage<std::string> author_file("author_file");
  author_file.Initialize();
  File_Storage<std::string> keyword_file("keyword_file");
  keyword_file.Initialize();
  File_Storage<Book_Info> book_file("book_file");
  book_file.Initialize();
  while (true) {
    try {
      std::string line;
      getline(std::cin, line);
      std::vector<std::string> input = CommandParser(line, ' ');
      std::string command = input[0];
      input.erase(input.begin());
      if (command == "quit" || command == "exit") {
        exit(0);
      } else if (command == "su") {
        if (input.size() >= 3 || input.size() == 0) {
          throw InvalidOpertionException();
        }
        std::string user_id = input[0];
        std::vector<User_Info> result = account_file.Find(user_id);
        if (result.size() == 0) {
          throw InvalidOpertionException();
        } else {
          User_Info user_info = result[0];
          if (login_privilege > user_info.privilege && loginFlag) {
            loginStack.push(
                std::make_pair(user_info.userid, user_info.privilege));
            loginFlag = true;
            login_privilege = user_info.privilege;
          } else {
            if (input.size() == 1) {
              throw InvalidOpertionException();
            }
            if (user_info.password == input[1]) {
              loginStack.push(
                  std::make_pair(user_info.userid, user_info.privilege));
              loginFlag = true;
              login_privilege = user_info.privilege;
            } else {
              throw InvalidOpertionException();
            }
          }
        }
      } else if (command == "logout") {
        if (input.size() != 0) {
          throw InvalidOpertionException();
        }
        if (loginStack.empty()) {
          throw InvalidOpertionException();
        } else {
          loginStack.pop();
          if (loginStack.empty()) {
            loginFlag = false;
            login_privilege = 0;
          } else {
            login_privilege = loginStack.top().second;
          }
        }
      } else if (command == "register") {
        if (input.size() != 3) {
          throw InvalidOpertionException();
        }
        User_Info user_info(input[0], input[1], input[2], 1);
        account_file.Insert(user_info.userid, user_info);
      } else if (command == "passwd") {
        if (login_privilege == 7 && input.size() == 2) {
          std::vector<User_Info> result = account_file.Find(input[0]);
          if (result.size() == 0) {
            throw InvalidOpertionException();
          } else {
            User_Info user_info = result[0];
            User_Info new_user_info(user_info.userid, input[1],
                                    user_info.username, user_info.privilege);
            account_file.Update(user_info.userid, user_info, new_user_info);
          }
        } else if (input.size() == 3) {
          std::vector<User_Info> result = account_file.Find(input[0]);
          if (result.size() == 0) {
            throw InvalidOpertionException();
          } else {
            User_Info user_info = result[0];
            if (user_info.password == input[1]) {
              User_Info new_user_info(user_info.userid, input[2],
                                      user_info.username, user_info.privilege);
              account_file.Update(user_info.userid, user_info, new_user_info);
            } else {
              throw InvalidOpertionException();
            }
          }
        } else {
          throw InvalidOpertionException();
        }
      } else if (command == "useradd") {
        if (input.size() != 4) {
          throw InvalidOpertionException();
        } else {
          std::vector<User_Info> result = account_file.Find(input[0]);
          if (result.size() != 0) {
            throw InvalidOpertionException();
          } else {
            if (!is_privilege(input[2])) {
              throw InvalidOpertionException();
            }
            int privilege = std::stoi(input[2]);
            if (login_privilege <= privilege) {
              throw InvalidOpertionException();
            }
            User_Info user_info(input[0], input[1], input[3], privilege);
            account_file.Insert(user_info.userid, user_info);
          }
        }
      } else if (command == "delete") {
        if (login_privilege != 7) {
          throw InvalidOpertionException();
        }
        if (input.size() != 1) {
          throw InvalidOpertionException();
        } else {
          std::vector<User_Info> result = account_file.Find(input[0]);
          if (result.size() == 0) {
            throw InvalidOpertionException();
          } else {
            std::stack<std::pair<std::string, int>> tempStack = loginStack;
            while (!tempStack.empty()) {
              auto it = tempStack.top();
              tempStack.pop();
              if (it.first == input[0]) {
                throw InvalidOpertionException();
              }
            }
            account_file.Delete(input[0], result[0]);
          }
        }
      } else if (command == "show") {
        File_Storage<Book_Info> book_file("book_file");
        if (login_privilege < 1) {
          throw InvalidOpertionException();
        }
        if (input.size() == 0) {
          std::vector<Book_Info> result = book_file.FindAll();
          if (result.size() == 0) {
            std::cout << "\n";
          } else {
            for (auto &i : result) {
              std::cout << std::fixed << std::setprecision(2) << i.ISBN << '\t'
                        << i.bookname << '\t' << i.author << '\t' << i.keyword
                        << '\t' << i.price << '\t' << i.quantity << std::endl;
            }
          }
        } else if (input.size() == 1) {
          std::string order = "";
          int index = 0;
          while (input[0][index] != '=') {
            order += input[0][index];
            index++;
          }
          input[0] = input[0].substr(index + 1);
          if (input[0] == "") {
            throw InvalidOpertionException();
          }
          if (order == "-ISBN") {
            std::vector<Book_Info> result = book_file.Find(input[0]);
            if (result.size() == 0) {
              std::cout << "\n";
            } else {
              for (auto &i : result) {
                std::cout << std::fixed << std::setprecision(2) << i.ISBN
                          << '\t' << i.bookname << '\t' << i.author << '\t'
                          << i.keyword << '\t' << i.price << '\t' << i.quantity
                          << std::endl;
              }
            }
          } else if (order == "-name") {
            File_Storage<std::string> name_file("name_file");
            std::vector<std::string> ISBN_result =
                name_file.Find(remove_quote(input[0]));
            if (ISBN_result.size() == 0) {
              std::cout << "\n";
            } else {
              std::sort(ISBN_result.begin(), ISBN_result.end());
              for (auto &i : ISBN_result) {
                std::vector<Book_Info> result = book_file.Find(i);
                for (auto &j : result) {
                  std::cout << std::fixed << std::setprecision(2) << j.ISBN
                            << '\t' << j.bookname << '\t' << j.author << '\t'
                            << j.keyword << '\t' << j.price << '\t'
                            << j.quantity << std::endl;
                }
              }
            }
          } else if (order == "-author") {
            File_Storage<std::string> author_file("author_file");
            std::vector<std::string> ISBN_result =
                author_file.Find(remove_quote(input[0]));
            if (ISBN_result.size() == 0) {
              std::cout << "\n";
            } else {
              std::sort(ISBN_result.begin(), ISBN_result.end());
              for (auto &i : ISBN_result) {
                std::vector<Book_Info> result = book_file.Find(i);
                for (auto &j : result) {
                  std::cout << std::fixed << std::setprecision(2) << j.ISBN
                            << '\t' << j.bookname << '\t' << j.author << '\t'
                            << j.keyword << '\t' << j.price << '\t'
                            << j.quantity << std::endl;
                }
              }
            }
          } else if (order == "-keyword") {
            std::vector<std::string> keyword = CommandParser(input[0], '|');
            if (keyword.size() >= 2) {
              throw InvalidOpertionException();
            }
            File_Storage<std::string> keyword_file("keyword_file");
            std::vector<std::string> ISBN_result =
                keyword_file.Find(remove_quote(keyword[0]));
            if (ISBN_result.size() == 0) {
              std::cout << "\n";
            } else {
              std::sort(ISBN_result.begin(), ISBN_result.end());
              for (auto &i : ISBN_result) {
                std::vector<Book_Info> result = book_file.Find(i);
                for (auto &j : result) {
                  std::cout << std::fixed << std::setprecision(2) << j.ISBN
                            << '\t' << j.bookname << '\t' << j.author << '\t'
                            << j.keyword << '\t' << j.price << '\t'
                            << j.quantity << std::endl;
                }
              }
            }
          } else {
            throw InvalidOpertionException();
          }
        } else {
          throw InvalidOpertionException();
        }

      } else if (command == "buy") {
        if (login_privilege < 1) {
          throw InvalidOpertionException();
        }
        if (input.size() != 2) {
          throw InvalidOpertionException();
        }
        File_Storage<Book_Info> book_file("book_file");
        std::vector<Book_Info> result = book_file.Find(input[0]);
        if (result.size() == 0) {
          throw InvalidOpertionException();
        } else if (!is_positive_integer(input[1]) || std::stoi(input[1]) < 0 ||
                   result[0].quantity < std::stoi(input[1])) {
          throw InvalidOpertionException();
        }
        Book_Info book_info = result[0];
        book_info.quantity -= std::stoi(input[1]);
        book_file.Update(book_info.ISBN, result[0], book_info);
        std::cout << std::fixed << std::setprecision(2)
                  << book_info.price * std::stoi(input[1]) << std::endl;
      } else if (command == "select") {
        if (login_privilege < 3) {
          throw InvalidOpertionException();
        }
        if (input.size() != 1) {
          throw InvalidOpertionException();
        }
        File_Storage<Book_Info> book_file("book_file");
        std::vector<Book_Info> result = book_file.Find(input[0]);
        if (result.size() == 0) {
          book_file.Insert(input[0], Book_Info());
        }
        selected_book = input[0];
      } else if (command == "modify") {
        if (login_privilege < 3) {
          throw InvalidOpertionException();
        }
        if (selected_book == "") {
          throw InvalidOpertionException();
        }
        std::vector<std::pair<std::string, std::string>> order =
            Modification_Parser(input, selected_book);
        for (int i = 0; i < order.size(); ++i) {
          File_Storage<Book_Info> book_file("book_file");
          if (order[i].first == "-ISBN") {
            std::vector<Book_Info> result = book_file.Find(selected_book);
            Book_Info book_info = result[0];
            strcpy(book_info.ISBN, order[i].second.c_str());
            book_file.Delete(selected_book, result[0]);
            book_file.Insert(order[i].second, book_info);
            if (strlen(book_info.bookname) != 0) {
              name_file.Update(book_info.bookname, selected_book,
                               order[i].second);
            }

            if (strlen(book_info.author) != 0) {
              author_file.Update(book_info.author, selected_book,
                                 order[i].second);
            }
            std::vector<std::string> keyword =
                CommandParser(book_info.keyword, '|');
            for (auto &it : keyword) {
              keyword_file.Update(it, selected_book, order[i].second);
            }
            selected_book = order[i].second;
          } else if (order[i].first == "-name") {
            std::vector<Book_Info> result = book_file.Find(selected_book);
            Book_Info book_info = result[0];
            strcpy(book_info.bookname, order[i].second.c_str());
            strcpy(book_info.ISBN, selected_book.c_str());
            book_file.Update(selected_book, result[0], book_info);
            File_Storage<std::string> name_file("name_file");
            name_file.Delete(book_info.bookname, selected_book);
            name_file.Insert(order[i].second, selected_book);
          } else if (order[i].first == "-author") {
            std::vector<Book_Info> result = book_file.Find(selected_book);
            Book_Info book_info = result[0];
            strcpy(book_info.author, order[i].second.c_str());
            strcpy(book_info.ISBN, selected_book.c_str());
            book_file.Update(selected_book, result[0], book_info);
            File_Storage<std::string> author_file("author_file");
            author_file.Delete(book_info.author, selected_book);
            author_file.Insert(order[i].second, selected_book);

          } else if (order[i].first == "-keyword") {
            std::vector<Book_Info> result = book_file.Find(selected_book);
            Book_Info book_info = result[0];
            std::vector<std::string> keyword =
                CommandParser(book_info.keyword, '|');
            File_Storage<std::string> keyword_file("keyword_file");
            for (auto &it : keyword) {
              keyword_file.Delete(it, selected_book);
            }
            std::vector<std::string> keyword_ =
                CommandParser(order[i].second, '|');
            for (auto &it : keyword_) {
              keyword_file.Insert(it, selected_book);
            }
            strcpy(book_info.keyword, order[i].second.c_str());
            strcpy(book_info.ISBN, selected_book.c_str());
            book_file.Update(selected_book, result[0], book_info);
          } else if (order[i].first == "-price") {
            std::vector<Book_Info> result = book_file.Find(selected_book);
            Book_Info book_info = result[0];
            strcpy(book_info.ISBN, selected_book.c_str());
            book_info.price = std::stod(order[i].second);
            book_file.Update(selected_book, result[0], book_info);
          }
        }
      } else if (command == "import") {
        if (login_privilege < 3) {
          throw InvalidOpertionException();
        }
        if (selected_book == "") {
          throw InvalidOpertionException();
        }
        if (input.size() != 2) {
          throw InvalidOpertionException();
        }
        if (!is_positive_integer(input[0])) {
          throw InvalidOpertionException();
        }
        if (!is_positive_number(input[1])) {
          throw InvalidOpertionException();
        }
        File_Storage<Book_Info> book_file("book_file");
        std::vector<Book_Info> result = book_file.Find(selected_book);
        Book_Info book_info = result[0];
        book_info.quantity += std::stoi(input[0]);
        book_file.Update(selected_book, result[0], book_info);
      } else {
        throw InvalidOpertionException();
      }
    } catch (InvalidOpertionException &e) {
      std::cout << e.what() << std::endl;
    }
  }
}
