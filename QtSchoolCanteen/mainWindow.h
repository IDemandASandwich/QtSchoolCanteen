#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"

#include "database.h"
#include "loginWindow.h"
#include "editUsers.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(int selectedId, QString userType = "regular", QWidget* parent = nullptr);
    ~mainWindow();
    void loadUser(int selectedId);
    void loadMenu();
    void setup(int selectedId, QString userType = "regular");
    void saveCurrentUserOrder();
    void loadCurrentUserOrder();
    int findListWidgetIndexInTable(QListWidgetItem* item);
    QTableWidget* currentTable(QString day);
    QString tableDay(QTableWidget* sender);

private slots:
    void pushButtonLogOut_clicked();
    void tableWidgetMenu_doubleClicked(int row, int column);
    void listWidgetOrders_doubleClicked(QListWidgetItem* item);
    void pushButtonEditUsers_clicked();

private:
    Ui::mainWindowClass ui;
    QVector<user*>& databaseUser;
    user* currentUser;
    menuDataType& databaseMenu;
};