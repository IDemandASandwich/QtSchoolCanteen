#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    loadDatabaseUser();
}

mainWindow::~mainWindow()
{
    for (int i = 0; i < databaseUser.size(); i++) {
		delete databaseUser[i];
	}
}

bool mainWindow::loadDatabaseUser() {
    enum jobs { none, cashier, cook, helper, admin };

    QFile file("items/databaseUser.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString job = in.readLine();

        if (job == "employee") {
            QString name = in.readLine();
            QString password = in.readLine();
            int credit = in.readLine().toInt();
            QString department = in.readLine();

            databaseUser.push_back(new employee(name, password, credit, department));
        }
        else if(job == "student") {
			QString name = in.readLine();
			QString password = in.readLine();
			int credit = in.readLine().toInt();
            QString field = in.readLine();
            int discount = in.readLine().toInt();

			databaseUser.push_back(new student(name, password, credit, field, discount));
		}
        else if (job == "staff") {
            QString name = in.readLine();
            QString password = in.readLine();
            int credit = in.readLine().toInt();
            QString position = in.readLine();

            if (position == "cashier")
                databaseUser.push_back(new staff(name, password, credit, cashier));
            else if (position == "cook")
                databaseUser.push_back(new staff(name, password, credit, cook));
            else if (position == "helper")
                databaseUser.push_back(new staff(name, password, credit, helper));
            else if (position == "admin")
                databaseUser.push_back(new staff(name, password, credit, admin));
            else
                databaseUser.push_back(new staff(name, password, credit, none));
        }

        in.skipWhiteSpace();
	}

    file.close();
	return true;
}