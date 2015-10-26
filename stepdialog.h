#ifndef STEPDIALOG_H
#define STEPDIALOG_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QtGui>
#include <QGroupBox>
#include <QStackedLayout>
#include <QCheckBox>

class StepDialog: public QDialog
{
    Q_OBJECT
public:
    StepDialog(QWidget *parent = 0);
    ~StepDialog();

private:
    QStackedLayout *layout;
    QVBoxLayout *defaultLayout;
    QVBoxLayout *stopLineLayout;
    QVBoxLayout *splitZoneLayout;
    QVBoxLayout *tramWaysLayout;

    void setDefaultLayout();
    void setStopLineLayout();
    void setSplitZoneLayout();
    void setTramwaysLayout();

public:
    bool rightSide;
    bool beginSide;
    bool beginRounding;
    bool endRounding;
    double splitZoneWidth;
    bool differentDirections;
    double step;
    int lineType;
    double beginStep;
    double endStep;
    bool singleWay;
    double axisStep;
    double splitZoneHeight;
    int splitZoneType;

signals:
    void rightSideChanged(bool status);
    void stepChanged(double value);
    void lineTypeChanged(int type);
    void beginStepChanged(double step);
    void endStepChanged(double step);
    void beginSideChanged(bool status);
    void differentDirectionsChanged(bool status);
    void beginRoundingChanged(bool status);
    void endRoundingChanged(bool status);
    void splitZoneWidthChanged(double value);
    void singleWayChanged(bool status);
    void axisStepChanged(double step);
    void splitZoneHeightChanged(double value);
    void splitZoneTypeChanged(int type);
    void splitZoneHeightEnabledChanged(bool status);

public slots:
    void setRightSide(bool status);
    void setDifferentDirection(bool status);
    void setBeginSide(bool status);
    void setStep(double value);
    void setLineType(int type);
    void setBeginStep(double step);
    void setEndStep(double step);
    void changeGroupBox(int index);
    void buttonClicked();
    void setBeginRoundingStatus(bool status);
    void setEndRoundingStatus(bool status);
    void setSplitZoneWidth(double width);
    void setSingleWay(bool status);
    void setAxisStep(double step);
    void setSplitZoneHeight(double value);
    void setSplitZoneType(int type);
};

#endif // STEPDIALOG_H
