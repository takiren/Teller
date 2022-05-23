#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TellerEditor.h"

class TellerEditor : public QMainWindow
{
    Q_OBJECT

public:
    TellerEditor(QWidget *parent = Q_NULLPTR);

private:
    Ui::TellerEditorClass ui;
};
