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

	database& database = database::getInstance();
	databaseUser = database.getDatabaseUser();

	for (int i = 0; i < databaseUser.size(); i++)
	{
		ui.comboBoxUsers->addItem(databaseUser[i]->getName());
	}
}