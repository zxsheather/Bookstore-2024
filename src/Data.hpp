// Data.hpp

#ifndef DATA_HPP
#define DATA_HPP

#include <string>


struct Login_Info {
    std::string user_id;
    int privilege;
    std::string selected_book;
    Login_Info() : user_id(""), privilege(0), selected_book("") {}
    Login_Info(std::string user_id, int privilege, std::string selected_book)
        : user_id(user_id), privilege(privilege), selected_book(selected_book) {}
  };

struct User_Info {
  char userid[31];
  char password[31];
  char username[31];
  int privilege;

  User_Info() {
    memset(userid, 0, sizeof(userid));
    memset(password, 0, sizeof(password));
    memset(username, 0, sizeof(username));
    privilege = 0;
  }

  User_Info(std::string userid, std::string password, std::string username,
            int privilege)
      : privilege(privilege) {
    strcpy(this->userid, userid.c_str());
    strcpy(this->password, password.c_str());
    strcpy(this->username, username.c_str());
  }

  bool operator==(const User_Info &other) const {
    return (strcmp(userid, other.userid) == 0) &&
           (strcmp(password, other.password) == 0) &&
           (strcmp(username, other.username) == 0) &&
           (privilege == other.privilege);
  }

  bool operator>(const User_Info &other) const {
    if (strcmp(userid, other.userid) == 0) {
      if (strcmp(password, other.password) == 0) {
        if (strcmp(username, other.username) == 0) {
          return privilege > other.privilege;
        } else {
          return strcmp(username, other.username) > 0;
        }
      } else {
        return strcmp(password, other.password) > 0;
      }
    } else {
      return strcmp(userid, other.userid) > 0;
    }
  }
};

struct Book_Info {
  char ISBN[21];
  char bookname[61];
  char author[61];
  char keyword[61];
  size_t quantity;
  double price;

  Book_Info() : quantity(0), price(0.0) {
    memset(ISBN, 0, sizeof(ISBN));
    memset(bookname, 0, sizeof(bookname));
    memset(author, 0, sizeof(author));
    memset(keyword, 0, sizeof(keyword));
  }

  Book_Info(std::string ISBN, std::string bookname, std::string author,
            std::string keyword, size_t quantity, double price)
      : quantity(quantity), price(price) {
    strcpy(this->ISBN, ISBN.c_str());
    strcpy(this->bookname, bookname.c_str());
    strcpy(this->author, author.c_str());
    strcpy(this->keyword, keyword.c_str());
  }

  bool operator==(const Book_Info &other) const {
    return (strcmp(ISBN, other.ISBN) == 0) &&
           (strcmp(bookname, other.bookname) == 0) &&
           (strcmp(author, other.author) == 0) &&
           (strcmp(keyword, other.keyword) == 0) &&
           (quantity == other.quantity) && (price == other.price);
  }

  bool operator>(const Book_Info &other) const {
    if (strcmp(ISBN, other.ISBN) == 0) {
      if (strcmp(bookname, other.bookname) == 0) {
        if (strcmp(author, other.author) == 0) {
          if (strcmp(keyword, other.keyword) == 0) {
            if (quantity == other.quantity) {
              return price > other.price;
            } else {
              return quantity > other.quantity;
            }
          } else {
            return strcmp(keyword, other.keyword) > 0;
          }
        } else {
          return strcmp(author, other.author) > 0;
        }
      } else {
        return strcmp(bookname, other.bookname) > 0;
      }
    } else {
      return strcmp(ISBN, other.ISBN) > 0;
    }
  }
};

struct Transaction_Info {
	char time[11];
  char userid[31];
  char ISBN[21];
  size_t quantity;
  double total_price;

  Transaction_Info() : quantity(0), total_price(0.0) {
		memset(time, 0, sizeof(time));
    memset(userid, 0, sizeof(userid));
    memset(ISBN, 0, sizeof(ISBN));
  }

  Transaction_Info(std::string time, std::string userid, std::string ISBN,
									 size_t quantity, double total_price)
			: quantity(quantity), total_price(total_price) {
		strcpy(this->time, time.c_str());
		strcpy(this->userid, userid.c_str());
		strcpy(this->ISBN, ISBN.c_str());
	}

	bool operator==(const Transaction_Info &other) const {
		return (strcmp(time, other.time) == 0) &&
					 (strcmp(userid, other.userid) == 0) &&
					 (strcmp(ISBN, other.ISBN) == 0) &&
					 (quantity == other.quantity) && (total_price == other.total_price);
	}

	bool operator>(const Transaction_Info &other) const {
		if (std::stoi(time) == std::stoi(other.time)) {
			if (strcmp(userid, other.userid) == 0) {
				if (strcmp(ISBN, other.ISBN) == 0) {
					if (quantity == other.quantity) {
						return total_price > other.total_price;
					} else {
						return quantity > other.quantity;
					}
				} else {
					return strcmp(ISBN, other.ISBN) > 0;
				}
			} else {
				return strcmp(userid, other.userid) > 0;
			}
		} else {
			return std::stoi(time) > std::stoi(other.time);
		}
	}
};

struct ISBN_String{
  char ISBN[21];
  ISBN_String(){
    memset(ISBN,0,sizeof(ISBN));
  }
  ISBN_String(std::string ISBN){
    strcpy(this->ISBN,ISBN.c_str());
  }
  bool operator==(const ISBN_String &other) const{
    return strcmp(ISBN,other.ISBN)==0;
  }
  bool operator>(const ISBN_String &other) const{
    return strcmp(ISBN,other.ISBN)>0;
  }
};
struct Operation{
  char time[11];
  char userid[31];
  int privilege;
  char operation[31];
};

#endif