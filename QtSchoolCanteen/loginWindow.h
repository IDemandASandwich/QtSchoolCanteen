#pragma once

//#define DEBUG
#define PRINT_PASSWORDS

#include <QMainWindow>
#include "ui_loginWindow.h"
#include <QMessageBox>

#include "mainWindow.h"
#include "database.h"

class loginWindow : public QMainWindow
{
	Q_OBJECT

public:
	loginWindow(QWidget *parent = nullptr);
	~loginWindow();
	void setup();
	void debug();

private slots:
	void lineEditPassword_Changed(QString);
	void checkBoxShowPassword_Changed(int);
	void pushButtonLogin_Clicked();

private:
	Ui::loginWindowClass ui;
	QVector<user*> databaseUser;
};