#pragma once

#include <QString>

class user
{
public:
	user();
	user(QString name, QString password, int credit);
	QString getName() const;
	QString getPassword() const;
	int getCredit() const;
	void addCredit(int amount);

	virtual QString getDepartment() const = 0;
	virtual QString getField() const = 0;
	virtual int getDiscount() const = 0;
	virtual int getPosition() const = 0;

private:
	QString name;
	QString password;
	int credit;
	
};

class employee : public user {
public:
	employee();
	employee(QString name, QString password, int credit , QString department);
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
	student(QString name, QString password, int credit, QString field, int discount);
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
	staff(QString name, QString password, int credit, int position);
	int getPosition() const override;

	//these are not used for staff but are needed for the program to work
	QString getDepartment() const override { return QString(); };
	QString getField() const override { return QString(); };
	int getDiscount() const override { return 0; };

	enum jobs { none, cashier, cook, helper, admin };

private:
	int position;

};
