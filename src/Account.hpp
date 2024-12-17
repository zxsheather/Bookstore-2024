// Account.hpp

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>

struct User_Info{
	char userid[30];
	char password[30];
	char username[30];
	int privilege;
	
	User_Info() {
		memset(userid, 0, sizeof(userid));
		memset(password, 0, sizeof(password));
		memset(username, 0, sizeof(username));
		privilege = 0;
	}

	User_Info(std::string userid, std::string password, std::string username, int privilege) : privilege(privilege) {
		strcpy(this->userid, userid.c_str());
		strcpy(this->password, password.c_str());
		strcpy(this->username, username.c_str());
	}
	
	bool operator==(const User_Info &other) const {
		return (strcmp(userid, other.userid) == 0) && (strcmp(password, other.password) == 0) && (strcmp(username, other.username) == 0) && (privilege == other.privilege);
	}

	bool operator>(const User_Info &other) const {
		if (strcmp(userid, other.userid) == 0) {
			if (strcmp(password, other.password) == 0) {
				if (strcmp(username, other.username) == 0) {
					return privilege > other.privilege;
				}
				else {
					return strcmp(username, other.username) > 0;
				}
			}
			else {
				return strcmp(password, other.password) > 0;
			}
		}
		else {
			return strcmp(userid, other.userid) > 0;
		}
	}
};
// class User {
// protected:
//   User_Info user_info;
// public:
//   User(std::string userid, std::string password, std::string username,int privilege):privilege(privilege) {
//     strcpy(this->userid, userid.c_str());
//     strcpy(this->password, password.c_str());
//     strcpy(this->username, username.c_str());
//   }

//   virtual ~User() {}
//   void changePassword(std::string newPassword);
//   bool checkPassword(std::string password);
//   bool checkUsername(std::string username);
//   void deleteUser();
//   int getPrivilege();
//   std::string getUsername();
// };

// class Customer : public virtual User {
//  public:
//   explicit Customer(std::string userid, std::string password,
//                     std::string username)
//       : User(userid, password, username, 1) {}
//   ~Customer() {}
//   void queryBook(std::string command);
//   void buyBook(int ISBN, int quantity);
// };

// class Salesperson : public virtual User, public Customer {
//  protected:
//   int selected_ISBN;

//  public:
//   explicit Salesperson(std::string userid, std::string password,
//                        std::string username)
//       : User(userid, password, username, 3),
//         Customer(userid, password, username) {}
//   ~Salesperson() {}
//   void selectBook(int ISBN);
//   void modifyBook(std::string command);
//   void importBook(int quantity, int cost);
// };

// class Manager : public virtual User, public Salesperson {
//  public:
//   explicit Manager()
//       : User("root", "sjtu", "root", 7), Salesperson("root", "sjtu", "root") {}
//   ~Manager() {}
//   void queryPerchase();
//   void querySales();
//   void queryFinance();
// };

#endif