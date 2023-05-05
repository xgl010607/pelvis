#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_System.h"

class System : public QMainWindow
{
    Q_OBJECT

public:
    System(QWidget *parent = nullptr);
    ~System();

private:
    Ui::SystemClass ui;
};
