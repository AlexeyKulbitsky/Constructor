#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "roadmodel.h"
#include "sliderdelegate.h"
#include "scene2d.h"
#include <QToolButton>
#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "objectslist.h"
#include <QDockWidget>
#include <QScrollBar>
#include <QToolBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //propertiesToolBar = new QToolBar(this);
    //this->addToolBar(Qt::RightToolBarArea, propertiesToolBar);
    properties = new QFormLayout();
    QWidget *wid = new QWidget();
    wid->setLayout(properties);
    QScrollArea *scroll = new QScrollArea();
    scroll->setWidget(wid);
    scroll->setWidgetResizable(true);
    scroll->setAlignment(Qt::AlignVCenter);

    //QScrollBar *scroll = new QScrollBar(Qt::Vertical, wid);


    //scroll->viewport()->setBackgroundRole(QPalette::Dark);
    //scroll->viewport()->setAutoFillBackground(true);
    //scroll->show();
    //propertiesToolBar->addWidget(scroll);


    propertiesWidget = new QDockWidget("Инспектор");
    propertiesWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    //properties->addWidget(propertiesWidget);
   // propertiesWidget->setLayout(properties);
    propertiesWidget->setWidget(scroll);
    addDockWidget(Qt::RightDockWidgetArea, propertiesWidget);
    //addDockWidget(Qt::BottomDockWidgetArea, scroll);
    //propertiesToolBar->setLayout(properties);
    //properties->addWidget(propertiesToolBar);
    //ui->properties->addWidget(propertiesToolBar);
    model = new Model();
    createActions();
    createMenu();
    createToolBar();
    jsonFileManager = new JSONFileManager(model);
    setFileManager(jsonFileManager);


    // Добавление редактора свойств
    //ed = new QLineEdit(this);
    //QLineEdit* t = new QLineEdit(this);
    //ui->properties->addRow("Ширина",ed);
    //ui->properties->addRow("Длина",t);

    //////ui->scene2D->setProperties(ui->properties);
    ui->scene2D->setProperties(properties);
    ui->scene2D->setModel(model);
    ui->scene2D->setAcceptDrops(true);
    ui->scene2D->setFocus();
    ui->tabWidget->setFocusPolicy(Qt::StrongFocus);
    ui->scene3D->setModel(model);



    //QListWidget* list_1 = new ObjectsList();
    //ui->toolBox->set
    QToolBox* toolBox = new QToolBox(this);
    toolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    //toolBox->setT
    while (toolBox->count()) {
        toolBox->removeItem(toolBox->count()-1);
    }

    ui->scrollAreaToolBox->setWidget(toolBox);
    ui->scrollAreaToolBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    /*
    QScrollArea *scrollAreaToolBox = new QScrollArea();
    scrollAreaToolBox->setWidget(ui->toolBox);
    scrollAreaToolBox->setWidgetResizable(true);
    scrollAreaToolBox->setAlignment(Qt::AlignVCenter);
    scrollAreaToolBox->show();
    */

    QListWidget* objectList = new ObjectsList();
    connect(objectList, SIGNAL(itemClicked(QListWidgetItem*)), ui->scene2D, SLOT(listItemClicked(QListWidgetItem*)));
    toolBox->addItem(objectList, "Элементы");
    //QDir dir("D:/3ds/city_roads/");

    QDir dir(QApplication::applicationDirPath());
    //qDebug() << QApplication::applicationDirPath();
    //QDir dir("models/city_roads/");
    dir.cd("models/city_roads/");
    toolBox->addItem(new ObjectsList(dir, QString("*.3ds")), "Городские дороги");
    dir.cdUp();
    dir.cd("highway/");
    toolBox->addItem(new ObjectsList(dir, QString("*.3ds")), "Магистрали");
    dir.cdUp();
    dir.cd("outtown_roads/");
    toolBox->addItem(new ObjectsList(dir, QString("*.3ds")), "Загородные дороги");
    dir.cdUp();
    dir.cd("elements/");
    toolBox->addItem(new ObjectsList(dir, QString("*.3ds")), "Элементы");
    dir.cdUp();
    dir.cd("road_signs/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Дорожные знаки");
    dir.cdUp();
    dir.cd("constructions/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Конструкции");
    dir.cdUp();
    dir.cd("plants/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Деревья");
    dir.cdUp();
    dir.cd("buildings/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Здания");
    dir.cdUp();
    dir.cd("humans/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Люди");
    dir.cdUp();
    dir.cd("transport/buses/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Busses");
    dir.cdUp();
    dir.cd("lorries/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Lorries");
    dir.cdUp();
    dir.cd("machinery/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Machinery");
    dir.cdUp();
    dir.cd("scooter/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Scooter");
    dir.cdUp();
    dir.cd("TGV/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "TGV");
    dir.cdUp();
    dir.cd("trucks/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Trucks");
    dir.cdUp();
    dir.cd("cars/");
    toolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Легковые автомобили");

    //connect(ui->tabWidget, SIGNAL(currentChanged(int)),
    //        ui->tabWidget->currentWidget(), SLOT(setFocus()));
    //ui->centralWidget = ui->widget;
    /*
    QCheckBox* ruler = new QCheckBox(this);
    ruler->setParent(ui->centralWidget);
    ruler->setText("Линейка");
    connect(ruler, SIGNAL(clicked(bool)),ui->widget, SLOT(setRulerActive(bool)));
    */


    RoadModel* model1 = new RoadModel(this);
    connect(ui->scene2D, SIGNAL(selectedFigureChanged(RoadElement*)), model1, SLOT(setRoadElement(RoadElement*)));





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(newAction);
    fileMenu->addSeparator();
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Правка"));
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

}

void MainWindow::createActions()
{
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
    connect(showRoadAction, SIGNAL(toggled(bool)), this->model, SLOT(setRoadVisible(bool)));

    showLinesAction = new QAction(tr("Отображать дорожную разметку"), this);
    showLinesAction->setCheckable(true);
    //showGridAction->setChecked(spreadsheet->showGrid());
    showLinesAction->setStatusTip(tr("Отображать/прятать дорожную разметку"));
    showLinesAction->setChecked(true);
    showLinesAction->setShortcut(tr("Ctrl+2"));
    connect(showLinesAction, SIGNAL(toggled(bool)), this->model, SLOT(setLinesVilible(bool)));

    //////////////////////////////////////

    rulerAction = new QAction(tr("Линейка"), this);
    rulerAction->setIcon(QIcon("://icons/ruler.png"));
    rulerAction->setCheckable(true);
    rulerAction->setChecked(false);
    connect(rulerAction, SIGNAL(toggled(bool)), ui->scene2D, SLOT(setRulerActive(bool)));
    connect(ui->scene2D, SIGNAL(rulerStatusChanged(bool)), rulerAction, SLOT(setChecked(bool)));

    showProperties = new QAction(tr("Инспектор объектов"), this);
    showProperties->setStatusTip(tr("Отображать/прятать инспектор объектов"));
    showProperties->setCheckable(true);
    showProperties->setChecked(true);
    showProperties->setShortcut(tr("Ctrl+I"));
    connect(showProperties, SIGNAL(toggled(bool)), propertiesWidget, SLOT(setVisible(bool)));
}

void MainWindow::createToolBar()
{
    ui->mainToolBar->addAction(rulerAction);
}


void MainWindow::newFile()
{
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
}




void MainWindow::openFile()
{
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
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::currentPath(), tr("JSON files (*.json)") );

    if (fileManager->saveFile(fileName))
    {
        ui->statusBar->showMessage("Файл сохранен");
    }
    else
    {
        ui->statusBar->showMessage("Не удалось сохранить файл");
    }
}

void MainWindow::setFileManager(FileManager *manager)
{
    fileManager = manager;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
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
}


void MainWindow::contextMenuEvent(QContextMenuEvent *pe)
{
    //contextMenu->exec(pe->globalPos());
}
