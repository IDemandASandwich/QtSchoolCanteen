#include "mainWindow.h"

mainWindow::mainWindow(int selectedId ,bool isAdmin, QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setup(selectedId ,isAdmin);

    connect(ui.pushButtonLogOut, SIGNAL(clicked()), this, SLOT(pushButtonLogOut_clicked()));
}

mainWindow::~mainWindow()
{
    //maybe a destructor since this is a pointer
}

void mainWindow::loadUser(int selectedId) {
    user* currentUser = databaseUser[selectedId];

    ui.lineEditUser->setText(currentUser->getName());
    ui.spinBoxCredit->setValue(currentUser->getCredit());
}

void mainWindow::loadMenu() {
    QList<QTableWidget*> tableWidgets = { ui.tableWidgetMonday, ui.tableWidgetTuesday, ui.tableWidgetWednesday, ui.tableWidgetThursday, ui.tableWidgetFriday };
    QStringList header = { "Name", "Price", "Quantity" };
    QList<int> columnWidths = { 350, 55, 60 };

    for (QTableWidget* currentWidget : tableWidgets) {
        currentWidget->setColumnCount(header.size());
        currentWidget->setHorizontalHeaderLabels(header);
        currentWidget->resizeColumnsToContents();
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
}

void mainWindow::pushButtonLogOut_clicked() {
    this->close();

    loginWindow* l = new loginWindow;
    l->show();
}