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
#include <QVector3D>
#include <QGLWidget>

class RoadElement;
namespace Line
{
enum LineType
{
    SingleSolid = 0,
    SingleIntermittent,
    DoubleSolid,
    DoubleIntermittentLeft,
    DoubleIntermittentRight,
    DoubleIntermittent,
    SplitZone,
    StopLine,
    TramWays
};

enum SplitZoneType
{
    Marking = 0,
    Board,
    Grass
};

}




struct LineLinkedToRoad
{
    RoadElement *line = NULL;
    Line::LineType type = Line::SingleSolid;
    float step = 0.0f;
    float beginStep = 0.0f;
    float endStep = 0.0f;
    bool linkedToRightSide = true; // Привязка к правой стороне по ходу движения (иначе к левой)
    bool linkedToBeginSide = true; // Привязка к началу (иначе к концу)

    Line::SplitZoneType splitZoneType = Line::Marking;
    bool beginRounding = true;
    bool endRounding = true;
    float splitZoneHeight = 0.0f;
    float splitZoneWidth = 0.0f;

    float lineWidth = 0.0f;

    bool singleWay = true;
    float axisStep = 0.0f;

    bool isActive = false;
    void drawDescription(QGLWidget*);

    QVector3D stepPoint_Begin;
    QVector3D stepPoint_End;
    QVector3D endStepPoint_Begin;
    QVector3D endStepPoint_End;
    QVector3D beginStepPoint_Begin;
    QVector3D beginStepPoint_End;
    void drawLineDescription(QGLWidget*);

    void drawSplitZoneDescription(QGLWidget*);
    void drawStopLineDescription(QGLWidget*);
};

enum StepDialogUsingTarget
{
    Create = 0,
    Edit
};

class StepDialog: public QDialog
{
    Q_OBJECT
public:
    StepDialog(QWidget *parent = 0);
    ~StepDialog();

    void setUsingTarget(StepDialogUsingTarget usingTarget);
    void clear();

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
    LineLinkedToRoad line;
    StepDialogUsingTarget usingTarget;

signals:
    void rightSideChanged(bool status);
    void stepChanged(double value);
    void lineTypeChanged(int type);
    void beginStepChanged(double step);
    void endStepChanged(double step);
    void beginSideChanged(bool status);
    void beginRoundingChanged(bool status);
    void endRoundingChanged(bool status);
    void splitZoneWidthChanged(double value);
    void singleWayChanged(bool status);
    void axisStepChanged(double step);
    void splitZoneHeightChanged(double value);
    void splitZoneTypeChanged(int type);
    void splitZoneHeightEnabledChanged(bool status);

    void lineCreated(LineLinkedToRoad value);

public slots:
    void setRightSide(bool status);
    void setBeginSide(bool status);
    void setStep(double value);
    void setLineType(int type);
    void setBeginStep(double step);
    void setEndStep(double step);
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
