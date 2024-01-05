#include "editUsers.h"

editUsers::editUsers(QWidget *parent)
	: QMainWindow(parent), databaseUser(database::getInstance().getDatabaseUser())
{
	ui.setupUi(this);

	setup();

	connect(ui.pushButtonOk, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.listWidgetView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listWidgetView_doubleClicked(QListWidgetItem*)));
	connect(ui.comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxType_currentIndexChanged(int)));
	connect(ui.pushButtonAdd, SIGNAL(clicked()), this, SLOT(pushButtonAdd_clicked()));
	connect(ui.listWidgetView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listWidgetView_clicked(QListWidgetItem*)));
	connect(ui.pushButtonEdit, SIGNAL(clicked()), this, SLOT(pushButtonEdit_clicked()));

}

editUsers::~editUsers()
{}

void editUsers::setup() {
	for (user* user : databaseUser)
		ui.listWidgetView->addItem(user->getName() + "\t" + user->getUserType());

	QStringList type = { "student", "employee", "staff" };
	ui.comboBoxType->addItems(type);

	QStringList position = { "cashier", "cook", "helper", "admin" };
	ui.comboBoxPosition->addItems(position);

	comboBoxType_currentIndexChanged(0);
}

void editUsers::comboBoxType_currentIndexChanged(int index) {
	enum type { student, employee, staff };

	ui.lineEditName->clear();
	ui.lineEditPassword->clear();
	ui.spinBoxCredit->setValue(0);
	ui.lineEditDepartment->clear();
	ui.lineEditField->clear();
	ui.spinBoxDiscount->setValue(0);

	ui.lineEditDepartment->setVisible(index == employee);
	ui.labelDepartment->setVisible(index == employee);

	ui.lineEditField->setVisible(index == student);
	ui.labelField->setVisible(index == student);
	ui.spinBoxDiscount->setVisible(index == student);
	ui.labelDiscount->setVisible(index == student);

	ui.comboBoxPosition->setVisible(index == staff);
	ui.labelPosition->setVisible(index == staff);

	ui.pushButtonEdit->setEnabled(false);
}

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

void editUsers::pushButtonAdd_clicked() {
	QString userType = ui.comboBoxType->currentText();
	QString name = ui.lineEditName->text();
	QString password = ui.lineEditPassword->text();
	double credit = ui.spinBoxCredit->value();

	if (userType == "Student")
	{
		QString field = ui.lineEditField->text();
		int discount = ui.spinBoxDiscount->value();

		databaseUser.push_back(new student("student", name, password, credit, field, discount));
	}
	else if (userType == "Employee")
	{
		QString department = ui.lineEditDepartment->text();

		databaseUser.push_back(new employee("employee", name, password, credit, department));
	}
	else if (userType == "Staff")
	{
		int position = ui.comboBoxPosition->currentIndex();

		databaseUser.push_back(new staff("staff", name, password, credit, position));
	}

	ui.listWidgetView->addItem(name + "\t" + userType);
}

void editUsers::listWidgetView_clicked(QListWidgetItem* item) {
	int index = ui.listWidgetView->row(item);

	if (databaseUser[index]->getUserType() == "student")
	{
		ui.comboBoxType->setCurrentIndex(0);
		comboBoxType_currentIndexChanged(0);
		ui.lineEditField->setText(dynamic_cast<student*>(databaseUser[index])->getField());
		ui.spinBoxDiscount->setValue(dynamic_cast<student*>(databaseUser[index])->getDiscount());
	}
	else if (databaseUser[index]->getUserType() == "employee")
	{
		ui.comboBoxType->setCurrentIndex(1);
		comboBoxType_currentIndexChanged(1);
		ui.lineEditDepartment->setText(dynamic_cast<employee*>(databaseUser[index])->getDepartment());
	}
	else if (databaseUser[index]->getUserType() == "staff")
	{
		ui.comboBoxType->setCurrentIndex(2);
		comboBoxType_currentIndexChanged(2);
		ui.comboBoxPosition->setCurrentIndex(dynamic_cast<staff*>(databaseUser[index])->getPosition() - 1);
	}

	ui.lineEditName->setText(databaseUser[index]->getName());
	ui.lineEditPassword->setText(databaseUser[index]->getPassword());
	ui.spinBoxCredit->setValue(databaseUser[index]->getCredit());

	ui.pushButtonEdit->setEnabled(true);
}

void editUsers::pushButtonEdit_clicked() {
	
	int index = ui.listWidgetView->currentRow();

	QString userType = ui.comboBoxType->currentText();
	QString name = ui.lineEditName->text();
	QString password = ui.lineEditPassword->text();
	double credit = ui.spinBoxCredit->value();

	if (userType == "student")
	{
		QString field = ui.lineEditField->text();
		int discount = ui.spinBoxDiscount->value();

		dynamic_cast<student*>(databaseUser[index])->editUser(name, password, credit, field, discount);
	}
	else if (userType == "employee")
	{
		QString department = ui.lineEditDepartment->text();

		dynamic_cast<employee*>(databaseUser[index])->editUser(name, password, credit, department);
	}
	else if (userType == "staff")
	{
		int position = ui.comboBoxPosition->currentIndex() + 1;

		dynamic_cast<staff*>(databaseUser[index])->editUser(name, password, credit, position);
	}

	ui.listWidgetView->item(index)->setText(name + "\t" + userType);
}