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
	void setup();

private slots:
	void listWidgetView_doubleClicked(QListWidgetItem*);
	void comboBoxType_currentIndexChanged(int);
	void pushButtonAdd_clicked();
	void listWidgetView_clicked(QListWidgetItem*);
	void pushButtonEdit_clicked();

private:
	Ui::editUsersClass ui;
	QVector<user*>& databaseUser;
};
