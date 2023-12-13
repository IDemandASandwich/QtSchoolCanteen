#pragma once

#include <QString>

class user
{
public:
	user();
	user(QString userType, QString name, QString password, double credit);
	QString getUserType() const;
	QString getName() const;
	QString getPassword() const;
	double getCredit() const;
	void addCredit(double amount);

	virtual QString getDepartment() const = 0;
	virtual QString getField() const = 0;
	virtual int getDiscount() const = 0;
	virtual int getPosition() const = 0;

private:
	QString userType;
	QString name;
	QString password;
	double credit;
	
};

class employee : public user {
public:
	employee();
	employee(QString userType, QString name, QString password, double credit , QString department);
	QString getDepartment() const override;

	//these are not used for employees but are needed for the program to work
	QString getField() const override { return QString(); };
	int getDiscount() const override { return 0; };
	int getPosition() const override { return 0; };

private:
	QString department;

};

class student : public user {
public:
	student();
	student(QString userType, QString name, QString password, double credit, QString field, int discount);
	QString getField() const override;
	int getDiscount() const override;

	//these are not used for students but are needed for the program to work
	QString getDepartment() const override { return QString(); };
	int getPosition() const override { return 0; };

private:
	QString field;
	int discount;

};

class staff : public user {
public:
	staff();
	staff(QString userType, QString name, QString password, double credit, int position);
	int getPosition() const override;

	//these are not used for staff but are needed for the program to work
	QString getDepartment() const override { return QString(); };
	QString getField() const override { return QString(); };
	int getDiscount() const override { return 0; };

	enum jobs { none, cashier, cook, helper, admin };

private:
	int position;

};
