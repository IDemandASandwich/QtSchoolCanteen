#include "mainWindow.h"

mainWindow::mainWindow(int selectedId, QString userType, QWidget* parent)
    : QMainWindow(parent), databaseMenu(database::getInstance().getDatabaseMenu()), databaseUser(database::getInstance().getDatabaseUser())
{
    ui.setupUi(this);

    setup(selectedId, userType);

    connect(ui.tableWidgetMonday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));
    connect(ui.tableWidgetTuesday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));
    connect(ui.tableWidgetWednesday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));
    connect(ui.tableWidgetThursday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));
    connect(ui.tableWidgetFriday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));
     
    connect(ui.pushButtonLogOut, SIGNAL(clicked()), this, SLOT(pushButtonLogOut_clicked()));
    connect(ui.pushButtonEditUsers, SIGNAL(clicked()), this, SLOT(pushButtonEditUsers_clicked()));
    connect(ui.pushButtonClearOrders, SIGNAL(clicked()), this, SLOT(pushButtonClearOrders_clicked()));

    connect(ui.checkBoxEditMode, SIGNAL(stateChanged(int)), this, SLOT(checkBoxEditMenu_stateChanged(int)));

    connect(ui.listWidgetOrder, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listWidgetOrders_doubleClicked(QListWidgetItem*)));
}

mainWindow::~mainWindow()
{
    //a connected signal will take care of deleting the mainWindow instance
}

void mainWindow::loadUser(int selectedId) {
    currentUser = databaseUser[selectedId];

    ui.lineEditUser->setText(currentUser->getName());
    ui.doubleSpinBoxCredit->setValue(currentUser->getCredit());
}

void mainWindow::loadMenu() {
    QList<int> columnWidths = { 350, 55, 60 };

    QStringList header = {"Course", "Name", "Quantity" , "Price"};

    QStringList Day = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
    QStringList Course = { "Dinner", "Lunch", "Breakfast" };
    int currentDay = 0;

    for (QTableWidget* currentWidget : tableWidgets) {
        currentWidget->setColumnCount(header.size());
        currentWidget->setHorizontalHeaderLabels(header);

        for (int currentCourse = 0; currentCourse < databaseMenu[Day[currentDay]].size(); currentCourse++) {

            QList foodList = databaseMenu[Day.at(currentDay)][Course.at(currentCourse)];

            for (int j = 0; j < foodList.size(); j++) {
                currentWidget->insertRow(j); 

                currentWidget->setItem(j, 0, new QTableWidgetItem(Course[currentCourse]));
                currentWidget->setItem(j, 1, new QTableWidgetItem(foodList[j].name));
                currentWidget->setItem(j, 2, new QTableWidgetItem(QString::number(foodList[j].quantity)));

                if (currentUser->getUserType() == "student") {
                    double discount = 1 - static_cast<double>(currentUser->getDiscount()) / 100;
                    double newPrice = round(foodList[j].price * discount * 100) / 100;
                    currentWidget->setItem(j, 3, new QTableWidgetItem(QString::number(newPrice)));
                }
				else
					currentWidget->setItem(j, 3, new QTableWidgetItem(QString::number(foodList[j].price)));
            }
        }

        currentWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        currentWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        currentWidget->resizeColumnsToContents();
        currentWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        currentWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        currentDay++;
    }

    loadCurrentUserOrder();
}

void mainWindow::setup(int selectedId ,QString userType) {
    ui.pushButtonEditUsers->setVisible(userType == "admin");
    ui.checkBoxEditMode->setVisible(userType == "admin" || userType == "cook");
    ui.pushButtonClearOrders->setVisible(userType == "admin" || userType == "cook");

    tableWidgets = { ui.tableWidgetMonday, ui.tableWidgetTuesday, ui.tableWidgetWednesday, ui.tableWidgetThursday, ui.tableWidgetFriday };

    loadUser(selectedId);
    loadMenu();
}

void mainWindow::saveCurrentUserOrder(){
    if(ui.listWidgetOrder->count() == 0)
		return;

    QString fileName = "items/" + currentUser->getName() + "-Order.csv";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (int i = 0; i < ui.listWidgetOrder->count(); i++) {
        QStringList data = ui.listWidgetOrder->item(i)->text().split('\t');
        out << data[0] << "," << data[1] << "," << data[2] << "," << data[3].remove('$') << "\n";
    }

    file.close();
}

void mainWindow::loadCurrentUserOrder() {

	QString fileName = "items/" + currentUser->getName() + "-Order.csv";

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);

    while (!in.atEnd()) {
		QString line = in.readLine().trimmed();

		if (line.isEmpty())
			continue;

        QStringList data = line.split(',');

        QString day = data[0];
        QString course = data[1];
        QString name = data[2];
        QString price = data[3];

        QString toAdd = day + "\t" + course + "\t" + name + "\t" + price + "$";
		
        ui.listWidgetOrder->addItem(toAdd);

        QTableWidget* table = currentTable(day);

        for (int i = 0; i < table->rowCount(); i++) {
            QString tableCourse = table->item(i, 0)->text();
            QString tableName = table->item(i, 1)->text();

            if (tableCourse == course)
                table->hideRow(i);
        }
	}
	file.close();
}

void mainWindow::pushButtonLogOut_clicked() {
    
    saveCurrentUserOrder();
    
    database& database = database::getInstance();
    database.saveDatabaseUser();
    database.saveDatabaseMenu();
    
    this->close();

    loginWindow* loginwindow = new loginWindow;
    loginwindow->show();

    connect(loginwindow, &QObject::destroyed, this, [loginwindow]() { delete loginwindow; });	// establishes connection between the instance loginwindow and function delete
}

void mainWindow::tableWidgetMenu_doubleClicked(int row, int column) {
    QTableWidget* senderTableWidget = qobject_cast<QTableWidget*>(sender());

    QString day = tableDay(senderTableWidget);
    QString course = senderTableWidget->item(row, 0)->text();
    QString name = senderTableWidget->item(row, 1)->text();
    int quantity = senderTableWidget->item(row, 2)->text().toInt();
    double price = senderTableWidget->item(row, 3)->text().toDouble();
    int index = row % databaseMenu[day][course].size();

    Food* currentFood = &databaseMenu[day][course][index];

    if (quantity == 0) {
        QMessageBox::warning(this, "Warning", "This food is out of stock!");
    }
    else if (currentUser->getCredit() <= currentFood->price) {
        QMessageBox::warning(this, "Warning", "You don't have enough credit!");
    }
    else{
        QString toAdd = day + "\t" + course + "\t" + name + "\t" + QString::number(price) + "$";
        ui.listWidgetOrder->addItem(toAdd);

        currentUser->addCredit(-price);
        ui.doubleSpinBoxCredit->setValue(currentUser->getCredit());

        currentFood->quantity -= 1;
        senderTableWidget->item(row, 2)->setText(QString::number(currentFood->quantity));

        for (int i = 0; i < senderTableWidget->rowCount(); i++) {
            if(senderTableWidget->item(i, 0)->text() == course)
                senderTableWidget->hideRow(i);
        }
    }
}

void mainWindow::listWidgetOrders_doubleClicked(QListWidgetItem* item) {
    if(ui.checkBoxEditMode->isChecked())
		return;

    QString order = item->text();
	QStringList orderList = order.split('\t');

	QString day = orderList[0];
	QString course = orderList[1];
	QString name = orderList[2];
	double price = orderList[3].remove('$').toDouble();

    int listWidgetIndex = findListWidgetIndexInTable(item);
    QTableWidget* currentTableWidget = currentTable(day);
    
    for (int i = 0; i < currentTableWidget->rowCount(); i++) {
        if (currentTableWidget->item(i, 0)->text() == course && currentTableWidget->item(i, 1)->text() == name) {
			currentTableWidget->item(i, 2)->setText(QString::number(currentTableWidget->item(i, 2)->text().toDouble() + 1));
			break;
		}
	}

    for (int i = 0; i < currentTableWidget->rowCount(); i++) {
        if (currentTableWidget->item(i, 0)->text() == course)
            currentTableWidget->showRow(i);
    }

    databaseMenu[day][course][listWidgetIndex].quantity += 1;
	currentUser->addCredit(price);
	ui.doubleSpinBoxCredit->setValue(currentUser->getCredit());

	delete item;
}

int mainWindow::findListWidgetIndexInTable(QListWidgetItem* item) {

    QString order = item->text();
    QStringList orderList = order.split('\t');

    QString day = orderList[0];
    QString course = orderList[1];
    QString name = orderList[2];
    int listWidgetIndex = 0;
    int tableWidgetRow = 0;

    QTableWidget* currentTableWidget = currentTable(day);

    for (int i = 0; i < currentTableWidget->rowCount(); i++) {
        if (currentTableWidget->item(i, 0)->text() == course && currentTableWidget->item(i, 1)->text() == name)
            tableWidgetRow = i;
    }

    return (tableWidgetRow % databaseMenu[day][course].size());
}

QTableWidget* mainWindow::currentTable(QString day) {

    if (day == "Monday")
        return tableWidgets[0];
    else if (day == "Tuesday")
        return tableWidgets[1];
    else if (day == "Wednesday")
        return tableWidgets[2];
    else if (day == "Thursday")
        return tableWidgets[3];
    else if (day == "Friday")
        return tableWidgets[4];
    else
        return nullptr;
};

QString mainWindow::tableDay(QTableWidget* sender) {
    if (sender == ui.tableWidgetMonday)
        return "Monday";
    else if (sender == ui.tableWidgetTuesday)
        return "Tuesday";
    else if (sender == ui.tableWidgetWednesday)
        return "Wednesday";
    else if (sender == ui.tableWidgetThursday)
        return "Thursday";
    else if (sender == ui.tableWidgetFriday)
        return "Friday";
    else
        return "";
}

void mainWindow::pushButtonEditUsers_clicked() {
	editUsers* editusers = new editUsers;
	editusers->show();
}

void mainWindow::checkBoxEditMenu_stateChanged(int state) {
    if (state == Qt::Checked) {
		enableFoodQuantityEditing(true);
	}
    else {
		enableFoodQuantityEditing(false);

        updateDatabaseMenu();
    }
}

void mainWindow::enableFoodQuantityEditing(bool enable) {

    for (QTableWidget* currentWidget : tableWidgets) {
        if (enable) {
            currentWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
            currentWidget->setEditTriggers(QAbstractItemView::CurrentChanged);
        }
        else {
            currentWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
            currentWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }
    }
}

void mainWindow::updateDatabaseMenu() {

    for (QTableWidget* currentTable : tableWidgets) {
        	QString day = tableDay(currentTable);

            for (int i = 0; i < currentTable->rowCount(); i++) {
			    QString course = currentTable->item(i, 0)->text();
			    QString name = currentTable->item(i, 1)->text();
			    int quantity = currentTable->item(i, 2)->text().toInt();
			    double price = currentTable->item(i, 3)->text().toDouble();

			    int index = i % databaseMenu[day][course].size();

			    databaseMenu[day][course][index].quantity = quantity;
			    databaseMenu[day][course][index].price = price;
                databaseMenu[day][course][index].name = name;
		}
    }
}

void mainWindow::pushButtonClearOrders_clicked() {

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Clear orders", "Are you sure you want to clear orders?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        ui.listWidgetOrder->clear();

        for (QTableWidget* currentTable : tableWidgets) {
            QString day = tableDay(currentTable);

            for (int i = 0; i < currentTable->rowCount(); i++)
                currentTable->showRow(i);
        }

        QDir directory("items");
        QStringList files = directory.entryList(QStringList() << "*-Order.csv", QDir::Files);

        for (QString file : files) {
            QFile::remove("items/" + file);
        }
    }
}