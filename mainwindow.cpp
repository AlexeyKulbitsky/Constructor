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
#include <QtWebKitWidgets/QWebView>
#include <QUrl>
#include <QToolBox>
#include "logger.h"
#include <QStackedLayout>
#include "overlayedmapswidget.h"
#include <QWebFrame>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("LpGR","Constructor")
{    
    camera = NULL;
    Logger::getLogger()->startLogging();
    ui->setupUi(this);
    setWindowTitle("Constructor ver.1.2.0");
    model = new Model(this);
    jsonFileManager = new JSONFileManager(model);
    setFileManager(jsonFileManager);
    settingsDialog = new SettingsDialog(&settings, this);

    RoadElement::undoStack = new QUndoStack();////////////????????????????????????
    undoStack = RoadElement::undoStack;



    propertiesDockWidget = new QDockWidget("Инспектор", this);
    propertiesDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                  QDockWidget::DockWidgetFloatable);
    propertiesScrollArea = new QScrollArea(propertiesDockWidget);
    propertiesScrollArea->setFocusPolicy(Qt::StrongFocus);
    propertiesDockWidget->setWidget(propertiesScrollArea);
    propertiesScrollArea->setMaximumWidth(300);

    propertiesWidget = new QWidget(propertiesDockWidget);
    propertiesLayout = new QFormLayout(propertiesWidget);
    propertiesWidget->setLayout(propertiesLayout);
    propertiesScrollArea->setWidget(propertiesWidget);
    propertiesScrollArea->setWidgetResizable(true);
    addDockWidget(Qt::RightDockWidgetArea, propertiesDockWidget);

    scenePropertiesDockWidget = new QDockWidget("Свойства сцены", this);
    scenePropertiesDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                  QDockWidget::DockWidgetFloatable);
    scenePropertiesScrollArea = new QScrollArea(scenePropertiesDockWidget);
    scenePropertiesDockWidget->setWidget(scenePropertiesScrollArea);
    scenePropertiesScrollArea->setMaximumWidth(300);

    scenePropertiesWidget = new QWidget(scenePropertiesDockWidget);
    scenePropertiesLayout = new QFormLayout(scenePropertiesWidget);
    scenePropertiesWidget->setLayout(scenePropertiesLayout);
    scenePropertiesScrollArea->setWidget(scenePropertiesWidget);
    scenePropertiesScrollArea->setWidgetResizable(true);
    addDockWidget(Qt::LeftDockWidgetArea, scenePropertiesDockWidget);

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabWidget_currentChanged(int)));
    QWidget* tabScene2D = new QWidget();

    stackedLayout = new QStackedLayout(tabScene2D);
    tabScene2D->setLayout(stackedLayout);
    yandexMaps = new YandexMapsView(this);

    googleMaps = new GoogleMapsView(this);
    osMaps = new OSMview(this);
    scene2D = new Scene2D(&settings, tabScene2D);
    connect(scene2D, SIGNAL(currentIndexChanged(int)), this, SLOT(setMap(int)));
    scene2D->setOverlayWidget(yandexMaps);
    stackedLayout->addWidget(scene2D);
    stackedLayout->addWidget(yandexMaps);
    stackedLayout->addWidget(googleMaps);
    stackedLayout->addWidget(osMaps);

    scene2D->setProperties(propertiesLayout);
    scene2D->setModel(model);
    scene2D->setAcceptDrops(true);
    scene2D->setFocus();
    ui->tabWidget->setFocusPolicy(Qt::StrongFocus);
    ui->tabWidget->insertTab(0, tabScene2D, "Вид сверху");
    ui->tabWidget->setCurrentIndex(0);

    QWidget* tabScene3D = new QWidget();
    QGridLayout* layout = new QGridLayout(tabScene3D);
    layout->setMargin(0);
    scene3D = new Scene3D(tabScene3D, scene2D);
    //scene3D->setSettings(&settings);
    scene3D->setModel(model);
    layout->addWidget(scene3D);
    ui->tabWidget->insertTab(1, tabScene3D, "3D-вид");

    Camera::readSensors(QApplication::applicationDirPath() + QString("/sensors.json"));

    QWidget* tabCamera = new QWidget();
    QGridLayout* camLayout = new QGridLayout(tabCamera);
    camLayout->setMargin(0);
    camera = new CameraView(tabCamera, scene2D);
    camera->setModel(model);
    jsonFileManager->setCameraView(camera);
    scene2D->setCamera(camera);
    camLayout->addWidget(camera);
    ui->tabWidget->insertTab(2, tabCamera, "Камера");
    //connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabWidget_currentChanged(int)));



    /*
    ui->tabWidget->setFocusPolicy(Qt::StrongFocus);
    */

    createActions();
    createMenu();
    createToolBar();




    /*



    //while (toolBox->count()) {
    //    toolBox->removeItem(toolBox->count()-1);
    //}



    /*
     * ghjjhfghjfgjhfgjh
    QVBoxLayout* toolLayout = new QVBoxLayout();
    QComboBox* itemsComboBox = new QComboBox();
    toolLayout->addWidget(itemsComboBox);
    toolLayout->addWidget(elementsScrollArea);
    QWidget* toolBoxWidget = new QWidget();
    toolBoxWidget->setLayout(toolLayout);
    */


//    elementsDockWidget = new QDockWidget("Элементы", this);
//    QScrollArea* elementsScrollArea = new QScrollArea(elementsDockWidget);
//    elementsDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
//                                    QDockWidget::DockWidgetFloatable);
//    QToolBox* elementsToolBox = new QToolBox(elementsDockWidget);
//    elementsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    elementsDockWidget->setWidget(elementsScrollArea);
//    elementsScrollArea->setWidgetResizable(true);
//    elementsScrollArea->setWidget(elementsToolBox);
//    elementsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    addDockWidget(Qt::LeftDockWidgetArea, elementsDockWidget);
//    this->tabifyDockWidget ( elementsDockWidget, scenePropertiesDockWidget);
//    elementsDockWidget->raise();
//    QListWidget* objectList = new ObjectsList(elementsDockWidget);
//    //connect(objectList, SIGNAL(itemClicked(QListWidgetItem*)), ui->scene2D, SLOT(listItemClicked(QListWidgetItem*)));
//    elementsToolBox->addItem(objectList, "Части дорог");

    elementsDockWidget = new QDockWidget("Элементы", this);
    elementsDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                        QDockWidget::DockWidgetFloatable);
    QWidget* backgroundWidget = new QWidget(elementsDockWidget);
    QVBoxLayout* backgroundLayout = new QVBoxLayout(backgroundWidget);
    backgroundWidget->setLayout(backgroundLayout);
    elementsDockWidget->setWidget(backgroundWidget);

    QToolBox* elementsToolBox = new QToolBox(elementsDockWidget);
    elementsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QScrollArea* elementsScrollArea = new QScrollArea(elementsDockWidget);
    elementsScrollArea->setWidgetResizable(true);
    elementsScrollArea->setWidget(elementsToolBox);
    elementsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QListWidget* objectList = new ObjectsList(elementsDockWidget);
    elementsToolBox->addItem(objectList, "Основные элементы");
    QString dir = "/models/user/";
    ObjectsList* templtatesList = new ObjectsList(dir, QString("*.json"));
    connect(scene2D->stateManager, SIGNAL(templtateAdded()), templtatesList, SLOT(resetObjects()));
    elementsToolBox->addItem(templtatesList, "Пользовательские шаблоны");
    backgroundLayout->addWidget(elementsScrollArea);

    elementsComboBox = new QComboBox(elementsDockWidget);
    elementsComboBox->addItem("Транспорт");
    elementsComboBox->addItem("Люди");
    elementsComboBox->addItem("Деревья, растения");    
    elementsComboBox->addItem("Конструкции");
    elementsComboBox->addItem("Строения");
    elementsComboBox->addItem("Знаки");
    backgroundLayout->addWidget(elementsComboBox);

    elementsLayout = new QStackedLayout();
    backgroundLayout->addLayout(elementsLayout);

    connect(elementsComboBox, SIGNAL(currentIndexChanged(int)), elementsLayout, SLOT(setCurrentIndex(int)));

    QToolBox* transportToolBox = new QToolBox(elementsDockWidget);
    transportToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QScrollArea* transportScrollArea = new QScrollArea(elementsDockWidget);
    transportScrollArea->setWidgetResizable(true);
    transportScrollArea->setWidget(transportToolBox);
    transportScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dir = "/models/transport/buses/";
    transportToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Пассажирский транспорт");
    dir = "/models/transport/lorries/";
    transportToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Для небольших грузов");
//    dir.cdUp();
//    dir.cd("machinery/");
//    transportToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Machinery");
//    dir.cdUp();
//    dir.cd("scooter/");
//    transportToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Scooter");
    dir = "/models/transport/TGV/";
    transportToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Ж/д траспорт");
    dir = "/models/transport/trucks/";
    transportToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Грузовики");
    dir = "/models/transport/cars/";
    transportToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Легковые автомобили");
    elementsLayout->addWidget(transportScrollArea);

    QToolBox* humansToolBox = new QToolBox(elementsDockWidget);
    humansToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QScrollArea* humnasScrollArea = new QScrollArea(elementsDockWidget);
    humnasScrollArea->setWidgetResizable(true);
    humnasScrollArea->setWidget(humansToolBox);
    humnasScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dir = "/models/humans/men/";
    humansToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Мужчины");
    dir = "/models/humans/women/";
    humansToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Женщины");
    elementsLayout->addWidget(humnasScrollArea);

    QToolBox* plantsToolBox = new QToolBox(elementsDockWidget);
    plantsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QScrollArea* plantsScrollArea = new QScrollArea(elementsDockWidget);
    plantsScrollArea->setWidgetResizable(true);
    plantsScrollArea->setWidget(plantsToolBox);
    plantsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dir = "/models/plants/";
    plantsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Деревья");
    elementsLayout->addWidget(plantsScrollArea);

    QToolBox* constructionsToolBox = new QToolBox(elementsDockWidget);
    constructionsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QScrollArea* constructionsScrollArea = new QScrollArea(elementsDockWidget);
    constructionsScrollArea->setWidgetResizable(true);
    constructionsScrollArea->setWidget(constructionsToolBox);
    constructionsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dir = "/models/constructions/poles/";
    constructionsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Столбы");
    dir = "/models/constructions/traffic_lights/";
    constructionsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Светофоры");
    dir = "/models/constructions/bus_stops/";
    constructionsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Остановки");
    //dir.cdUp();
    //constructionsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Остальное");
    elementsLayout->addWidget(constructionsScrollArea);

    QToolBox* buildingsToolBox = new QToolBox(elementsDockWidget);
    buildingsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QScrollArea* buildingsScrollArea = new QScrollArea(elementsDockWidget);
    buildingsScrollArea->setWidgetResizable(true);
    buildingsScrollArea->setWidget(buildingsToolBox);
    buildingsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dir = "/models/buildings/";
    buildingsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "Здания");
    elementsLayout->addWidget(buildingsScrollArea);

    QToolBox* signsToolBox = new QToolBox(elementsDockWidget);
    signsToolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QScrollArea* signsScrollArea = new QScrollArea(elementsDockWidget);
    signsScrollArea->setWidgetResizable(true);
    signsScrollArea->setWidget(signsToolBox);
    signsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dir = "/models/road_signs/1.Предупреждающие/";
    signsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "1.Предупреждающие");
    dir = "/models/road_signs/2.Приоритета/";
    signsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "2.Приоритета");
    dir = "/models/road_signs/3.Запрещающие/";
    signsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "3.Запрещающие");
    dir = "/models/road_signs/4.Предписывающие/";
    signsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "4.Предписывающие");
    dir = "/models/road_signs/5.Особых_предписаний/";
    signsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "5.Особых_предписаний");
    dir = "/models/road_signs/6.Информационные/";
    signsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "6.Информационные");
    dir = "/models/road_signs/7.Сервиса/";
    signsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "7.Сервиса");
    dir = "/models/road_signs/8.Таблички/";
    signsToolBox->addItem(new ObjectsList(dir, QString("*.obj")), "8.Таблички");
    elementsLayout->addWidget(signsScrollArea);

    addDockWidget(Qt::LeftDockWidgetArea, elementsDockWidget);
    this->tabifyDockWidget ( elementsDockWidget, scenePropertiesDockWidget);
    elementsDockWidget->raise();

    /*
    QDir dir(QApplication::applicationDirPath());
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
    readSettings();
}

MainWindow::~MainWindow()
{
    Camera::saveSensors(QApplication::applicationDirPath() + QString("/sensors.json"));
    //scene3D->saveSettings();
    scene2D->saveSettings();
    writeSettings();
    delete undoStack;
    undoStack = NULL;
    RoadElement::undoStack = undoStack;
    settingsDialog->saveLogTreeSettings();

    Logger::getLogger()->stopLogging();
    Logger::deleteLogger();
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
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(deleteAction);




    toolsMenu = menuBar()->addMenu(tr("&Инструменты"));
    toolsMenu->addAction(showGridAction);
    toolsMenu->addAction(showRoadAction);
    toolsMenu->addAction(showLinesAction);
    toolsMenu->addAction(showMapAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(showProperties);    
    toolsMenu->addSeparator();
    toolsMenu->addAction(saveToPresetAction);

    contextMenu = new QMenu(this);


    optionsMenu = menuBar()->addMenu(tr("&Настройки"));
    optionsMenu->addAction(optionsAction);

}

void MainWindow::createActions()
{

    undoAction = undoStack->createUndoAction(this, tr("&Отменить"));
    undoAction->setShortcuts(QKeySequence::Undo);

    redoAction = undoStack->createRedoAction(this, tr("&Повторить"));
    redoAction->setShortcuts(QKeySequence::Redo);


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
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    exitAction = new QAction(tr("Выход"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Закрыть приложение"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    //////////////////////////////////////////

    copyAction = new QAction(tr("&Копировать"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Скопировать объект в буфер обмена"));
    connect(copyAction, SIGNAL(triggered()), scene2D, SLOT(copy()));

    cutAction = new QAction(tr("&Вырезать"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Вырезать объект"));
    connect(cutAction, SIGNAL(triggered()), scene2D, SLOT(cut()));

    pasteAction = new QAction(tr("&Вставить"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Вставить объект из буфера обмена"));
    connect(pasteAction, SIGNAL(triggered()), scene2D, SLOT(paste()));

    deleteAction = new QAction(tr("&Удалить"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Удалить объект"));
    connect(deleteAction, SIGNAL(triggered()), scene2D, SLOT(del()));

    ///////////////////////////////////////////

    showGridAction = new QAction(tr("Отображать координатную сетку"), this);
    showGridAction->setCheckable(true);
    //showGridAction->setChecked(spreadsheet->showGrid());
    showGridAction->setStatusTip(tr("Отображать/прятать координатную сетку"));
    showGridAction->setChecked(true);
    connect(showGridAction, SIGNAL(toggled(bool)),scene2D, SLOT(setShowGrid(bool)));

    showRoadAction = new QAction(tr("Отображать дорожное полотно"), this);
    showRoadAction->setCheckable(true);
    //showGridAction->setChecked(spreadsheet->showGrid());
    showRoadAction->setStatusTip(tr("Отображать/прятать дорожное полотно"));
    showRoadAction->setChecked(true);
    showRoadAction->setShortcut(tr("Ctrl+1"));
    connect(showRoadAction, SIGNAL(toggled(bool)), this->model, SLOT(setRoadVisible(bool)));

    showLinesAction = new QAction(tr("Отображать дорожную разметку"), this);
    showLinesAction->setCheckable(true);
//    //showGridAction->setChecked(spreadsheet->showGrid());
    showLinesAction->setStatusTip(tr("Отображать/прятать дорожную разметку"));
    showLinesAction->setChecked(true);
    showLinesAction->setShortcut(tr("Ctrl+2"));
    connect(showLinesAction, SIGNAL(toggled(bool)), this->model, SLOT(setLinesVilible(bool)));

    showMapAction = new QAction(tr("Отображать карту"), this);
    showMapAction->setCheckable(true);
    showMapAction->setStatusTip(tr("Отображать/прятать карту"));
    showMapAction->setChecked(true);
    showMapAction->setShortcut(tr("Ctrl+3"));
    connect(showMapAction, SIGNAL(toggled(bool)), this->model, SLOT(setMapVisible(bool)));

    showRulerAction = new QAction(tr("Отображать линейку"), this);
    showRulerAction->setCheckable(true);
    showRulerAction->setStatusTip(tr("Отображать/прятать линейку"));
    showRulerAction->setChecked(true);
    showRulerAction->setShortcut(tr("Ctrl+4"));
    connect(showRulerAction, SIGNAL(toggled(bool)), this->model, SLOT(setRulerVisible(bool)));
    //////////////////////////////////////

    rulerAction = new QAction(tr("Линейка"), this);
    rulerAction->setIcon(QIcon("://icons/ruler.png"));
    rulerAction->setCheckable(true);
    rulerAction->setChecked(false);
    connect(rulerAction, SIGNAL(toggled(bool)), scene2D, SLOT(setRulerActive(bool)));
    connect(scene2D, SIGNAL(rulerStatusChanged(bool)), rulerAction, SLOT(setChecked(bool)));

    showProperties = new QAction(tr("Отображать инспектор объектов"), this);
    showProperties->setStatusTip(tr("Отображать/прятать инспектор объектов"));
    showProperties->setCheckable(true);
    showProperties->setChecked(true);
    showProperties->setShortcut(tr("Ctrl+I"));
    connect(showProperties, SIGNAL(toggled(bool)), propertiesDockWidget, SLOT(setVisible(bool)));

    optionsAction = new QAction(tr("Настройки"), this);
    optionsAction->setStatusTip(tr("Показать настройки приложения"));
    connect(optionsAction, SIGNAL(triggered()), settingsDialog, SLOT(exec()));

    saveToPresetAction = new QAction(tr("Сохранить в шаблон"), this);
    saveToPresetAction->setStatusTip(tr("Сохранить объект/сцену в пользовательские шаблоны"));
    connect(saveToPresetAction, SIGNAL(triggered()), scene2D, SLOT(saveToPresets()));
}

void MainWindow::createToolBar()
{
    ui->mainToolBar->addAction(rulerAction);
}

void MainWindow::readSettings()
{
    settings.beginGroup("/Settings/MainWindow");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    settings.beginGroup("/Settings/MainWindow");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
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
    scene2D->updateGL();

}




void MainWindow::openFile()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("0pen Spreadsheet"),QApplication::applicationDirPath(),
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

void MainWindow::saveAsFile()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Document"), QApplication::applicationDirPath(), tr("JSON files (*.json)") );

    if (fileManager->saveFile(fileName))
    {
        ui->statusBar->showMessage("Файл сохранен");
        currentFileName = fileName;
    }
    else
    {
        ui->statusBar->showMessage("Не удалось сохранить файл");
    }

}

void MainWindow::saveFile()
{
    if (currentFileName.size() == 0)
    {
        saveAsFile();
    }
    else
    {
        if (fileManager->saveFile(currentFileName))
        {
            ui->statusBar->showMessage("Файл сохранен");
        }
        else
        {
            ui->statusBar->showMessage("Не удалось сохранить файл");
        }
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
    {
        if (camera)
            camera->setActive(false);
        scene2D->getProperties(scenePropertiesLayout);

    }
        break;
    case 1:
    {
        if (camera)
            camera->setActive(false);
        scene3D->getProperties(scenePropertiesLayout);

    }
        break;
    case 2:
    {        
        if (camera)
            camera->setActive(true);

        camera->getProperties(scenePropertiesLayout);

    }
        break;
    default:
        break;
    }

}

void MainWindow::setMap(int index)
{
    switch (index)
    {
    case 0:
        stackedLayout->setCurrentIndex(index);
        break;
    case 1:
        scene2D->setOverlayWidget(yandexMaps);
        stackedLayout->setCurrentIndex(index);
        break;
    case 2:
        scene2D->setOverlayWidget(googleMaps);
        stackedLayout->setCurrentIndex(index);
        break;
    case 3:
        scene2D->setOverlayWidget(osMaps);
        stackedLayout->setCurrentIndex(index);
        break;
    default:
        break;
    }
}


void MainWindow::contextMenuEvent(QContextMenuEvent*)
{
    //contextMenu->exec(pe->globalPos());
}
