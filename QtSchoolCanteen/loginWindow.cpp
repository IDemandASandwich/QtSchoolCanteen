#include "loginWindow.h"

loginWindow::loginWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setup();
	
	
}

loginWindow::~loginWindow()
{}

void loginWindow::setup()
{
	ui.setupUi(this);

	ui.pushButtonLogin->setEnabled(false);

}