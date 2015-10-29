#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->logRoadBrokenCheckBox, SIGNAL(toggled(bool)), this, SLOT(setLoggingRoadBroken(bool)));
    connect(ui->logRoadSimpleCheckBox, SIGNAL(toggled(bool)), this, SLOT(setLoggingRoadSimple(bool)));
    connect(ui->logCompositeRoadCheckBox, SIGNAL(toggled(bool)), this, SLOT(setLoggingCompositeRoad(bool)));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setLoggingRoadBroken(bool status)
{
    RoadBroken::setLogging(status);
}

void SettingsDialog::setLoggingRoadSimple(bool status)
{
    RoadSimple::setLogging(status);
}

void SettingsDialog::setLoggingCompositeRoad(bool status)
{
    CompositeRoad::setLogging(status);
}
