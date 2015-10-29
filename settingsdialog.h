#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "model.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;

public slots:
    void setLoggingRoadBroken(bool status);
    void setLoggingRoadSimple(bool status);
    void setLoggingCompositeRoad(bool status);
};

#endif // SETTINGSDIALOG_H
