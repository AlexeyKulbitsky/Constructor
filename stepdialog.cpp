#include "stepdialog.h"


StepDialog::
StepDialog(QWidget *parent):QDialog(parent)
{
    rightSide = true;
    endRounding = beginRounding = true;
    splitZoneWidth = 1.0;
    step = 1.0f;
    lineType = 0;
    beginSide = true;
    differentDirections = true;
    beginStep = 0.0;
    endStep = 0.0;
    singleWay = true;
    axisStep = 0.0;
    splitZoneHeight = 0.1;
    splitZoneType = 0;
    layout = new QStackedLayout();
    //layout->setStackingMode(QStackedLayout::StackAll);
    setDefaultLayout();
    setSplitZoneLayout();
    setStopLineLayout();
    setTramwaysLayout();
    setLayout(layout);

}

StepDialog::~StepDialog()
{
    if (layout)
        delete layout;
    layout = NULL;
    if (defaultLayout)
        delete defaultLayout;
    defaultLayout = NULL;
    if (stopLineLayout)
        delete stopLineLayout;
    stopLineLayout = NULL;
    if (splitZoneLayout)
        delete splitZoneLayout;
    splitZoneLayout = NULL;
    if (tramWaysLayout)
        delete tramWaysLayout;
    tramWaysLayout = NULL;
}

void StepDialog::setDefaultLayout()
{
    QLabel *lineTypeLabel = new QLabel("Тип линии");
    QComboBox *lineTypeComboBox = new QComboBox();
    lineTypeComboBox->addItem("Сплошная");
    lineTypeComboBox->addItem("Прерывистая");
    lineTypeComboBox->addItem("Двойная сплошная");
    lineTypeComboBox->addItem("Двойная прерывистая/сплошная левая");
    lineTypeComboBox->addItem("Двойная прерывистая/сплошная правая");
    lineTypeComboBox->addItem("Двойная прерывистая");
    lineTypeComboBox->addItem("Разделительная зона");
    lineTypeComboBox->addItem("Стоп-линия");
    lineTypeComboBox->addItem("Трамвайные пути");
    connect(lineTypeComboBox, SIGNAL(activated(int)), this, SLOT(setLineType(int)));
    connect(this, SIGNAL(lineTypeChanged(int)), lineTypeComboBox, SLOT(setCurrentIndex(int)));

    QHBoxLayout *lineTypeLayout = new QHBoxLayout();
    lineTypeLayout->addWidget(lineTypeLabel);
    lineTypeLayout->addWidget(lineTypeComboBox);


    QPushButton *okPushButton = new QPushButton("OK");
    connect(okPushButton, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    QPushButton *cancelPushButton = new QPushButton("Cancel");
    connect(cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(okPushButton);
    buttonsLayout->addWidget(cancelPushButton);


    QRadioButton *leftSideRadioButton = new QRadioButton("Привязать к левой стороне");
    QRadioButton *rightSideRadioButton = new QRadioButton("Привязать к правой стороне");
    connect(rightSideRadioButton, SIGNAL(toggled(bool)), this, SLOT(setRightSide(bool)));
    connect(this, SIGNAL(rightSideChanged(bool)), rightSideRadioButton, SLOT(setChecked(bool)));

    rightSideRadioButton->setChecked(true);

    QDoubleSpinBox *beginStepDoubleSpinBox = new QDoubleSpinBox();
    beginStepDoubleSpinBox->setMinimum(0.0);
    QLabel *beginStepLabel = new QLabel("Отступ от начала");
    connect(beginStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setBeginStep(double)));
    connect(this, SIGNAL(beginStepChanged(double)), beginStepDoubleSpinBox, SLOT(setValue(double)));
    beginStepDoubleSpinBox->setValue(beginStep);

    QHBoxLayout *beginStepLayout = new QHBoxLayout();
    beginStepLayout->addWidget(beginStepLabel);
    beginStepLayout->addWidget(beginStepDoubleSpinBox);

    QDoubleSpinBox *endStepDoubleSpinBox = new QDoubleSpinBox();
    endStepDoubleSpinBox->setMinimum(0.0);
    QLabel *endStepLabel = new QLabel("Отступ с конца");
    connect(endStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setEndStep(double)));
    connect(this, SIGNAL(endStepChanged(double)), endStepDoubleSpinBox, SLOT(setValue(double)));
    endStepDoubleSpinBox->setValue(endStep);

    QHBoxLayout *endStepLayout = new QHBoxLayout();
    endStepLayout->addWidget(endStepLabel);
    endStepLayout->addWidget(endStepDoubleSpinBox);

    QLabel *lineStepLabel = new QLabel("Отступ");
    QDoubleSpinBox *lineStepDoubleSpinBox = new QDoubleSpinBox();
    lineStepDoubleSpinBox->setMinimum(0.0f);
    connect(lineStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setStep(double)));
    connect(this, SIGNAL(stepChanged(double)), lineStepDoubleSpinBox, SLOT(setValue(double)));
    lineStepDoubleSpinBox->setValue(step);

    QHBoxLayout *lineStepLayout = new QHBoxLayout();
    lineStepLayout->addWidget(lineStepLabel);
    lineStepLayout->addWidget(lineStepDoubleSpinBox);

    QRadioButton *oneDirectionRadionButton = new QRadioButton("Разделяет потоки одного направления");
    QRadioButton *differentDirectionsRadioButton = new QRadioButton("Разделяет потоки противоположных направлений");
    connect(differentDirectionsRadioButton, SIGNAL(toggled(bool)), this, SLOT(setDifferentDirection(bool)));
    connect(this, SIGNAL(differentDirectionsChanged(bool)), differentDirectionsRadioButton, SLOT(setChecked(bool)));

    differentDirectionsRadioButton->setChecked(true);
    QVBoxLayout *directionsLayout = new QVBoxLayout();
    directionsLayout->addWidget(differentDirectionsRadioButton);
    directionsLayout->addWidget(oneDirectionRadionButton);
    QGroupBox *directionsGroupBox = new QGroupBox();
    directionsGroupBox->setLayout(directionsLayout);


    QVBoxLayout *leftRightLayout = new QVBoxLayout();
    leftRightLayout->addWidget(rightSideRadioButton);
    leftRightLayout->addWidget(leftSideRadioButton);
    leftRightLayout->addWidget(directionsGroupBox);
    leftRightLayout->addLayout(lineStepLayout);
    leftRightLayout->addLayout(beginStepLayout);
    leftRightLayout->addLayout(endStepLayout);
    QGroupBox *defaultGroupBox = new QGroupBox();
    defaultGroupBox->setLayout(leftRightLayout);


    defaultLayout = new QVBoxLayout();
    defaultLayout->addLayout(lineTypeLayout);
    defaultLayout->addWidget(defaultGroupBox);
    defaultLayout->addLayout(buttonsLayout);

    QWidget *widget = new QWidget();
    widget->setLayout(defaultLayout);
    layout->addWidget(widget);
}

void StepDialog::setStopLineLayout()
{
    QLabel *lineTypeLabel = new QLabel("Тип линии");
    QComboBox *lineTypeComboBox = new QComboBox();
    lineTypeComboBox->addItem("Сплошная");
    lineTypeComboBox->addItem("Прерывистая");
    lineTypeComboBox->addItem("Двойная сплошная");
    lineTypeComboBox->addItem("Двойная прерывистая/сплошная левая");
    lineTypeComboBox->addItem("Двойная прерывистая/сплошная правая");
    lineTypeComboBox->addItem("Двойная прерывистая");
    lineTypeComboBox->addItem("Разделительная зона");
    lineTypeComboBox->addItem("Стоп-линия");
    lineTypeComboBox->addItem("Трамвайные пути");
    connect(lineTypeComboBox, SIGNAL(activated(int)), this, SLOT(setLineType(int)));
    connect(this, SIGNAL(lineTypeChanged(int)), lineTypeComboBox, SLOT(setCurrentIndex(int)));

    QHBoxLayout *lineTypeLayout = new QHBoxLayout();
    lineTypeLayout->addWidget(lineTypeLabel);
    lineTypeLayout->addWidget(lineTypeComboBox);


    QPushButton *okPushButton = new QPushButton("OK");
    connect(okPushButton, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    QPushButton *cancelPushButton = new QPushButton("Cancel");
    connect(cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(okPushButton);
    buttonsLayout->addWidget(cancelPushButton);


    QRadioButton *leftSideRadioButton = new QRadioButton("Отсчет вдоль левой стороны");
    leftSideRadioButton->setToolTip("Если у полотна длины правой и левой сторон отличаются, позволяет выбрать ту сторону,\nвдоль которой будет отсчитываться отступ от края дороги (от начала или конца)");
    QRadioButton *rightSideRadioButton = new QRadioButton("Отсчет вдоль правой стороны");
    rightSideRadioButton->setToolTip("Если у полотна длины правой и левой сторон отличаются, позволяет выбрать ту сторону,\nвдоль которой будет отсчитываться отступ от края дороги (от начала или конца)");
    connect(rightSideRadioButton, SIGNAL(toggled(bool)), this, SLOT(setRightSide(bool)));
    connect(this, SIGNAL(rightSideChanged(bool)), rightSideRadioButton, SLOT(setChecked(bool)));
    rightSideRadioButton->setChecked(true);

    QLabel *lineStepLabel = new QLabel("Отступ");
    QDoubleSpinBox *lineStepDoubleSpinBox = new QDoubleSpinBox();
    lineStepDoubleSpinBox->setMinimum(0.0f);
    connect(lineStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setStep(double)));
    connect(this, SIGNAL(stepChanged(double)), lineStepDoubleSpinBox, SLOT(setValue(double)));
    QHBoxLayout *lineStepLayout = new QHBoxLayout();
    lineStepLayout->addWidget(lineStepLabel);
    lineStepLayout->addWidget(lineStepDoubleSpinBox);

    QVBoxLayout *leftRightLayout = new QVBoxLayout();
    leftRightLayout->addWidget(leftSideRadioButton);
    leftRightLayout->addWidget(rightSideRadioButton);
    //leftRightLayout->addLayout(lineStepLayout);

    QGroupBox *leftRightGroupBox = new QGroupBox();
    leftRightGroupBox->setLayout(leftRightLayout);

    ///////////////////////////

    QRadioButton *beginRadioButton = new QRadioButton("Отсчет от начала");
    beginRadioButton->setToolTip("Производить отсчет от начала дороги");
    QRadioButton *endRadioButton = new QRadioButton("Отсчет с конца");
    endRadioButton->setToolTip("Производить отсчет с конца дороги");
    connect(beginRadioButton, SIGNAL(toggled(bool)), this, SLOT(setBeginSide(bool)));
    connect(this, SIGNAL(beginSideChanged(bool)), beginRadioButton, SLOT(setChecked(bool)));
    beginRadioButton->setChecked(true);

    QVBoxLayout *beginEndLayout = new QVBoxLayout();
    beginEndLayout->addWidget(beginRadioButton);
    beginEndLayout->addWidget(endRadioButton);

    QGroupBox *beginEndGroupBox = new QGroupBox();
    beginEndGroupBox->setLayout(beginEndLayout);

    QVBoxLayout *stopLayout = new QVBoxLayout();
    stopLayout->addWidget(beginEndGroupBox);
    stopLayout->addWidget(leftRightGroupBox);
    stopLayout->addLayout(lineStepLayout);

    QGroupBox *stopLineGroupBox = new QGroupBox();
    stopLineGroupBox->setLayout(stopLayout);


    stopLineLayout = new QVBoxLayout();
    stopLineLayout->addLayout(lineTypeLayout);
    stopLineLayout->addWidget(stopLineGroupBox);
    stopLineLayout->addLayout(buttonsLayout);

    QWidget *widget = new QWidget();
    widget->setLayout(stopLineLayout);
    layout->addWidget(widget);
}

void StepDialog::setSplitZoneLayout()
{
    QLabel *lineTypeLabel = new QLabel("Тип линии");
    QComboBox *lineTypeComboBox = new QComboBox();
    lineTypeComboBox->addItem("Сплошная");
    lineTypeComboBox->addItem("Прерывистая");
    lineTypeComboBox->addItem("Двойная сплошная");
    lineTypeComboBox->addItem("Двойная прерывистая/сплошная левая");
    lineTypeComboBox->addItem("Двойная прерывистая/сплошная правая");
    lineTypeComboBox->addItem("Двойная прерывистая");
    lineTypeComboBox->addItem("Разделительная зона");
    lineTypeComboBox->addItem("Стоп-линия");
    lineTypeComboBox->addItem("Трамвайные пути");
    connect(lineTypeComboBox, SIGNAL(activated(int)), this, SLOT(setLineType(int)));
    connect(this, SIGNAL(lineTypeChanged(int)), lineTypeComboBox, SLOT(setCurrentIndex(int)));

    QHBoxLayout *lineTypeLayout = new QHBoxLayout();
    lineTypeLayout->addWidget(lineTypeLabel);
    lineTypeLayout->addWidget(lineTypeComboBox);

    QLabel *splitZoneTypeLabel = new QLabel("Тип разделительной зоны");
    QComboBox *splitZoneTypeComboBox = new QComboBox();
    splitZoneTypeComboBox->addItem("Разметка");
    splitZoneTypeComboBox->addItem("Газон");
    splitZoneTypeComboBox->addItem("Тротуар");
    connect(splitZoneTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSplitZoneType(int)));
    connect(this, SIGNAL(splitZoneTypeChanged(int)), splitZoneTypeComboBox, SLOT(setCurrentIndex(int)));

    QHBoxLayout *splitZoneTypeLayout = new QHBoxLayout();
    splitZoneTypeLayout->addWidget(splitZoneTypeLabel);
    splitZoneTypeLayout->addWidget(splitZoneTypeComboBox);

    QDoubleSpinBox* splitZoneHeightSpinBox = new QDoubleSpinBox();
    splitZoneHeightSpinBox->setMinimum(0.0);
    splitZoneHeightSpinBox->setValue(splitZoneHeight);
    splitZoneHeightSpinBox->setEnabled(false);
    connect(splitZoneHeightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSplitZoneHeight(double)));
    connect(this, SIGNAL(splitZoneHeightChanged(double)), splitZoneHeightSpinBox, SLOT(setValue(double)));
    connect(this, SIGNAL(splitZoneHeightEnabledChanged(bool)), splitZoneHeightSpinBox, SLOT(setEnabled(bool)));

    QLabel* splitZoneHeightLabel = new QLabel("Высота");

    QHBoxLayout* splitZoneHeightLayout = new QHBoxLayout();
    splitZoneHeightLayout->addWidget(splitZoneHeightLabel);
    splitZoneHeightLayout->addWidget(splitZoneHeightSpinBox);

    QPushButton *okPushButton = new QPushButton("OK");
    connect(okPushButton, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    QPushButton *cancelPushButton = new QPushButton("Cancel");
    connect(cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(okPushButton);
    buttonsLayout->addWidget(cancelPushButton);


    QRadioButton *leftSideRadioButton = new QRadioButton("Привязать к левой стороне");
    QRadioButton *rightSideRadioButton = new QRadioButton("Привязать к правой стороне");
    connect(rightSideRadioButton, SIGNAL(toggled(bool)), this, SLOT(setRightSide(bool)));
    connect(this, SIGNAL(rightSideChanged(bool)), rightSideRadioButton, SLOT(setChecked(bool)));
    rightSideRadioButton->setChecked(true);

    QDoubleSpinBox *beginStepDoubleSpinBox = new QDoubleSpinBox();
    beginStepDoubleSpinBox->setMinimum(0.0);
    QLabel *beginStepLabel = new QLabel("Отступ от начала");
    connect(beginStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setBeginStep(double)));
    connect(this, SIGNAL(beginStepChanged(double)), beginStepDoubleSpinBox, SLOT(setValue(double)));

    QHBoxLayout *beginStepLayout = new QHBoxLayout();
    beginStepLayout->addWidget(beginStepLabel);
    beginStepLayout->addWidget(beginStepDoubleSpinBox);

    QDoubleSpinBox *endStepDoubleSpinBox = new QDoubleSpinBox();
    endStepDoubleSpinBox->setMinimum(0.0);
    QLabel *endStepLabel = new QLabel("Отступ с конца");
    connect(endStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setEndStep(double)));
    connect(this, SIGNAL(endStepChanged(double)), endStepDoubleSpinBox, SLOT(setValue(double)));

    QHBoxLayout *endStepLayout = new QHBoxLayout();
    endStepLayout->addWidget(endStepLabel);
    endStepLayout->addWidget(endStepDoubleSpinBox);

    QLabel *lineStepLabel = new QLabel("Отступ");
    QDoubleSpinBox *lineStepDoubleSpinBox = new QDoubleSpinBox();
    lineStepDoubleSpinBox->setMinimum(0.0f);
    connect(lineStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setStep(double)));
    connect(this, SIGNAL(stepChanged(double)), lineStepDoubleSpinBox, SLOT(setValue(double)));

    QHBoxLayout *lineStepLayout = new QHBoxLayout();
    lineStepLayout->addWidget(lineStepLabel);
    lineStepLayout->addWidget(lineStepDoubleSpinBox);

    QCheckBox *beginRoundingCheckBox = new QCheckBox("Закругление вначале");
    beginRoundingCheckBox->setChecked(beginRounding);
    connect(beginRoundingCheckBox, SIGNAL(toggled(bool)), this, SLOT(setBeginRoundingStatus(bool)));
    connect(this, SIGNAL(beginRoundingChanged(bool)), beginRoundingCheckBox, SLOT(setChecked(bool)));

    QCheckBox *endRoundingCheckBox = new QCheckBox("Закругление вконце");
    endRoundingCheckBox->setChecked(endRounding);
    connect(endRoundingCheckBox, SIGNAL(toggled(bool)), this, SLOT(setEndRoundingStatus(bool)));
    connect(this, SIGNAL(endRoundingChanged(bool)), endRoundingCheckBox, SLOT(setChecked(bool)));

    QDoubleSpinBox *splitZoneWidthSpinBox = new QDoubleSpinBox();
    splitZoneWidthSpinBox->setMinimum(0.0);
    splitZoneWidthSpinBox->setValue(splitZoneWidth);
    connect(splitZoneWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSplitZoneWidth(double)));
    connect(this, SIGNAL(splitZoneWidthChanged(double)), splitZoneWidthSpinBox, SLOT(setValue(double)));

    QLabel *splitZoneWidthLabel = new QLabel("Ширина");

    QHBoxLayout *splitZoneWidthLayout = new QHBoxLayout();
    splitZoneWidthLayout->addWidget(splitZoneWidthLabel);
    splitZoneWidthLayout->addWidget(splitZoneWidthSpinBox);

    QVBoxLayout *leftRightLayout = new QVBoxLayout();
    leftRightLayout->addWidget(rightSideRadioButton);
    leftRightLayout->addWidget(leftSideRadioButton);
    leftRightLayout->addWidget(beginRoundingCheckBox);
    leftRightLayout->addWidget(endRoundingCheckBox);
    leftRightLayout->addLayout(splitZoneWidthLayout);
    //leftRightLayout->addWidget(directionsGroupBox);
    leftRightLayout->addLayout(lineStepLayout);
    leftRightLayout->addLayout(beginStepLayout);
    leftRightLayout->addLayout(endStepLayout);
    QGroupBox *defaultGroupBox = new QGroupBox();
    defaultGroupBox->setLayout(leftRightLayout);


    splitZoneLayout = new QVBoxLayout();
    splitZoneLayout->addLayout(lineTypeLayout);
    splitZoneLayout->addLayout(splitZoneTypeLayout);
    splitZoneLayout->addLayout(splitZoneHeightLayout);
    splitZoneLayout->addWidget(defaultGroupBox);
    splitZoneLayout->addLayout(buttonsLayout);

    QWidget *widget = new QWidget();
    widget->setLayout(splitZoneLayout);
    layout->addWidget(widget);
}

void StepDialog::setTramwaysLayout()
{
    QLabel *lineTypeLabel = new QLabel("Тип линии");
    QComboBox *lineTypeComboBox = new QComboBox();
    lineTypeComboBox->addItem("Сплошная");
    lineTypeComboBox->addItem("Прерывистая");
    lineTypeComboBox->addItem("Двойная сплошная");
    lineTypeComboBox->addItem("Двойная прерывистая/сплошная левая");
    lineTypeComboBox->addItem("Двойная прерывистая/сплошная правая");
    lineTypeComboBox->addItem("Двойная прерывистая");
    lineTypeComboBox->addItem("Разделительная зона");
    lineTypeComboBox->addItem("Стоп-линия");
    lineTypeComboBox->addItem("Трамвайные пути");
    connect(lineTypeComboBox, SIGNAL(activated(int)), this, SLOT(setLineType(int)));
    connect(this, SIGNAL(lineTypeChanged(int)), lineTypeComboBox, SLOT(setCurrentIndex(int)));

    QHBoxLayout *lineTypeLayout = new QHBoxLayout();
    lineTypeLayout->addWidget(lineTypeLabel);
    lineTypeLayout->addWidget(lineTypeComboBox);


    QPushButton *okPushButton = new QPushButton("OK");
    connect(okPushButton, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    QPushButton *cancelPushButton = new QPushButton("Cancel");
    connect(cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(okPushButton);
    buttonsLayout->addWidget(cancelPushButton);


    QRadioButton *singleWayRadioButton = new QRadioButton("В одном направлении");
    QRadioButton *doubleWayRadioButton = new QRadioButton("В двух направлениях");
    connect(singleWayRadioButton, SIGNAL(toggled(bool)), this, SLOT(setSingleWay(bool)));
    connect(this, SIGNAL(singleWayChanged(bool)), singleWayRadioButton, SLOT(setChecked(bool)));

    singleWayRadioButton->setChecked(true);
    QLabel* waysStepLabel = new QLabel("Расстояние между осями");
    QDoubleSpinBox* waysStepSpinBox = new QDoubleSpinBox();
    waysStepSpinBox->setMinimum(0.0);
    waysStepSpinBox->setValue(0.0);
    waysStepSpinBox->setEnabled(false);
    connect(doubleWayRadioButton, SIGNAL(toggled(bool)), waysStepSpinBox, SLOT(setEnabled(bool)));
    connect(waysStepSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAxisStep(double)));
    connect(this, SIGNAL(axisStepChanged(double)), waysStepSpinBox, SLOT(setValue(double)));

    QHBoxLayout* waysStepLayout = new QHBoxLayout();
    waysStepLayout->addWidget(waysStepLabel);
    waysStepLayout->addWidget(waysStepSpinBox);
    QVBoxLayout* singleDoubleLayout = new QVBoxLayout();
    singleDoubleLayout->addWidget(singleWayRadioButton);
    singleDoubleLayout->addWidget(doubleWayRadioButton);
    singleDoubleLayout->addLayout(waysStepLayout);
    QGroupBox* singleDoubleGroupBox = new QGroupBox();
    singleDoubleGroupBox->setLayout(singleDoubleLayout);

    QRadioButton *leftSideRadioButton = new QRadioButton("Привязать к левой стороне");
    QRadioButton *rightSideRadioButton = new QRadioButton("Привязать к правой стороне");
    connect(rightSideRadioButton, SIGNAL(toggled(bool)), this, SLOT(setRightSide(bool)));
    connect(this, SIGNAL(rightSideChanged(bool)), rightSideRadioButton, SLOT(setChecked(bool)));

    rightSideRadioButton->setChecked(true);



    QLabel *lineStepLabel = new QLabel("Отступ");
    QDoubleSpinBox *lineStepDoubleSpinBox = new QDoubleSpinBox();
    lineStepDoubleSpinBox->setMinimum(0.0f);
    connect(lineStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setStep(double)));
    connect(this, SIGNAL(stepChanged(double)), lineStepDoubleSpinBox, SLOT(setValue(double)));

    QHBoxLayout *lineStepLayout = new QHBoxLayout();
    lineStepLayout->addWidget(lineStepLabel);
    lineStepLayout->addWidget(lineStepDoubleSpinBox);


    QVBoxLayout *leftRightLayout = new QVBoxLayout();
    leftRightLayout->addWidget(rightSideRadioButton);
    leftRightLayout->addWidget(leftSideRadioButton);
    leftRightLayout->addLayout(lineStepLayout);
    QGroupBox *tramwaysGroupBox = new QGroupBox();
    tramwaysGroupBox->setLayout(leftRightLayout);


    tramWaysLayout = new QVBoxLayout();
    tramWaysLayout->addLayout(lineTypeLayout);
    tramWaysLayout->addWidget(singleDoubleGroupBox);
    tramWaysLayout->addWidget(tramwaysGroupBox);
    tramWaysLayout->addLayout(buttonsLayout);

    QWidget *widget = new QWidget();
    widget->setLayout(tramWaysLayout);
    layout->addWidget(widget);
}

void StepDialog::setRightSide(bool status)
{
    if (rightSide == status)
        return;
    rightSide = status;
    emit rightSideChanged(status);
}

void StepDialog::setDifferentDirection(bool status)
{
    if (differentDirections == status)
        return;
    differentDirections = status;
    emit differentDirectionsChanged(status);
}

void StepDialog::setBeginSide(bool status)
{
    if (beginSide == status)
        return;
    beginSide = status;
    emit beginSideChanged(status);
}

void StepDialog::setStep(double value)
{
    if (step == value)
        return;
    step = value;
    emit stepChanged(value);
}

void StepDialog::setLineType(int type)
{
    qDebug() << "SetLineType" << type;
    if (lineType == type)
        return;
    switch (type)
    {
    case 6:
        layout->setCurrentIndex(1);
        break;
    case 7:
        layout->setCurrentIndex(2);
        break;
    case 8:
        layout->setCurrentIndex(3);
        break;
    default:
        if (type >=0 && type <6)
            layout->setCurrentIndex(0);
        break;
    }
    lineType = type;
    emit lineTypeChanged(type);
}

void StepDialog::setBeginStep(double step)
{
    if (beginStep == step)
        return;
    beginStep = step;
    emit beginStepChanged(step);
}

void StepDialog::setEndStep(double step)
{
    if (endStep == step)
        return;
    endStep = step;
    emit endStepChanged(step);
}

void StepDialog::changeGroupBox(int)
{

}

void StepDialog::buttonClicked()
{
    if (lineType < 0)
        lineType = 0;
    switch (lineType)
    {
    case 6:
    {
        emit rightSideChanged(rightSide);
        emit stepChanged(step);
        emit lineTypeChanged(lineType);
        emit endStepChanged(endStep);
        emit beginStepChanged(beginStep);
        emit beginRoundingChanged(beginRounding);
        emit endRoundingChanged(endRounding);
        emit splitZoneWidthChanged(splitZoneWidth);
        emit splitZoneTypeChanged(splitZoneType);
        emit splitZoneHeightChanged(splitZoneHeight);
        ////qDebug() << "SplitZone";
    }
        break;
    case 7:
    {
        emit rightSideChanged(rightSide);
        emit beginSideChanged(beginSide);
        emit stepChanged(step);
        emit lineTypeChanged(lineType);
        emit beginStepChanged(beginStep);
        emit endStepChanged(endStep);

    }
        break;
    case 8:
    {
        emit rightSideChanged(rightSide);
        emit stepChanged(step);
        emit lineTypeChanged(lineType);
        emit singleWayChanged(singleWay);
        emit axisStepChanged(axisStep);
        emit endStepChanged(endStep);
        emit beginStepChanged(beginStep);
    }
        break;
    default:
    {
        emit rightSideChanged(rightSide);
        emit stepChanged(step);
        emit lineTypeChanged(lineType);
        emit endStepChanged(endStep);
        emit beginStepChanged(beginStep);
        emit differentDirectionsChanged(differentDirections);
    }
        break;
    }
    this->accept();
    qDebug() << "Accepted";
    setLineType(0);
    setRightSide(true);
    setDifferentDirection(true);
    setBeginSide(true);
    setStep(0.0f);
    setBeginStep(0.0);
    setEndStep(0.0);
    setBeginRoundingStatus(true);
    setEndRoundingStatus(true);
    setSplitZoneWidth(1.0);
    setSingleWay(true);
    setAxisStep(0.0);
    setSplitZoneHeight(0.1);
    setSplitZoneType(0);

}

void StepDialog::setBeginRoundingStatus(bool status)
{
    if (beginRounding == status)
        return;
    beginRounding = status;
    if (beginRounding == false)
    {
        endRounding = true;
        emit endRoundingChanged(endRounding);
    }
    emit beginRoundingChanged(beginRounding);
}

void StepDialog::setEndRoundingStatus(bool status)
{
    if (endRounding == status)
        return;
    endRounding = status;
    if (endRounding == false)
    {
        beginRounding = true;
        emit beginRoundingChanged(beginRounding);
    }
    emit endRoundingChanged(endRounding);
}

void StepDialog::setSplitZoneWidth(double width)
{
    if (splitZoneWidth == width)
        return;
    splitZoneWidth = width;
    emit splitZoneWidthChanged(splitZoneWidth);
}

void StepDialog::setSingleWay(bool status)
{
    if (singleWay == status)
        return;
    singleWay = status;
    emit singleWayChanged(status);
}

void StepDialog::setAxisStep(double step)
{
    if (axisStep == step)
        return;
    axisStep = step;
    emit axisStepChanged(step);
}

void StepDialog::setSplitZoneHeight(double value)
{
    if (splitZoneHeight == value)
        return;
    splitZoneHeight = value;
    emit splitZoneHeightChanged(value);
}

void StepDialog::setSplitZoneType(int type)
{
    if (splitZoneType == type)
        return;
    splitZoneType = type;
    emit splitZoneTypeChanged(type);
    switch (type)
    {
    case 0:
        emit splitZoneHeightEnabledChanged(false);
        break;
    case 1:
        emit splitZoneHeightEnabledChanged(true);
        break;
    case 2:
        emit splitZoneHeightEnabledChanged(true);
        break;
    default:
        emit splitZoneHeightEnabledChanged(false);
        break;
    }
}

