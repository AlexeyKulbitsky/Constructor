#include "stepdialog.h"
#include "roadelement.h"


StepDialog::
StepDialog(QWidget *parent):QDialog(parent)
{   
    QVBoxLayout *mainLayout = new QVBoxLayout();
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

    layout = new QStackedLayout();


    QPushButton *okPushButton = new QPushButton("OK");
    connect(okPushButton, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    QPushButton *cancelPushButton = new QPushButton("Cancel");
    connect(cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QPushButton *deleteButton = new QPushButton("Удалить");
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
    connect(this, SIGNAL(useDeleteButton(bool)), deleteButton, SLOT(setVisible(bool)));
    deleteButton->setVisible(false);
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(okPushButton);
    buttonsLayout->addWidget(cancelPushButton);


    mainLayout->addLayout(lineTypeLayout);
    mainLayout->addLayout(layout);
    mainLayout->addLayout(buttonsLayout);

    setDefaultLayout();
    setSplitZoneLayout();
    setStopLineLayout();
    setTramwaysLayout();
    setLayout(mainLayout);

    usingTarget = Create;
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

void StepDialog::setUsingTarget(StepDialogUsingTarget usingTarget)
{
    this->usingTarget = usingTarget;
    switch(usingTarget)
    {
    case Create:
        emit useDeleteButton(false);
        break;
    case Edit:
        emit useDeleteButton(true);
        break;
    default:
        break;
    }
}

void StepDialog::setLine(LineLinkedToRoad line)
{

    setLineType(line.type);
    setStep(line.step);
    switch(line.type)
    {
    case Line::StopLine:
    {
        setRightSide(line.linkedToRightSide);
        setBeginSide(line.linkedToBeginSide);
        setRightStep(line.rightStep);
        setLeftStep(line.leftStep);
    }
        break;
    case Line::SplitZone:
    {
        setSplitZoneType(line.splitZoneType);
        setRightSide(line.linkedToRightSide);
        setBeginStep(line.beginStep);
        setEndStep(line.endStep);
        setBeginRoundingStatus(line.beginRounding);
        setEndRoundingStatus(line.endRounding);
        setSplitZoneWidth(line.splitZoneWidth);
        setSplitZoneHeight(line.splitZoneHeight);
    }
        break;
    case Line::TramWays:
    {

    }
        break;
    default:
    {
        setRightSide(line.linkedToRightSide);
        setBeginStep(line.beginStep);
        setEndStep(line.endStep);
    }
        break;
    }

    this->line = line;

}

void StepDialog::clear()
{
    setRightSide(true);
    setBeginSide(true);
    setStep(0.0);
    setBeginStep(0.0);
    setEndStep(0.0);
    setBeginRoundingStatus(true);
    setEndRoundingStatus(true);
    setSplitZoneWidth(0.0);
    setSingleWay(true);
    setAxisStep(0.0);
    setSplitZoneHeight(0.0);
    setSplitZoneType(0);
}

void StepDialog::setDefaultLayout()
{   
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
    beginStepDoubleSpinBox->setValue(line.beginStep);

    QHBoxLayout *beginStepLayout = new QHBoxLayout();
    beginStepLayout->addWidget(beginStepLabel);
    beginStepLayout->addWidget(beginStepDoubleSpinBox);

    QDoubleSpinBox *endStepDoubleSpinBox = new QDoubleSpinBox();
    endStepDoubleSpinBox->setMinimum(0.0);
    QLabel *endStepLabel = new QLabel("Отступ с конца");
    connect(endStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setEndStep(double)));
    connect(this, SIGNAL(endStepChanged(double)), endStepDoubleSpinBox, SLOT(setValue(double)));
    endStepDoubleSpinBox->setValue(line.endStep);

    QHBoxLayout *endStepLayout = new QHBoxLayout();
    endStepLayout->addWidget(endStepLabel);
    endStepLayout->addWidget(endStepDoubleSpinBox);

    QLabel *lineStepLabel = new QLabel("Отступ");
    QDoubleSpinBox *lineStepDoubleSpinBox = new QDoubleSpinBox();
    lineStepDoubleSpinBox->setMinimum(0.0f);
    connect(lineStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setStep(double)));
    connect(this, SIGNAL(stepChanged(double)), lineStepDoubleSpinBox, SLOT(setValue(double)));
    lineStepDoubleSpinBox->setValue(line.step);

    QHBoxLayout *lineStepLayout = new QHBoxLayout();
    lineStepLayout->addWidget(lineStepLabel);
    lineStepLayout->addWidget(lineStepDoubleSpinBox);   

    QVBoxLayout *leftRightLayout = new QVBoxLayout();
    leftRightLayout->addWidget(rightSideRadioButton);
    leftRightLayout->addWidget(leftSideRadioButton);
    leftRightLayout->addLayout(lineStepLayout);
    leftRightLayout->addLayout(beginStepLayout);
    leftRightLayout->addLayout(endStepLayout);
    QGroupBox *defaultGroupBox = new QGroupBox();
    defaultGroupBox->setLayout(leftRightLayout);


    defaultLayout = new QVBoxLayout();
    defaultLayout->addWidget(defaultGroupBox);

    QWidget *widget = new QWidget();
    widget->setLayout(defaultLayout);
    layout->addWidget(widget);
}

void StepDialog::setStopLineLayout()
{
    QRadioButton *leftSideRadioButton = new QRadioButton("Отсчет вдоль левой стороны");
    leftSideRadioButton->setToolTip("Если у полотна длины правой и левой сторон отличаются, позволяет выбрать ту сторону,\nвдоль которой будет отсчитываться отступ от края дороги (от начала или конца)");
    QRadioButton *rightSideRadioButton = new QRadioButton("Отсчет вдоль правой стороны");
    rightSideRadioButton->setToolTip("Если у полотна длины правой и левой сторон отличаются, позволяет выбрать ту сторону,\nвдоль которой будет отсчитываться отступ от края дороги (от начала или конца)");
    connect(rightSideRadioButton, SIGNAL(toggled(bool)), this, SLOT(setRightSide(bool)));
    connect(this, SIGNAL(rightSideChanged(bool)), rightSideRadioButton, SLOT(setChecked(bool)));
    rightSideRadioButton->setChecked(true);

    QLabel *lineStepLabel = new QLabel("Отступ");
    QDoubleSpinBox *lineStepDoubleSpinBox = new QDoubleSpinBox();
    lineStepDoubleSpinBox->setMinimum(0.0);
    connect(lineStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setStep(double)));
    connect(this, SIGNAL(stepChanged(double)), lineStepDoubleSpinBox, SLOT(setValue(double)));
    QHBoxLayout *lineStepLayout = new QHBoxLayout();
    lineStepLayout->addWidget(lineStepLabel);
    lineStepLayout->addWidget(lineStepDoubleSpinBox);

    QLabel *rightStepLabel = new QLabel("Отступ от правого края");
    QDoubleSpinBox *rightStepDoubleSpinBox = new QDoubleSpinBox();
    rightStepDoubleSpinBox->setMinimum(0.0);
    connect(rightStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRightStep(double)));
    connect(this, SIGNAL(rightStepChanged(double)), rightStepDoubleSpinBox, SLOT(setValue(double)));
    QHBoxLayout *rightStepLayout = new QHBoxLayout();
    rightStepLayout->addWidget(rightStepLabel);
    rightStepLayout->addWidget(rightStepDoubleSpinBox);

    QLabel *leftStepLabel = new QLabel("Отступ от левого края");
    QDoubleSpinBox *leftStepDoubleSpinBox = new QDoubleSpinBox();
    leftStepDoubleSpinBox->setMinimum(0.0);
    connect(leftStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLeftStep(double)));
    connect(this, SIGNAL(leftStepChanged(double)), leftStepDoubleSpinBox, SLOT(setValue(double)));
    QHBoxLayout *leftStepLayout = new QHBoxLayout();
    leftStepLayout->addWidget(leftStepLabel);
    leftStepLayout->addWidget(leftStepDoubleSpinBox);

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
    stopLayout->addLayout(rightStepLayout);
    stopLayout->addLayout(leftStepLayout);
    stopLayout->addLayout(lineStepLayout);

    QGroupBox *stopLineGroupBox = new QGroupBox();
    stopLineGroupBox->setLayout(stopLayout);


    stopLineLayout = new QVBoxLayout();
    stopLineLayout->addWidget(stopLineGroupBox);

    QWidget *widget = new QWidget();
    widget->setLayout(stopLineLayout);
    layout->addWidget(widget);
}

void StepDialog::setSplitZoneLayout()
{    
    QLabel *splitZoneTypeLabel = new QLabel("Тип разделительной зоны");
    QComboBox *splitZoneTypeComboBox = new QComboBox();
    splitZoneTypeComboBox->addItem("Разметка");
    splitZoneTypeComboBox->addItem("Газон");
    splitZoneTypeComboBox->addItem("Тротуар");
    connect(splitZoneTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSplitZoneType(int)));
    //connect(this, SIGNAL(splitZoneTypeChanged(int)), splitZoneTypeComboBox, SLOT(setCurrentIndex(int)));

    QHBoxLayout *splitZoneTypeLayout = new QHBoxLayout();
    splitZoneTypeLayout->addWidget(splitZoneTypeLabel);
    splitZoneTypeLayout->addWidget(splitZoneTypeComboBox);

    QDoubleSpinBox* splitZoneHeightSpinBox = new QDoubleSpinBox();
    splitZoneHeightSpinBox->setMinimum(0.0);
    splitZoneHeightSpinBox->setValue(line.splitZoneHeight);
    splitZoneHeightSpinBox->setEnabled(false);


    //connect(this, SIGNAL(splitZoneHeightChanged(double)), splitZoneHeightSpinBox, SLOT(setValue(double)));

    connect(splitZoneHeightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSplitZoneHeight(double)));
    connect(this, SIGNAL(splitZoneHeightEnabledChanged(bool)), splitZoneHeightSpinBox, SLOT(setEnabled(bool)));

    QLabel* splitZoneHeightLabel = new QLabel("Высота");

    QHBoxLayout* splitZoneHeightLayout = new QHBoxLayout();
    splitZoneHeightLayout->addWidget(splitZoneHeightLabel);
    splitZoneHeightLayout->addWidget(splitZoneHeightSpinBox);

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
    beginRoundingCheckBox->setChecked(line.beginRounding);
    connect(beginRoundingCheckBox, SIGNAL(toggled(bool)), this, SLOT(setBeginRoundingStatus(bool)));
    connect(this, SIGNAL(beginRoundingChanged(bool)), beginRoundingCheckBox, SLOT(setChecked(bool)));

    QCheckBox *endRoundingCheckBox = new QCheckBox("Закругление вконце");
    endRoundingCheckBox->setChecked(line.endRounding);
    connect(endRoundingCheckBox, SIGNAL(toggled(bool)), this, SLOT(setEndRoundingStatus(bool)));
    connect(this, SIGNAL(endRoundingChanged(bool)), endRoundingCheckBox, SLOT(setChecked(bool)));

    QDoubleSpinBox *splitZoneWidthSpinBox = new QDoubleSpinBox();
    splitZoneWidthSpinBox->setMinimum(0.0);
    splitZoneWidthSpinBox->setValue(line.splitZoneWidth);
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
    leftRightLayout->addLayout(lineStepLayout);
    leftRightLayout->addLayout(beginStepLayout);
    leftRightLayout->addLayout(endStepLayout);
    QGroupBox *defaultGroupBox = new QGroupBox();
    defaultGroupBox->setLayout(leftRightLayout);


    splitZoneLayout = new QVBoxLayout();
    splitZoneLayout->addLayout(splitZoneTypeLayout);
    splitZoneLayout->addLayout(splitZoneHeightLayout);
    splitZoneLayout->addWidget(defaultGroupBox);

    QWidget *widget = new QWidget();
    widget->setLayout(splitZoneLayout);
    layout->addWidget(widget);
}

void StepDialog::setTramwaysLayout()
{
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
    connect(this, SIGNAL(useDeleteButton(bool)), singleDoubleGroupBox, SLOT(setHidden(bool)));

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
    tramWaysLayout->addWidget(singleDoubleGroupBox);
    tramWaysLayout->addWidget(tramwaysGroupBox);

    QWidget *widget = new QWidget();
    widget->setLayout(tramWaysLayout);
    layout->addWidget(widget);
}

void StepDialog::setRightSide(bool status)
{
    if (line.linkedToRightSide == status)
        return;
    line.linkedToRightSide = status;
    emit rightSideChanged(status);
}


void StepDialog::setBeginSide(bool status)
{
    if (line.linkedToBeginSide == status)
        return;
    line.linkedToBeginSide = status;
    emit beginSideChanged(status);
}

void StepDialog::setStep(double value)
{    
    if (line.step == value)
        return;
    line.step = value;
    emit stepChanged(value);
    qDebug() << "Step =" << value;
}

void StepDialog::setLineType(int type)
{
    qDebug() << "SetLineType" << type;
    if (line.type == type)
        return;
    clear();
    switch (type)
    {
    case Line::SplitZone:
        layout->setCurrentIndex(1);
        break;
    case Line::StopLine:
        layout->setCurrentIndex(2);
        break;
    case Line::TramWays:
        layout->setCurrentIndex(3);
        break;
    default:
        if (type >=0 && type <6)
            layout->setCurrentIndex(0);
        break;
    }
    line.type = Line::LineType(type);
    emit lineTypeChanged(type);
}

void StepDialog::setBeginStep(double step)
{
    if (line.beginStep == step)
        return;
    line.beginStep = step;
    emit beginStepChanged(step);
}

void StepDialog::setEndStep(double step)
{
    if (line.endStep == step)
        return;
    line.endStep = step;
    emit endStepChanged(step);
}


void StepDialog::buttonClicked()
{
    if (line.type < 0)
        line.type = Line::SingleSolid;
    this->accept();

    switch (usingTarget)
    {
    case Create:
        emit lineCreated(line);
        break;
    case Edit:
        emit lineEdited(line);
        break;
    default:
        break;
    }



}

void StepDialog::setBeginRoundingStatus(bool status)
{
    if (line.beginRounding == status)
        return;
    line.beginRounding = status;
    if (line.beginRounding == false)
    {
        line.endRounding = true;
        emit endRoundingChanged(line.endRounding);
    }
    emit beginRoundingChanged(line.beginRounding);
}

void StepDialog::setEndRoundingStatus(bool status)
{
    if (line.endRounding == status)
        return;
    line.endRounding = status;
    if (line.endRounding == false)
    {
        line.beginRounding = true;
        emit beginRoundingChanged(line.beginRounding);
    }
    emit endRoundingChanged(line.endRounding);
}

void StepDialog::setSplitZoneWidth(double width)
{
    if (line.splitZoneWidth == width)
        return;
    line.splitZoneWidth = width;
    emit splitZoneWidthChanged(line.splitZoneWidth);
}

void StepDialog::setSingleWay(bool status)
{
    if (line.singleWay == status)
        return;
    line.singleWay = status;
    emit singleWayChanged(status);
}

void StepDialog::setAxisStep(double step)
{
    if (line.axisStep == step)
        return;
    line.axisStep = step;
    emit axisStepChanged(step);
}

void StepDialog::setSplitZoneHeight(double value)
{
    if (line.splitZoneHeight == value)
        return;
    line.splitZoneHeight = value;
    emit splitZoneHeightChanged(value);
}

void StepDialog::setSplitZoneType(int type)
{
    if (line.splitZoneType == type)
        return;
    line.splitZoneType = Line::SplitZoneType(type);
    emit splitZoneTypeChanged(type);
    bool enable = false;
    switch (type)
    {
    case Line::Marking:
    {
        qDebug() << "Marking";
        enable = false;
    }
        break;
    case Line::Grass:
    {
        qDebug() << "Grass";
        enable = true;
    }
        break;
    case Line::Board:
    {
        qDebug() << "Board";
        enable = true;
    }
        break;
    default:
        break;
    }
    emit splitZoneHeightEnabledChanged(enable);
}

void StepDialog::deleteButtonClicked()
{
    int result = QMessageBox::warning(this, tr("Удаление разметки"),
                                 tr("Вы действительно хотите удалить линию разметки?"),
                                 QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes)
    {
        emit lineDeleted(line);
    }
    this->reject();
}

void StepDialog::setRightStep(double step)
{
    if (line.rightStep == step)
        return;
    line.rightStep = step;
    emit rightStepChanged(step);
}

void StepDialog::setLeftStep(double step)
{
    if (line.leftStep == step)
        return;
    line.leftStep = step;
    emit leftStepChanged(step);
}



void LineLinkedToRoad::drawDescription(QGLWidget *render)
{
    if (!isActive)
        return;
    switch (type)
    {
    case Line::SplitZone:
        drawSplitZoneDescription(render);
        break;
    case Line::StopLine:
        drawStopLineDescription(render);
        break;
    case Line::TramWays:
        drawTramWaysDescription(render);
        break;
    default:
        drawLineDescription(render);
        break;
    }
    isActive = false;
}

void LineLinkedToRoad::drawLineDescription(QGLWidget *render)
{
    if (beginStepPoints.size() == 0 && endStepPoints.size() == 0)
    {
        glLineWidth(1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        // Отступ
        glVertex3f(stepPoint_Begin.x(), stepPoint_Begin.y(), stepPoint_Begin.z());
        glVertex3f(stepPoint_End.x(), stepPoint_End.y(), stepPoint_End.z());
        // Отступ от начала
        glVertex3f(beginStepPoint_Begin.x(), beginStepPoint_Begin.y(), beginStepPoint_Begin.z());
        glVertex3f(beginStepPoint_End.x(), beginStepPoint_End.y(), beginStepPoint_End.z());
        // Отступ с конца
        glVertex3f(endStepPoint_Begin.x(), endStepPoint_Begin.y(), endStepPoint_Begin.z());
        glVertex3f(endStepPoint_End.x(), endStepPoint_End.y(), endStepPoint_End.z());
        glEnd();
        if (!render)
        {
            return;
        }
        GLdouble x, y, z;
        GLdouble wx, wy, wz;
        QFont shrift = QFont("Times", 8, QFont::Bold);
        float x1, x2, y1, y2;

        // Отрисовка шага
        x1 = stepPoint_Begin.x();
        y1 = stepPoint_Begin.y();
        x2 = stepPoint_End.x();
        y2 = stepPoint_End.y();
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, QString("%1").arg(step, 0, 'f', 2), shrift);

        // Отрисовка шага от начала
        x1 = beginStepPoint_Begin.x();
        y1 = beginStepPoint_Begin.y();
        x2 = beginStepPoint_End.x();
        y2 = beginStepPoint_End.y();
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, QString("%1").arg(beginStep, 0, 'f', 2), shrift);

        // Отрисовка шага с конца
        x1 = endStepPoint_Begin.x();
        y1 = endStepPoint_Begin.y();
        x2 = endStepPoint_End.x();
        y2 = endStepPoint_End.y();
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, QString("%1").arg(endStep, 0, 'f', 2), shrift);
    }
    else
    {
        glLineWidth(1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        // Отступ
        glVertex3f(stepPoint_Begin.x(), stepPoint_Begin.y(), stepPoint_Begin.z());
        glVertex3f(stepPoint_End.x(), stepPoint_End.y(), stepPoint_End.z());
        glEnd();

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < beginStepPoints.size() / 3; ++i)
        {
            glVertex3f(beginStepPoints[i * 3],
                       beginStepPoints[i * 3 + 1],
                       beginStepPoints[i * 3 + 2]);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < endStepPoints.size() / 3; ++i)
        {
            glVertex3f(endStepPoints[i * 3],
                       endStepPoints[i * 3 + 1],
                       endStepPoints[i * 3 + 2]);
        }
        glEnd();
        if (!render)
        {
            return;
        }
        GLdouble x, y, z;
        GLdouble wx, wy, wz;
        QFont shrift = QFont("Times", 8, QFont::Bold);
        float x1, x2, y1, y2;

        // Отрисовка шага
        x1 = stepPoint_Begin.x();
        y1 = stepPoint_Begin.y();
        x2 = stepPoint_End.x();
        y2 = stepPoint_End.y();
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, QString("%1").arg(step, 0, 'f', 2), shrift);

        int index = beginStepPoints.size() / 6;
        // Отрисовка шага от начала
        x1 = beginStepPoints[index * 3];
        y1 = beginStepPoints[index * 3 + 1];
        x = x1;
        y = y1;
        z = 0.0f;
        RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, QString("%1").arg(beginStep, 0, 'f', 2), shrift);

        index = endStepPoints.size() / 6;
        // Отрисовка шага с конца
        x1 = endStepPoints[index * 3];
        y1 = endStepPoints[index * 3 + 1];
        x = x1;
        y = y1;
        z = 0.0f;
        RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, QString("%1").arg(endStep, 0, 'f', 2), shrift);
    }

}

void LineLinkedToRoad::drawSplitZoneDescription(QGLWidget *render)
{
    drawLineDescription(render);
}

void LineLinkedToRoad::drawTramWaysDescription(QGLWidget *render)
{
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    // Отступ
    glVertex3f(stepPoint_Begin.x(), stepPoint_Begin.y(), stepPoint_Begin.z());
    glVertex3f(stepPoint_End.x(), stepPoint_End.y(), stepPoint_End.z());
    glEnd();
    if (!render)
    {
        return;
    }
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Bold);
    float x1, x2, y1, y2;

    // Отрисовка шага
    x1 = stepPoint_Begin.x();
    y1 = stepPoint_Begin.y();
    x2 = stepPoint_End.x();
    y2 = stepPoint_End.y();
    x = (x1 + x2) / 2.0f;
    y = (y1 + y2) / 2.0f;
    z = 0.0f;
    RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
    render->renderText(wx + 5, wy + 5, QString("%1").arg(step, 0, 'f', 2), shrift);

}

void LineLinkedToRoad::drawStopLineDescription(QGLWidget *render)
{
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    // Отступ
    glVertex3f(stepPoint_Begin.x(), stepPoint_Begin.y(), stepPoint_Begin.z());
    glVertex3f(stepPoint_End.x(), stepPoint_End.y(), stepPoint_End.z());
    // Отступ от начала
    glVertex3f(linkedPoint.x(), linkedPoint.y(), linkedPoint.z());
    glVertex3f(stepPoint_End.x(), stepPoint_End.y(), stepPoint_End.z());
    // Отступ от левой стороны
    glVertex3f(beginStepPoint_Begin.x(), beginStepPoint_Begin.y(), beginStepPoint_Begin.z());
    glVertex3f(beginStepPoint_End.x(), beginStepPoint_End.y(), beginStepPoint_End.z());
    // Отступ от правой стороны
    glVertex3f(endStepPoint_Begin.x(), endStepPoint_Begin.y(), endStepPoint_Begin.z());
    glVertex3f(endStepPoint_End.x(), endStepPoint_End.y(), endStepPoint_End.z());
    glEnd();
    if (!render)
    {
        return;
    }
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Bold);
    float x1, x2, y1, y2;

    // Отрисовка шага
    x1 = stepPoint_Begin.x();
    y1 = stepPoint_Begin.y();
    x2 = stepPoint_End.x();
    y2 = stepPoint_End.y();
    x = (x1 + x2) / 2.0f;
    y = (y1 + y2) / 2.0f;
    z = 0.0f;
    RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
    render->renderText(wx + 5, wy + 5, QString("%1").arg(step, 0, 'f', 2), shrift);

    // Отрисовка шага от левой стороны
    x1 = endStepPoint_Begin.x();
    y1 = endStepPoint_Begin.y();
    x2 = endStepPoint_End.x();
    y2 = endStepPoint_End.y();
    x = (x1 + x2) / 2.0f;
    y = (y1 + y2) / 2.0f;
    z = 0.0f;
    RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
    render->renderText(wx + 5, wy + 5, QString("%1").arg(rightStep, 0, 'f', 2), shrift);

    // Отрисовка шага от правой стороны
    x1 = beginStepPoint_Begin.x();
    y1 = beginStepPoint_Begin.y();
    x2 = beginStepPoint_End.x();
    y2 = beginStepPoint_End.y();
    x = (x1 + x2) / 2.0f;
    y = (y1 + y2) / 2.0f;
    z = 0.0f;
    RoadElement::getWindowCoord(x, y, z, wx, wy, wz);
    render->renderText(wx + 5, wy + 5, QString("%1").arg(leftStep, 0, 'f', 2), shrift);


}
