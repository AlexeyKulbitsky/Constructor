#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//#include <QtXml>
//#include <QXmlStreamReader>
#include <QMessageBox>
#include <QMainWindow>
#include <QFileDialog>
#include "model.h"
#include "filemanager.h"
#include "jsonfilemanager.h"
#include <QLineEdit>
#include <QScrollArea>
#include <QFormLayout>
#include <QUndoStack>
#include <QGraphicsScene>
#include "scene2d.h"
#include "settingsdialog.h"
#include "yandexmapsview.h"
#include "googlemapsview.h"
#include "osmview.h"
#include "cameraview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createMenu();
    void createActions();
    void createToolBar();

    Scene2D* scene2D;
    Scene3D* scene3D;
    CameraView* camera;

    QGraphicsScene *scene;
    SettingsDialog* settingsDialog;

    QStackedLayout* stackedLayout;
    YandexMapsView* yandexMaps;
    GoogleMapsView* googleMaps;
    OSMview* osMaps;

    QScrollArea *scenePropertiesScrollArea;
    QWidget* scenePropertiesWidget;
    QFormLayout* scenePropertiesLayout;
    QDockWidget* scenePropertiesDockWidget;

    QScrollArea *propertiesScrollArea;
    QWidget *propertiesWidget;
    QFormLayout *propertiesLayout;
    QDockWidget *propertiesDockWidget;

    QDockWidget *elementsDockWidget;
    QToolBar *propertiesToolBar;
    QLineEdit* ed;

    QComboBox* elementsComboBox;
    QStackedLayout* elementsLayout;

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* toolsMenu;
    QMenu* optionsMenu;
    QMenu* contextMenu;

    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;

    QAction* copyAction;
    QAction* deleteAction;
    QAction* pasteAction;
    QAction* cutAction;

    QAction* showGridAction;
    QAction* showRoadAction;
    QAction* showLinesAction;
    QAction* showMapAction;
    QAction* showRulerAction;
    QAction* saveToPresetAction;
    QAction* rulerAction;

    QAction* showProperties;

    QAction *undoAction;
    QAction *redoAction;

    QAction *optionsAction;

    QUndoStack *undoStack;

    Model* model;

    void readSettings();
    void writeSettings();
    QString currentFileName;

private:
    JSONFileManager* jsonFileManager;
    QSettings settings;

private slots:
    void newFile();
    void openFile();
    void saveAsFile();
    void saveFile();
    void on_tabWidget_currentChanged(int index);
    void setMap(int index);

public slots:
    void setFileManager(FileManager* manager);

private:
    Ui::MainWindow *ui;
    FileManager* fileManager;
    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent*);
};

#endif // MAINWINDOW_H
