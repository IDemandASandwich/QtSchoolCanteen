#include "editUsers.h"

editUsers::editUsers(QWidget *parent)
	: QMainWindow(parent), databaseUser(database::getInstance().getDatabaseUser())
{
	ui.setupUi(this);

	connect(ui.pushButtonOk, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.listWidgetView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listWidgetView_doubleClicked(QListWidgetItem*)));

	for (user* user : databaseUser)
		ui.listWidgetView->addItem(user->getName() + "\t" + user->getUserType());
}

editUsers::~editUsers()
{}

void editUsers::listWidgetView_doubleClicked(QListWidgetItem* item)
{
	int index = ui.listWidgetView->row(item);

	if(databaseUser[index]->getPosition() == staff::admin)
		QMessageBox::warning(this, "Error", "You can't delete admin account!");
	else
	{
		QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete user", "Are you sure you want to delete this user?", QMessageBox::Yes | QMessageBox::No);

		if (reply == QMessageBox::Yes)
		{
			databaseUser.remove(index);
			ui.listWidgetView->takeItem(index);
		}
	}
}