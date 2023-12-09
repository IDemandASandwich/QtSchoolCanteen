#pragma once

#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "users.h"

class database
{
public:
	static database& getInstance();
	bool loadDatabaseUser();
	QVector<user*>& getDatabaseUser();

	enum jobs { none, cashier, cook, helper, admin };

private:
	QVector<user*> databaseUser;

	database();
	~database();
	database(const database&) = delete;
	database& operator=(const database&) = delete;
};

