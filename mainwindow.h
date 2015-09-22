#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtXml>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QMainWindow>
#include <QFileDialog>
#include "model.h"
#include "filemanager.h"
#include "jsonfilemanager.h"
#include <QLineEdit>
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

    QDockWidget *propertiesWidget;
    QToolBar *propertiesToolBar;
    QFormLayout *properties;
    QLineEdit* ed;

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* toolsMenu;
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

    QAction* rulerAction;

    QAction* showProperties;

    Model* model;

private:
    JSONFileManager* jsonFileManager;

private slots:
    void newFile();
    void openFile();
    void saveFile();

    void on_tabWidget_currentChanged(int index);

public slots:
    void setFileManager(FileManager* manager);

private:
    Ui::MainWindow *ui;
    FileManager* fileManager;

    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent *pe);
};

#endif // MAINWINDOW_H
