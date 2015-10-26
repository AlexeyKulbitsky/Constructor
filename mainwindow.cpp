#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "scene2d.h"
#include <QToolButton>
#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "objectslist.h"
#include <QDockWidget>
#include <QScrollBar>

#include <QToolBox>
#include "logger.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    Logger::getLogger()->startLogging();
    ui->setupUi(this);

    RoadElement::undoStack = new QUndoStack(this);
    undoStack = RoadElement::undoStack;

    model = new Model(this);

    propertiesDockWidget = new QDockWidget("Инспектор", this);
    propertiesDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                  QDockWidget::DockWidgetFloatable);
    propertiesScrollArea = new QScrollArea(propertiesDockWidget);
    propertiesDockWidget->setWidget(propertiesScrollArea);
    propertiesScrollArea->setMaximumWidth(300);

    propertiesWidget = new QWidget(propertiesDockWidget);
    propertiesLayout = new QFormLayout(propertiesWidget);
    propertiesWidget->setLayout(propertiesLayout);
    propertiesScrollArea->setWidget(propertiesWidget);
    propertiesScrollArea->setWidgetResizable(true);
    addDockWidget(Qt::RightDockWidgetArea, propertiesDockWidget);


    ui->scene2D->setProperties(propertiesLayout);
    ui->scene2D->setModel(model);
    ui->scene3D->setModel(model);
    ui->scene2D->setAcceptDrops(true);
    ui->scene2D->setFocus();
    ui->tabWidget->setFocusPolicy(Qt::StrongFocus);





    createActions();
    createMenu();
     /*
    createToolBar();
    jsonFileManager = new JSONFileManager(model);
    setFileManager(jsonFileManager);





    //while (toolBox->count()) {
    //    toolBox->removeItem(toolBox->count()-1);
    //}



    /*
    QVBoxLayout* toolLayout = new QVBoxLayout();
    QComboBox* itemsComboBox = new QComboBox();
    toolLayout->addWidget(itemsComboBox);
    toolLayout->addWidget(elementsScrollArea);
    QWidget* toolBoxWidget = new QWidget();
    toolBoxWidget->setLayout(toolLayout);
    */


    elementsDockWidget = new QDockWidget("Элементы", this);
    QScrollArea* elementsScrollArea = new QScrollArea(elementsDockWidget);
    elementsDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                    QDockWidget::DockWidgetFloatable);
    QToolBox* elementsToolBox = new QToolBox(elementsDockWidget);
    elementsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    elementsDockWidget->setWidget(elementsScrollArea);
    elementsScrollArea->setWidgetResizable(true);
    elementsScrollArea->setWidget(elementsToolBox);
    elementsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    addDockWidget(Qt::LeftDockWidgetArea, elementsDockWidget);


    QListWidget* objectList = new ObjectsList(elementsDockWidget);
    connect(objectList, SIGNAL(itemClicked(QListWidgetItem*)), ui->scene2D, SLOT(listItemClicked(QListWidgetItem*)));
    elementsToolBox->addItem(objectList, "Части дорог");



    /*
    QDir dir(QApplication::applicationDirPath());
    ////qDebug() << QApplication::applicationDirPath();
    //QDir dir("models/city_roads/");
    dir.cd("models/city_roads/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.3ds")), "Городские дороги");
    dir.cdUp();
    dir.cd("highway/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.3ds")), "Магистрали");
    dir.cdUp();
    dir.cd("outtown_roads/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.3ds")), "Загородные дороги");
    dir.cdUp();
    dir.cd("elements/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.3ds")), "Элементы");
    dir.cdUp();
    dir.cd("road_signs/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Дорожные знаки");
    dir.cdUp();
    dir.cd("constructions/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Конструкции");
    dir.cdUp();
    dir.cd("plants/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Деревья");
    dir.cdUp();
    dir.cd("buildings/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Здания");
    dir.cdUp();
    dir.cd("humans/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Люди");
    dir.cdUp();
    dir.cd("transport/buses/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Busses");
    dir.cdUp();
    dir.cd("lorries/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Lorries");
    dir.cdUp();
    dir.cd("machinery/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Machinery");
    dir.cdUp();
    dir.cd("scooter/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Scooter");
    dir.cdUp();
    dir.cd("TGV/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "TGV");
    dir.cdUp();
    dir.cd("trucks/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Trucks");
    dir.cdUp();
    dir.cd("cars/");
    elementsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Легковые автомобили");
    */

}

MainWindow::~MainWindow()
{
    Logger::getLogger()->stopLogging();
    Logger::deleteLogger();

    delete ui;
}

void MainWindow::createMenu()
{
    /*
    fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(newAction);
    fileMenu->addSeparator();
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    */

    editMenu = menuBar()->addMenu(tr("&Правка"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    /*
    editMenu->addAction(copyAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);

    toolsMenu = menuBar()->addMenu(tr("&Инструменты"));
    toolsMenu->addAction(showGridAction);
    toolsMenu->addAction(showRoadAction);
    toolsMenu->addAction(showLinesAction);
    toolsMenu->addAction(showProperties);

    contextMenu = new QMenu(this);
    */

}

void MainWindow::createActions()
{

    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);

    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);

    /*
    newAction = new QAction(tr("&Новый"), this);
    //newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Создать новый проект"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Открыть..."), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Открыть существующий проект"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAction = new QAction(tr("&Сохранить"), this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Сохранить проект"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction = new QAction(tr("&Сохранить как..."), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Сохранить проект как..."));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    exitAction = new QAction(tr("Выход"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Закрыть приложение"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    //////////////////////////////////////////

    copyAction = new QAction(tr("&Копировать"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Скопировать объект в буфер обмена"));
    //connect(copyAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    cutAction = new QAction(tr("&Вырезать"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Вырезать объект"));
    //connect(cutAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    pasteAction = new QAction(tr("&Вставить"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Вставить объект из буфера обмена"));
    //connect(pasteAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    deleteAction = new QAction(tr("&Удалить"), this);
    //deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Удалить объект"));
    //connect(deleteAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    ///////////////////////////////////////////

    showGridAction = new QAction(tr("Отображать координатную сетку"), this);
    showGridAction->setCheckable(true);
    //showGridAction->setChecked(spreadsheet->showGrid());
    showGridAction->setStatusTip(tr("Отображать/прятать координатную сетку"));
    showGridAction->setChecked(true);
    connect(showGridAction, SIGNAL(toggled(bool)),ui->scene2D, SLOT(setShowGrid(bool)));

    showRoadAction = new QAction(tr("Отображать дорожное полотно"), this);
    showRoadAction->setCheckable(true);
    //showGridAction->setChecked(spreadsheet->showGrid());
    showRoadAction->setStatusTip(tr("Отображать/прятать дорожное полотно"));
    showRoadAction->setChecked(true);
    showRoadAction->setShortcut(tr("Ctrl+1"));
    //connect(showRoadAction, SIGNAL(toggled(bool)), this->model, SLOT(setRoadVisible(bool)));

    showLinesAction = new QAction(tr("Отображать дорожную разметку"), this);
    showLinesAction->setCheckable(true);
    //showGridAction->setChecked(spreadsheet->showGrid());
    showLinesAction->setStatusTip(tr("Отображать/прятать дорожную разметку"));
    showLinesAction->setChecked(true);
    showLinesAction->setShortcut(tr("Ctrl+2"));
    //connect(showLinesAction, SIGNAL(toggled(bool)), this->model, SLOT(setLinesVilible(bool)));

    //////////////////////////////////////

    rulerAction = new QAction(tr("Линейка"), this);
    rulerAction->setIcon(QIcon("://icons/ruler.png"));
    rulerAction->setCheckable(true);
    rulerAction->setChecked(false);
    //connect(rulerAction, SIGNAL(toggled(bool)), ui->scene2D, SLOT(setRulerActive(bool)));
    //connect(ui->scene2D, SIGNAL(rulerStatusChanged(bool)), rulerAction, SLOT(setChecked(bool)));

    showProperties = new QAction(tr("Инспектор объектов"), this);
    showProperties->setStatusTip(tr("Отображать/прятать инспектор объектов"));
    showProperties->setCheckable(true);
    showProperties->setChecked(true);
    showProperties->setShortcut(tr("Ctrl+I"));
    connect(showProperties, SIGNAL(toggled(bool)), propertiesDockWidget, SLOT(setVisible(bool)));
    */
}

void MainWindow::createToolBar()
{
    //ui->mainToolBar->addAction(rulerAction);
}


void MainWindow::newFile()
{
    /*
    if (model->isModified())
    {
        int r = QMessageBox::warning(this, tr("Spreadsheet"),
                                     tr("Проект был изменен.\n"
                                        "Желаете сохранить изменения?"),
                                     QMessageBox::Yes | QMessageBox::No|
                                     QMessageBox::Cancel);
        if (r == QMessageBox::Yes)
        {
            saveFile();
        }
    }
    model->clear();
    ui->scene2D->updateGL();
    */
}




void MainWindow::openFile()
{
    /*
    QString fileName = QFileDialog::getOpenFileName(this, tr("0pen Spreadsheet"),".",
                                                    tr("Spreadsheet files (*.json)"));

    if (fileManager->openFile(fileName))
    {
        ui->statusBar->showMessage("Файл загружен");
    }
    else
    {
        ui->statusBar->showMessage("Не удалось загрузить файл");
    }
    */
}

void MainWindow::saveFile()
{
    /*
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath(), tr("JSON files (*.json)") );

    if (fileManager->saveFile(fileName))
    {
        ui->statusBar->showMessage("Файл сохранен");
    }
    else
    {
        ui->statusBar->showMessage("Не удалось сохранить файл");
    }
    */
}

/*
void MainWindow::setFileManager(FileManager *manager)
{
    //fileManager = manager;
}
*/


void MainWindow::on_tabWidget_currentChanged(int index)
{
    /*
    switch (index)
    {
    case 0:
        ui->scene2D->setFocus();
        break;
    case 1:
        ui->scene3D->setFocus();
        break;
    default:
        break;
    }
    */
}


void MainWindow::contextMenuEvent(QContextMenuEvent *pe)
{
    //contextMenu->exec(pe->globalPos());
}
