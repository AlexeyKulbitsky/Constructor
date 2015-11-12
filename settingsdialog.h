#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "model.h"
#include "statemanager.h"
#include "scene2d.h"
#include "scene3d.h"
#include <QTreeWidget>
#include "yandexmapsview.h"
#include "googlemapsview.h"
#include "commands.h"
#include "filemanager.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    explicit SettingsDialog(QSettings* settings, QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
    QSettings* settings;
    QTimer* timer;
    QMap<QTreeWidgetItem*, void (SettingsDialog::*)(bool)> logTreeSlots;
    QMap<QTreeWidgetItem*, QString> logSettingsNames;

    bool autoSaveOn;
    double autoSaveTime;
    QString autoSaveFile;

signals:
    void autoSaveOnChanged(bool status);
    void autoSaveFileNameChanged(QString name);
    void autoSaveTimeChanged(double time);

public slots:
    void setLoggingRoadBroken(bool status);
    void setLoggingRoadSimple(bool status);
    void setLoggingCompositeRoad(bool status);
    void setLoggingRoundingRoad(bool status);
    void setLoggingIntersection(bool status);
    void setLoggingCurve(bool status);
    void setLoggingLineSimple(bool status);
    void setLoggingLineBroken(bool status);
    void setLoggingRailway(bool status);
    void setLoggingSplitZone(bool status);
    void setLoggingRuler(bool status);

    void setLoggingScene2D(bool status);
    void setLoggingScene3D(bool status);
    void setLoggingYandexMaps(bool status);
    void setLoggingGoogleMaps(bool status);

    void setLoggingStateManager(bool status);
    void setLoggingDefaultState(bool status);
    void setLoggingSelectedState(bool status);
    void setLoggingRoadBuilderState(bool status);
    void setLoggingLineBuilderState(bool status);
    void setLoggingRulerState(bool status);

    void setLoggingMoveCommand(bool status);
    void setLoggingResizeByControlCommand(bool status);
    void setLoggingDeleteCommand(bool status);
    void setLoggingInsertCommand(bool status);
    void setLoggingAddBreakCommand(bool status);
    void setLoggingPasteCommand(bool status);

    void setLoggingModel(bool status);
    void setLoggingStates(bool status);
    void setLoggingView(bool status);
    void setLoggingUndoStack(bool status);

    void setLoggingAll(bool status);

    void processTreeClick(QTreeWidgetItem* item, int column);
    void okButtonClicked();
    void acceptButtonClicked();
    void cancelButtonClicked();


    void setAutoSaveOn(bool status);
    void setAutoSaveFileName(QString name);
    void setAutoSaveTime(double time);

    void setFont();

private:
    void setLogTreeView();

public:
    void setSettings(QSettings* settings);
    void saveLogTreeSettings();
    void loadLogTreeSettings();
    void processTreeItemsStates();

    void loadAutoSave();
    void saveAutoSave();
};

#endif // SETTINGSDIALOG_H
