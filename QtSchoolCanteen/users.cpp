#include "users.h"

// user base class

user::user(): name(""), password(""), credit(0.){}

user::user(QString userType, QString name, QString password, double credit):
	userType(userType), name(name), password(password), credit(credit){}

QString user::getUserType() const { return userType; }

QString user::getName() const { return name; }

QString user::getPassword() const { return password; }

double user::getCredit() const { return credit; }

void user::addCredit(double amount) { credit += amount; }

// employee derived class

employee::employee(): user(), department(""){}

employee::employee(QString userType, QString name, QString password, double credit, QString department):
	user(userType, name, password, credit), department(department){}

QString employee::getDepartment() const { return department; }


// student derived class

student::student(): user(), field(""), discount(0){}

student::student(QString userType, QString name, QString password, double credit, QString field, int discount):
	user(userType, name, password, credit), field(field), discount(discount){}

QString student::getField() const { return field; }

int student::getDiscount() const { return discount; }


// staff derived class

staff::staff(): user(), position(none){}

staff::staff(QString userType, QString name, QString password, double credit, int position):
	user(userType, name, password, credit), position(position){}

int staff::getPosition() const { return position; }