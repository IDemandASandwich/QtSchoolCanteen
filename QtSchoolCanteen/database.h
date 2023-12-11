#pragma once

#include <QVector>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "users.h"

struct Food {
	QString name;
	double price;
	int quantity;
};

typedef QMap<QString, QMap<QString, QVector<Food>>> menuDataType;

//singleton pattern : only one instance of database can be created

class database
{
public:
	static database& getInstance();
	bool loadDatabaseUser();
	QVector<user*>& getDatabaseUser();
	bool loadDatabaseMenu();
	menuDataType& getDatabaseMenu();

private:
	QVector<user*> databaseUser;
	menuDataType databaseMenu;

	database();
	~database();
	database(const database&) = delete;
	database& operator=(const database&) = delete;
};

