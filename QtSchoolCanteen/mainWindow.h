#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"

#include "database.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(bool isAdmin = false, QWidget *parent = nullptr);
    ~mainWindow();

private:
    Ui::mainWindowClass ui;
};