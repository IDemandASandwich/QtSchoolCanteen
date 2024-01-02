#pragma once

#include <QMainWindow>
#include "ui_editUsers.h"

#include "users.h"
#include "database.h"
#include <QMessageBox>

class editUsers : public QMainWindow
{
	Q_OBJECT

public:
	editUsers(QWidget *parent = nullptr);
	~editUsers();

private slots:
	void listWidgetView_doubleClicked(QListWidgetItem*);

private:
	Ui::editUsersClass ui;
	QVector<user*>& databaseUser;
};
