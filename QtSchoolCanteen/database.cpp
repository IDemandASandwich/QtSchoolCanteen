#include "database.h"

database::database() {
    loadDatabaseUser();
    loadDatabaseMenu();
}

database::~database() {
    for(user* u : databaseUser)
		delete u;
}

database& database::getInstance() {
	static database instance;
	return instance;
}

QVector<user*>& database::getDatabaseUser() {
	return databaseUser;
}

bool database::loadDatabaseUser() {
    QFile file("items/databaseUser.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.isEmpty())
            continue;

        QStringList data = line.split(',');

        QString userType = data[0].trimmed();

        if (userType.isEmpty())
            continue;

        if (userType == "employee") {
            QString name       = data[1].trimmed();
            QString password   = data[2].trimmed();
            int credit         = data[3].toInt();
            QString department = data[4].trimmed();

            databaseUser.push_back(new employee(name, password, credit, department));
        }
        else if (userType == "student") {
            QString name     = data[1].trimmed();
            QString password = data[2].trimmed();
            int credit       = data[3].toInt();
            QString field    = data[4].trimmed();
            int discount     = data[5].toInt();

            databaseUser.push_back(new student(name, password, credit, field, discount));
        }
        else if (userType == "staff") {
            QString name     = data[1].trimmed();
            QString password = data[2].trimmed();
            int credit       = data[3].toInt();
            QString position = data[4].trimmed();

            staff::jobs staffPosition = staff::none;

            if (position == "cashier")
                staffPosition = staff::cashier;
            else if (position == "cook")
                staffPosition = staff::cook;
            else if (position == "helper")
                staffPosition = staff::helper;
            else if (position == "admin")
                staffPosition = staff::admin;

            databaseUser.push_back(new staff(name, password, credit, staffPosition));
        }
    }

    file.close();
    return in.status() == QTextStream::Ok;
}

bool database::loadDatabaseMenu() {
    QFile file("items/databaseMenu.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        
        if (line == "Day,Course,Food Name,Price,Count")
            continue;
        else if (line.isEmpty())
            continue;

        QStringList data = line.split(',');

        Food currentFood;
        currentFood.name = data.at(2);
        currentFood.price = data.at(3).toDouble();
        currentFood.quantity = data.at(4).toInt();

        databaseMenu[data.at(0)][data.at(1)].push_back(currentFood);
    }

    file.close();
    return in.status() == QTextStream::Ok;
}

menuDataType& database::getDatabaseMenu() {
	return databaseMenu;
}