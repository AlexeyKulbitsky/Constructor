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

    layout = new QStackedLayout();

    setDefaultLayout();
    setSplitZoneLayout();
    setStopLineLayout();

    setLayout(layout);

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
    connect(lineTypeComboBox, SIGNAL(activated(int)), this, SLOT(setLineType(int)));

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
    rightSideRadioButton->setChecked(true);

    QDoubleSpinBox *beginStepDoubleSpinBox = new QDoubleSpinBox();
    beginStepDoubleSpinBox->setMinimum(0.0);
    QLabel *beginStepLabel = new QLabel("Отступ от начала");
    connect(beginStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setBeginStep(double)));

    QHBoxLayout *beginStepLayout = new QHBoxLayout();
    beginStepLayout->addWidget(beginStepLabel);
    beginStepLayout->addWidget(beginStepDoubleSpinBox);

    QDoubleSpinBox *endStepDoubleSpinBox = new QDoubleSpinBox();
    endStepDoubleSpinBox->setMinimum(0.0);
    QLabel *endStepLabel = new QLabel("Отступ с конца");
    connect(endStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setEndStep(double)));

    QHBoxLayout *endStepLayout = new QHBoxLayout();
    endStepLayout->addWidget(endStepLabel);
    endStepLayout->addWidget(endStepDoubleSpinBox);

    QLabel *lineStepLabel = new QLabel("Отступ");
    QDoubleSpinBox *lineStepDoubleSpinBox = new QDoubleSpinBox();
    lineStepDoubleSpinBox->setMinimum(0.0f);
    connect(lineStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setStep(double)));

    QHBoxLayout *lineStepLayout = new QHBoxLayout();
    lineStepLayout->addWidget(lineStepLabel);
    lineStepLayout->addWidget(lineStepDoubleSpinBox);

    QRadioButton *oneDirectionRadionButton = new QRadioButton("Разделяет потоки одного направления");
    QRadioButton *differentDirectionsRadioButton = new QRadioButton("Разделяет потоки противоположных направлений");
    connect(differentDirectionsRadioButton, SIGNAL(toggled(bool)), this, SLOT(setDifferentDirection(bool)));
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
    connect(lineTypeComboBox, SIGNAL(activated(int)), this, SLOT(setLineType(int)));

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
    rightSideRadioButton->setChecked(true);

    QLabel *lineStepLabel = new QLabel("Отступ");
    QDoubleSpinBox *lineStepDoubleSpinBox = new QDoubleSpinBox();
    lineStepDoubleSpinBox->setMinimum(0.0f);
    connect(lineStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setStep(double)));
    QHBoxLayout *lineStepLayout = new QHBoxLayout();
    lineStepLayout->addWidget(lineStepLabel);
    lineStepLayout->addWidget(lineStepDoubleSpinBox);

    QVBoxLayout *leftRightLayout = new QVBoxLayout();
    leftRightLayout->addWidget(leftSideRadioButton);
    leftRightLayout->addWidget(rightSideRadioButton);
    leftRightLayout->addLayout(lineStepLayout);

    QGroupBox *leftRightGroupBox = new QGroupBox();
    leftRightGroupBox->setLayout(leftRightLayout);

    ///////////////////////////

    QRadioButton *beginRadioButton = new QRadioButton("Привязать к началу");
    QRadioButton *endRadioButton = new QRadioButton("Привязать к концу");
    connect(beginRadioButton, SIGNAL(toggled(bool)), this, SLOT(setBeginSide(bool)));
    beginRadioButton->setChecked(true);

    QVBoxLayout *beginEndLayout = new QVBoxLayout();
    beginEndLayout->addWidget(beginRadioButton);
    beginEndLayout->addWidget(endRadioButton);

    QGroupBox *beginEndGroupBox = new QGroupBox();
    beginEndGroupBox->setLayout(beginEndLayout);

    QVBoxLayout *stopLayout = new QVBoxLayout();
    stopLayout->addWidget(beginEndGroupBox);
    stopLayout->addWidget(leftRightGroupBox);

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
    connect(lineTypeComboBox, SIGNAL(activated(int)), this, SLOT(setLineType(int)));

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
    rightSideRadioButton->setChecked(true);

    QDoubleSpinBox *beginStepDoubleSpinBox = new QDoubleSpinBox();
    beginStepDoubleSpinBox->setMinimum(0.0);
    QLabel *beginStepLabel = new QLabel("Отступ от начала");
    connect(beginStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setBeginStep(double)));

    QHBoxLayout *beginStepLayout = new QHBoxLayout();
    beginStepLayout->addWidget(beginStepLabel);
    beginStepLayout->addWidget(beginStepDoubleSpinBox);

    QDoubleSpinBox *endStepDoubleSpinBox = new QDoubleSpinBox();
    endStepDoubleSpinBox->setMinimum(0.0);
    QLabel *endStepLabel = new QLabel("Отступ с конца");
    connect(endStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setEndStep(double)));

    QHBoxLayout *endStepLayout = new QHBoxLayout();
    endStepLayout->addWidget(endStepLabel);
    endStepLayout->addWidget(endStepDoubleSpinBox);

    QLabel *lineStepLabel = new QLabel("Отступ");
    QDoubleSpinBox *lineStepDoubleSpinBox = new QDoubleSpinBox();
    lineStepDoubleSpinBox->setMinimum(0.0f);
    connect(lineStepDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setStep(double)));

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
    splitZoneLayout->addWidget(defaultGroupBox);
    splitZoneLayout->addLayout(buttonsLayout);

    QWidget *widget = new QWidget();
    widget->setLayout(splitZoneLayout);
    layout->addWidget(widget);
}

void StepDialog::setRightSide(bool status)
{
    if (rightSide == status)
        return;
    rightSide = status;
    //emit rightSideChanged(status);
}

void StepDialog::setDifferentDirection(bool status)
{
    if (differentDirections == status)
        return;
    differentDirections = status;
}

void StepDialog::setBeginSide(bool status)
{
    if (beginSide == status)
        return;
    beginSide = status;
}

void StepDialog::setStep(double value)
{
    if (step == value)
        return;
    step = value;
    //emit stepChanged(value);
}

void StepDialog::setLineType(int type)
{
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
    default:
        if (type >=0 && type <6)
            layout->setCurrentIndex(0);
        break;
    }
    lineType = type;
    //emit lineTypeChanged(type);
}

void StepDialog::setBeginStep(double step)
{
    if (beginStep == step)
        return;
    beginStep = step;
}

void StepDialog::setEndStep(double step)
{
    if (endStep == step)
        return;
    endStep = step;
}

void StepDialog::changeGroupBox(int index)
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
        //qDebug() << "SplitZone";
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
    qDebug() << "StepDialog::splitZoneWidth" << splitZoneWidth;
}

