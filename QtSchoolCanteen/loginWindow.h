#pragma once

#include <QMainWindow>
#include "ui_loginWindow.h"

class loginWindow : public QMainWindow
{
	Q_OBJECT

public:
	loginWindow(QWidget *parent = nullptr);
	void setup();
	~loginWindow();

private:
	Ui::loginWindowClass ui;
};