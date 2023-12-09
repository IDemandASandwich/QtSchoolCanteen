#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"

#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "users.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();
    bool loadDatabaseUser();

private:
    Ui::mainWindowClass ui;
    QVector<user*> databaseUser;
};
