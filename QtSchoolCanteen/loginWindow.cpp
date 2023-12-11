#include "loginWindow.h"

loginWindow::loginWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setup();

	connect(ui.lineEditPassword, SIGNAL(textChanged(QString)), this, SLOT(lineEditPassword_Changed(QString)));
	connect(ui.checkBoxShowPassword, SIGNAL(stateChanged(int)), this, SLOT(checkBoxShowPassword_Changed(int)));
	connect(ui.pushButtonLogin, SIGNAL(clicked()), this, SLOT(pushButtonLogin_Clicked()));
	
	#ifdef DEBUG
		debug();
	#endif
}

loginWindow::~loginWindow()
{
}

void loginWindow::setup()
{
	ui.setupUi(this);
	ui.pushButtonLogin->setEnabled(false);

	database& database = database::getInstance();
	databaseUser = database.getDatabaseUser();

	for (user* user : databaseUser)
		ui.comboBoxUsers->addItem(user->getName());
}

void loginWindow::lineEditPassword_Changed(QString text)
{
	ui.pushButtonLogin->setEnabled(!text.isEmpty());
}

void loginWindow::checkBoxShowPassword_Changed(int state)
{
	if (state == Qt::Checked)
		ui.lineEditPassword->setEchoMode(QLineEdit::Normal);
	else
		ui.lineEditPassword->setEchoMode(QLineEdit::Password);
}

void loginWindow::pushButtonLogin_Clicked()
{
	QString name = ui.comboBoxUsers->currentText();
	QString password = ui.lineEditPassword->text();

	for (user* user : databaseUser)
	{
		if (user->getName() == name && user->getPassword() == password)
		{
			int id = ui.comboBoxUsers->currentIndex();

			if (user->getPosition() == staff::admin) {
				mainWindow* m = new mainWindow(id ,true);
				m->show();
			}
			else {
				mainWindow* m = new mainWindow(id);
				m->show();
			}

			this->close();
			return;
		}
	}

	QMessageBox::warning(this, "Oops!", "Wrong password");
}

void loginWindow::debug()
{
	#ifdef PRINT_PASSWORDS
	for(user* user : databaseUser)
		qDebug() << user->getName() << user->getPassword();
	#endif
}