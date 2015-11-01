#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(acceptButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    setLogTreeView();
}

SettingsDialog::SettingsDialog(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(acceptButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    this->settings = settings;
    setLogTreeView();
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

void SettingsDialog::setLoggingRoundingRoad(bool status)
{
    RoundingRoad::setLogging(status);
}

void SettingsDialog::setLoggingIntersection(bool status)
{
    Intersection::setLogging(status);
}

void SettingsDialog::setLoggingCurve(bool status)
{
    Curve::setLogging(status);
}

void SettingsDialog::setLoggingLineSimple(bool status)
{
    LineSimple::setLogging(status);
}

void SettingsDialog::setLoggingLineBroken(bool status)
{
    LineBroken::setLogging(status);
}

void SettingsDialog::setLoggingRailway(bool status)
{
    RailWay::setLogging(status);
}

void SettingsDialog::setLoggingSplitZone(bool status)
{
    SplitZone::setLogging(status);
}

void SettingsDialog::setLoggingScene2D(bool status)
{
    Scene2D::setLogging(status);
}

void SettingsDialog::setLoggingScene3D(bool status)
{
    Scene3D::setLogging(status);
}

void SettingsDialog::setLoggingYandexMaps(bool status)
{
    YandexMapsView::setLogging(status);
}

void SettingsDialog::setLoggingGoogleMaps(bool status)
{
    GoogleMapsView::setLogging(status);
}

void SettingsDialog::setLoggingStateManager(bool status)
{
    StateManager::setLogging(status);
}

void SettingsDialog::setLoggingDefaultState(bool status)
{
    DefaultState::setLogging(status);
}

void SettingsDialog::setLoggingSelectedState(bool status)
{
    SelectedState::setLogging(status);
}

void SettingsDialog::setLoggingRoadBuilderState(bool status)
{
    RoadBuilderState::setLogging(status);
}

void SettingsDialog::setLoggingLineBuilderState(bool status)
{
    LineBuilderState::setLogging(status);
}

void SettingsDialog::setLoggingModel(bool status)
{
    setLoggingRoadBroken(status);
    setLoggingRoadSimple(status);
    setLoggingCompositeRoad(status);
    setLoggingRoundingRoad(status);
    setLoggingIntersection(status);
    setLoggingCurve(status);
    setLoggingLineSimple(status);
    setLoggingLineBroken(status);
    setLoggingRailway(status);
    setLoggingSplitZone(status);
}

void SettingsDialog::setLoggingStates(bool status)
{
    setLoggingStateManager(status);
    setLoggingDefaultState(status);
    setLoggingSelectedState(status);
    setLoggingRoadBuilderState(status);
    setLoggingLineBuilderState(status);
}

void SettingsDialog::setLoggingView(bool status)
{
    setLoggingScene2D(status);
    setLoggingScene3D(status);
    setLoggingYandexMaps(status);
    setLoggingGoogleMaps(status);
}

void SettingsDialog::setLoggingAll(bool status)
{
    setLoggingModel(status);
    setLoggingStates(status);
    setLoggingView(status);
}

void SettingsDialog::processTreeClick(QTreeWidgetItem *item, int column)
{
    Qt::CheckState state = item->checkState(column);
    bool status = state == Qt::Checked ? true : false;
    (this->*logTreeSlots[item])(status);
    for (int i = 0; i < item->childCount(); i++)
    {
        item->child(i)->setCheckState(column, state);
    }
}

void SettingsDialog::okButtonClicked()
{
    acceptButtonClicked();
    this->accept();
}

void SettingsDialog::acceptButtonClicked()
{
    processTreeItemsStates();
    saveLogTreeSettings();
}

void SettingsDialog::cancelButtonClicked()
{
    loadLogTreeSettings();
    this->reject();
}

void SettingsDialog::setLogTreeView()
{
    ui->logTreeWidget->setHeaderLabel("Название");
    connect(ui->logTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(processTreeClick(QTreeWidgetItem*,int)));

    settings->beginGroup("/Settings/Logging/Tree");

    bool checked;
    Qt::CheckState state;
    QTreeWidgetItem *logAllItem = new QTreeWidgetItem(ui->logTreeWidget);
    logAllItem->setFlags(logAllItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All",true).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logAllItem->setCheckState(0, state);
    logAllItem->setText(0, "Всё");
    ui->logTreeWidget->addTopLevelItem(logAllItem);
    logTreeSlots[logAllItem] = &SettingsDialog::setLoggingAll;
    logSettingsNames[logAllItem] = "/All";

    QTreeWidgetItem *logStatesItem = new QTreeWidgetItem(logAllItem);
    logStatesItem->setFlags(logStatesItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/States",true).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logStatesItem->setCheckState(0, state);
    logStatesItem->setText(0, "Состояния");
    logTreeSlots[logStatesItem] = &SettingsDialog::setLoggingStates;
    logSettingsNames[logStatesItem] = "/All/States";

    QTreeWidgetItem *logStateManagerItem = new QTreeWidgetItem(logStatesItem);
    logStateManagerItem->setFlags(logStateManagerItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/States/StateManager",StateManager::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logStateManagerItem->setCheckState(0, state);
    logStateManagerItem->setText(0, "Менеджер состояний");
    logTreeSlots[logStateManagerItem] = &SettingsDialog::setLoggingStateManager;
    logSettingsNames[logStateManagerItem] = "/All/States/StateManager";

    QTreeWidgetItem *logDefaultStateItem = new QTreeWidgetItem(logStatesItem);
    logDefaultStateItem->setFlags(logDefaultStateItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/States/DefaultState", DefaultState::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logDefaultStateItem->setCheckState(0, state);
    logDefaultStateItem->setText(0, "Default state");
    logTreeSlots[logDefaultStateItem] = &SettingsDialog::setLoggingDefaultState;
    logSettingsNames[logDefaultStateItem] = "/All/States/DefaultState";

    QTreeWidgetItem *logSelectedStateItem = new QTreeWidgetItem(logStatesItem);
    logSelectedStateItem->setFlags(logSelectedStateItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/States/SelectedState", SelectedState::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logSelectedStateItem->setCheckState(0, state);
    logSelectedStateItem->setText(0, "Selected state");
    logTreeSlots[logSelectedStateItem] = &SettingsDialog::setLoggingSelectedState;
    logSettingsNames[logSelectedStateItem] = "/All/States/SelectedState";

    QTreeWidgetItem *logRoadBuilderStateItem = new QTreeWidgetItem(logStatesItem);
    logRoadBuilderStateItem->setFlags(logRoadBuilderStateItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/States/RoadBuilderState", RoadBuilderState::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logRoadBuilderStateItem->setCheckState(0, state);
    logRoadBuilderStateItem->setText(0, "Road builder state");
    logTreeSlots[logRoadBuilderStateItem] = &SettingsDialog::setLoggingRoadBuilderState;
    logSettingsNames[logRoadBuilderStateItem] = "/All/States/RoadBuilderState";

    QTreeWidgetItem *logLineBuilderStateItem = new QTreeWidgetItem(logStatesItem);
    logLineBuilderStateItem->setFlags(logLineBuilderStateItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/States/LineBuilderState", LineBuilderState::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logLineBuilderStateItem->setCheckState(0, state);
    logLineBuilderStateItem->setText(0, "Line builder state");
    logTreeSlots[logLineBuilderStateItem] = &SettingsDialog::setLoggingLineBuilderState;
    logSettingsNames[logLineBuilderStateItem] = "/All/States/LineBuilderState";

    QTreeWidgetItem *logModelItem = new QTreeWidgetItem(logAllItem);
    logModelItem->setFlags(logModelItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model", true).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logModelItem->setCheckState(0, state);
    logModelItem->setText(0, "Модель");
    logTreeSlots[logModelItem] = &SettingsDialog::setLoggingModel;
    logSettingsNames[logModelItem] = "/All/Model";

    QTreeWidgetItem *logRoadBrokenItem = new QTreeWidgetItem(logModelItem);
    logRoadBrokenItem->setFlags(logRoadBrokenItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/RoadBroken", RoadBroken::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logRoadBrokenItem->setCheckState(0, state);
    logRoadBrokenItem->setText(0, "Дорога ломаная");
    logTreeSlots[logRoadBrokenItem] = &SettingsDialog::setLoggingRoadBroken;
    logSettingsNames[logRoadBrokenItem] = "/All/Model/RoadBroken";

    QTreeWidgetItem *logRoadSimpleItem = new QTreeWidgetItem(logModelItem);
    logRoadSimpleItem->setFlags(logRoadSimpleItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/RoadSimple", RoadSimple::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logRoadSimpleItem->setCheckState(0, state);
    logRoadSimpleItem->setText(0, "Дорога простая");
    logTreeSlots[logRoadSimpleItem] = &SettingsDialog::setLoggingRoadSimple;
    logSettingsNames[logRoadSimpleItem] = "/All/Model/RoadSimple";

    QTreeWidgetItem *logCompositeRoadItem = new QTreeWidgetItem(logModelItem);
    logCompositeRoadItem->setFlags(logCompositeRoadItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/CompositeRoad", CompositeRoad::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logCompositeRoadItem->setCheckState(0, state);
    logCompositeRoadItem->setText(0, "Дорога составная");
    logTreeSlots[logCompositeRoadItem] = &SettingsDialog::setLoggingCompositeRoad;
    logSettingsNames[logCompositeRoadItem] = "/All/Model/CompositeRoad";

    QTreeWidgetItem *logRoundingRoadItem = new QTreeWidgetItem(logModelItem);
    logRoundingRoadItem->setFlags(logRoundingRoadItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/RoundingRoad", RoundingRoad::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logRoundingRoadItem->setCheckState(0, state);
    logRoundingRoadItem->setText(0, "Поворот дороги");
    logTreeSlots[logRoundingRoadItem] = &SettingsDialog::setLoggingRoundingRoad;
    logSettingsNames[logRoundingRoadItem] = "/All/Model/RoundingRoad";

    QTreeWidgetItem *logIntersectionItem = new QTreeWidgetItem(logModelItem);
    logIntersectionItem->setFlags(logIntersectionItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/Intersection", Intersection::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logIntersectionItem->setCheckState(0, state);
    logIntersectionItem->setText(0, "Перекресток");
    logTreeSlots[logIntersectionItem] = &SettingsDialog::setLoggingIntersection;
    logSettingsNames[logIntersectionItem] = "/All/Model/Intersection";

    QTreeWidgetItem *logCurveItem = new QTreeWidgetItem(logModelItem);
    logCurveItem->setFlags(logCurveItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/Curve", Curve::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logCurveItem->setCheckState(0, state);
    logCurveItem->setText(0, "Закругление");
    logTreeSlots[logCurveItem] = &SettingsDialog::setLoggingCurve;
    logSettingsNames[logCurveItem] = "/All/Model/Curve";

    QTreeWidgetItem *logLineSimpleItem = new QTreeWidgetItem(logModelItem);
    logLineSimpleItem->setFlags(logLineSimpleItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/LineSimple", LineSimple::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logLineSimpleItem->setCheckState(0, state);
    logLineSimpleItem->setText(0, "Линия простая");
    logTreeSlots[logLineSimpleItem] = &SettingsDialog::setLoggingLineSimple;
    logSettingsNames[logLineSimpleItem] = "/All/Model/LineSimple";

    QTreeWidgetItem *logLineBrokenItem = new QTreeWidgetItem(logModelItem);
    logLineBrokenItem->setFlags(logLineBrokenItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/LineBroken", LineBroken::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logLineBrokenItem->setCheckState(0, state);
    logLineBrokenItem->setText(0, "Линия ломаная");
    logTreeSlots[logLineBrokenItem] = &SettingsDialog::setLoggingLineBroken;
    logSettingsNames[logLineBrokenItem] = "/All/Model/LineBroken";

    QTreeWidgetItem *logRailWayItem = new QTreeWidgetItem(logModelItem);
    logRailWayItem->setFlags(logRailWayItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/Railway", RailWay::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logRailWayItem->setCheckState(0, state);
    logRailWayItem->setText(0, "Железная дорога");
    logTreeSlots[logRailWayItem] = &SettingsDialog::setLoggingRailway;
    logSettingsNames[logRailWayItem] = "/All/Model/Railway";

    QTreeWidgetItem *logSplitZoneItem = new QTreeWidgetItem(logModelItem);
    logSplitZoneItem->setFlags(logSplitZoneItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/Model/SplitZone", SplitZone::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logSplitZoneItem->setCheckState(0, state);
    logSplitZoneItem->setText(0, "Разделительная зона");
    logTreeSlots[logSplitZoneItem] = &SettingsDialog::setLoggingSplitZone;
    logSettingsNames[logSplitZoneItem] = "/All/Model/SplitZone";

    QTreeWidgetItem *logViewItem = new QTreeWidgetItem(logAllItem);
    logViewItem->setFlags(logViewItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/View", true).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logViewItem->setCheckState(0, state);
    logViewItem->setText(0, "Вид");
    logTreeSlots[logViewItem] = &SettingsDialog::setLoggingView;
    logSettingsNames[logViewItem] = "/All/View";

    QTreeWidgetItem *logScene2DItem = new QTreeWidgetItem(logViewItem);
    logScene2DItem->setFlags(logScene2DItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/View/Scene2D", Scene2D::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logScene2DItem->setCheckState(0, state);
    logScene2DItem->setText(0, "Вид сверху");
    logTreeSlots[logScene2DItem] = &SettingsDialog::setLoggingScene2D;
    logSettingsNames[logScene2DItem] = "/All/View/Scene2D";

    QTreeWidgetItem *logScene3DItem = new QTreeWidgetItem(logViewItem);
    logScene3DItem->setFlags(logScene3DItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/View/Scene3D", Scene3D::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logScene3DItem->setCheckState(0, state);
    logScene3DItem->setText(0, "3 - D вид");
    logTreeSlots[logScene3DItem] = &SettingsDialog::setLoggingScene3D;
    logSettingsNames[logScene3DItem] = "/All/View/Scene3D";

    QTreeWidgetItem *logYandexMapsItem = new QTreeWidgetItem(logViewItem);
    logYandexMapsItem->setFlags(logYandexMapsItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/View/YandexMaps", YandexMapsView::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logYandexMapsItem->setCheckState(0, state);
    logYandexMapsItem->setText(0, "Яндекс.Карты");
    logTreeSlots[logYandexMapsItem] = &SettingsDialog::setLoggingYandexMaps;
    logSettingsNames[logYandexMapsItem] = "/All/View/YandexMaps";

    QTreeWidgetItem *logGoogleMapsItem = new QTreeWidgetItem(logViewItem);
    logGoogleMapsItem->setFlags(logGoogleMapsItem->flags() | Qt::ItemIsUserCheckable);
    checked = settings->value("/All/View/GoogleMaps", GoogleMapsView::getLogging()).toBool();
    state = checked ? Qt::Checked : Qt::Unchecked;
    logGoogleMapsItem->setCheckState(0, state);
    logGoogleMapsItem->setText(0, "Google Maps");
    logTreeSlots[logGoogleMapsItem] = &SettingsDialog::setLoggingGoogleMaps;
    logSettingsNames[logGoogleMapsItem] = "/All/View/GoogleMaps";

    settings->endGroup();
    processTreeItemsStates();
}

void SettingsDialog::saveLogTreeSettings()
{
    settings->beginGroup("/Settings/Logging/Tree");
    bool checked;
    Qt::CheckState state;
    QTreeWidgetItemIterator it(ui->logTreeWidget);
    while (*it)
    {
        state = (*it)->checkState(0);
        checked = state == Qt::Checked ? true : false;
        settings->setValue( logSettingsNames[(*it)], checked);
        ++it;
    }
    settings->endGroup();
}

void SettingsDialog::loadLogTreeSettings()
{
    settings->beginGroup("/Settings/Logging/Tree");
    bool checked;
    Qt::CheckState state;
    QTreeWidgetItemIterator it(ui->logTreeWidget);
    while (*it)
    {
        checked = settings->value( logSettingsNames[(*it)], checked).toBool();
        state = checked ? Qt::Checked : Qt::Unchecked;
        (*it)->setCheckState(0, state);
        ++it;
    }
    settings->endGroup();

    processTreeItemsStates();
}

void SettingsDialog::processTreeItemsStates()
{
    bool checked;
    Qt::CheckState state;
    QTreeWidgetItemIterator it(ui->logTreeWidget);
    while (*it)
    {
        state = (*it)->checkState(0);
        checked = state == Qt::Checked ? true : false;
        (this->*logTreeSlots[(*it)])(checked);
        ++it;
    }
}

void SettingsDialog::setSettings(QSettings *settings)
{
    this->settings = settings;
}
