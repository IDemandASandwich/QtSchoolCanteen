#include "mainWindow.h"

mainWindow::mainWindow(int selectedId, bool isAdmin, QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setup(selectedId, isAdmin);

    connect(ui.pushButtonLogOut, SIGNAL(clicked()), this, SLOT(pushButtonLogOut_clicked()));

    connect(ui.tableWidgetMonday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));
    connect(ui.tableWidgetTuesday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));
    connect(ui.tableWidgetWednesday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));
    connect(ui.tableWidgetThursday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));
    connect(ui.tableWidgetFriday, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetMenu_doubleClicked(int, int)));

    connect(ui.listWidgetOrder, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listWidgetOrders_doubleClicked(QListWidgetItem*)));
}

mainWindow::~mainWindow()
{
    //maybe a destructor since this is a pointer
}

void mainWindow::loadUser(int selectedId) {
    currentUser = databaseUser[selectedId];

    ui.lineEditUser->setText(currentUser->getName());
    ui.doubleSpinBoxCredit->setValue(currentUser->getCredit());
}

void mainWindow::loadMenu() {
    QList<QTableWidget*> tableWidgets = { ui.tableWidgetMonday, ui.tableWidgetTuesday, ui.tableWidgetWednesday, ui.tableWidgetThursday, ui.tableWidgetFriday };
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
}

void mainWindow::setup(int selectedId ,bool isAdmin) {

    if (!isAdmin) {
        ui.pushButtonEditUsers->setVisible(false);
        ui.pushButtonEditMenu->setVisible(false);
    }

    database& database = database::getInstance();
    databaseUser = database.getDatabaseUser();
    databaseMenu = database.getDatabaseMenu();

    loadUser(selectedId);
    loadMenu();
}

void mainWindow::pushButtonLogOut_clicked() {
    this->close();

    loginWindow* l = new loginWindow;
    l->show();
}

void mainWindow::tableWidgetMenu_doubleClicked(int row, int column) {
    QTableWidget* senderTableWidget = qobject_cast<QTableWidget*>(sender());
    QString day = "";

    if (senderTableWidget == ui.tableWidgetMonday)
        day = "Monday";
    else if (senderTableWidget == ui.tableWidgetTuesday)
        day = "Tuesday";
    else if (senderTableWidget == ui.tableWidgetWednesday)
        day = "Wednesday";
    else if (senderTableWidget == ui.tableWidgetThursday)
        day = "Thursday";
    else if (senderTableWidget == ui.tableWidgetFriday)
        day = "Friday";

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
    }
}

void mainWindow::listWidgetOrders_doubleClicked(QListWidgetItem* item) {
    QList<QTableWidget*> tableWidgets = { ui.tableWidgetMonday, ui.tableWidgetTuesday, ui.tableWidgetWednesday, ui.tableWidgetThursday, ui.tableWidgetFriday };
    QTableWidget* currentTableWidget;

    QString order = item->text();
	QStringList orderList = order.split('\t');

	QString day = orderList[0];
	QString course = orderList[1];
	QString name = orderList[2];
	double price = orderList[3].remove('$').toDouble();

	if (day == "Monday")
        currentTableWidget = tableWidgets[0];
	else if (day == "Tuesday")
        currentTableWidget = tableWidgets[1];
	else if (day == "Wednesday")
        currentTableWidget = tableWidgets[2];
	else if (day == "Thursday")
        currentTableWidget = tableWidgets[3];
	else if (day == "Friday")
        currentTableWidget = tableWidgets[4];

    int index = currentTableWidget->currentRow() % databaseMenu[day][course].size();

    for (int i = 0; i < currentTableWidget->rowCount(); i++) {
        if (currentTableWidget->item(i, 0)->text() == course && currentTableWidget->item(i, 1)->text() == name) {
			currentTableWidget->item(i, 2)->setText(QString::number(currentTableWidget->item(i, 2)->text().toDouble() + 1));
			break;
		}
	}

    databaseMenu[day][course][index].quantity += 1;
	currentUser->addCredit(price);
	ui.doubleSpinBoxCredit->setValue(currentUser->getCredit());

	delete item;
}