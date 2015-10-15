#include "intersection.h"
#include "assert.h"
#include <QApplication>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include "mainwindow.h"
Intersection::Intersection()
{
    selected = false;
    fixed = false;
}

Intersection::Intersection(float x, float y)
{

    layer = 0;
    float r = 10.0f;
    for (int i = 0; i < 4; ++i)
    {
        float angle = 90.0f * float(i) * 3.1415926 / 180.0f;
        float dx = r * cosf(angle);
        float dy = r * sinf(angle);
        RoadSimple *road = new RoadSimple(x, y, x + dx, y + dy, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                          "RoadSimple", 0,
                                          QString("Рукав №") + QString::number(i + 1));
        road->setLeftBoardShowStatus(true);
        road->setRightBoardShowStatus(true);
        //QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(calculateRoadIntersections()));
        QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(recalculateRoads()));
        roads.push_back(road);

    }
    calculateRoadIntersections();


    for (int i = 0; i < roads.size(); ++i)
    {
        vec3 p0 = roads[i]->getCoordOfPoint(1);
        vec3 p1 = roads[i]->getCoordOfPoint(2);
        vec3 p2(0.0f, 0.0f, 0.0f);
        if (i == roads.size() - 1)
            p2 = roads[0]->getCoordOfPoint(3);
        else
            p2 = roads[i + 1]->getCoordOfPoint(3);
        float leftLength = 5.0f;
        float rightLength = 5.0f;
        float leftR = sqrt((p2.x - p0.x)*(p2.x - p0.x) + (p2.y - p0.y)*(p2.y - p0.y));
        float rightR = sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y));
        float dx1 = leftLength * (p2.x - p0.x) / leftR;
        float dy1 = leftLength * (p2.y - p0.y) / leftR;
        float dx2 = rightLength * (p1.x - p0.x) / rightR;
        float dy2 = rightLength * (p1.y - p0.y) / rightR;
        Curve *curve = new Curve(p0.x, p0.y, p0.z,
                                 p0.x + dx1, p0.y + dy1, p0.z,
                                 p0.x + dx2, p0.y + dy2, p0.z,
                                 QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                 QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                 10);
        //QObject::connect(curve, SIGNAL(angleChanged(double)), this, SLOT(setAngle(double)));
        curve->setBoardShowStatus(false);
        showBoardStatus.push_back(false);
        curve->setSelectedStatus(false);
        curves.push_back(curve);
    }

    texture1USize = 6.0f;
    texture1VSize = 6.0f;
    texture2USize = 2.75f;
    texture2VSize = 6.0f;
    textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg");
    textureID[1] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg");

    setRoadsTextures();

    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
    selected = fixed = false;
    QObject::connect(this, SIGNAL(intersectionsChanged()),this, SLOT(calculateRoundings()));
    QObject::connect(this, SIGNAL(intersectionsChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}



bool Intersection::isSelected()
{
    return selected;
}

void Intersection::setSelectedStatus(bool status)
{
    selected = status;
    for (int i = 0; i < roads.size(); ++i)
    {
        roads[i]->setSelectedStatus(status);
        curves[i]->setSelectedStatus(status);
    }

}

void Intersection::drawFigure(QGLWidget *render)
{


    glDisableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glTexCoordPointer(2, GL_FLOAT, 0, textureArray.begin());
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_BYTE, indexArray.begin());

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);

    for (int i = 0; i < curves.size(); ++i)
    {
        roads[i]->setLeftBoardShowStatus(curves[i]->getBoardShowStatus());
        if (i == curves.size() - 1)
            roads[0]->setRightBoardShowStatus(curves[i]->getBoardShowStatus());
        else
            roads[i + 1]->setRightBoardShowStatus(curves[i]->getBoardShowStatus());

        if (curves[i]->isSelected())
        {
            glDisable(GL_DEPTH_TEST);
            curves[i]->drawSelectionFrame();
            glEnable(GL_DEPTH_TEST);
        }
        curves[i]->drawFigure(render);
    }

    for (int i = 0; i < roads.size(); ++i)
    {
        if (roads[i]->isSelected())
        {
            glDisable(GL_DEPTH_TEST);
            roads[i]->drawSelectionFrame();
            glEnable(GL_DEPTH_TEST);
        }
        roads[i]->drawFigure();
    }
    for (int i = 0; i < roads.size(); ++i)
    {
        if (roads[i]->isSelected())
        {
            roads[i]->drawDescription(render, 1.0f, 0.0f, 0.0f);
        }
    }

}

void Intersection::drawSelectionFrame()
{
}

void Intersection::drawMeasurements(QGLWidget *render)
{
    for (int i = 0; i < curves.size(); ++i)
        curves[i]->drawMeasurements(render);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0FFF);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    for (int i = 0; i < roads.size(); ++i)
    {
        vec2 p1 = roads[i]->getAxisPoint_1();
        vec2 p2 = roads[i]->getAxisPoint_2();
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p1.x, p1.y, 0.03f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p2.x, p2.y, 0.03f);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void Intersection::move(float dx, float dy, float dz)
{
    if (fixed)
        return;

    for (int i = 0; i < roads.size(); ++i)
        roads[i]->move(dx, dy, dz);
    for (int i = 0; i < curves.size(); ++i)
        curves[i]->move(dx, dy, dz);
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        vertexArray[i * 3] += dx;
        vertexArray[i * 3 + 1] += dy;
    }
}

void Intersection::drawControlElement(int index, float lineWidth, float pointSize)
{
    int i;
    for (i = 0; i < roads.size(); ++i)
    {
        if (index > (roads[i]->getNumberOfControls() + 1))
            index -= (roads[i]->getNumberOfControls() + 1);
        else
            break;
    }
    if (i == roads.size())
    {
        for (i = 0; i < curves.size(); ++i)
        {
            if (index > (curves[i]->getNumberOfControls()))
                index -= (curves[i]->getNumberOfControls());
            else
                break;
        }
        if (index != 0)
        {
            curves[i]->drawControlElement(index, lineWidth, pointSize + 10.0f);
        }
    }
    else
        if (index == roads[i]->getNumberOfControls())
            roads[i]->drawFigure();
        else
        {
            if (index != 0)
            {
                roads[i]->drawControlElement(index, lineWidth, pointSize);
            }
        }

}

QCursor Intersection::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void Intersection::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
        return;

    int i;
    for (i = 0; i < roads.size(); ++i)
    {
        if (index > (roads[i]->getNumberOfControls() + 1))
            index -= (roads[i]->getNumberOfControls() + 1);
        else
            break;
    }
    if (i == roads.size())
    {
        for (i = 0; i < curves.size(); ++i)
        {
            if (index > (curves[i]->getNumberOfControls()))
                index -= (curves[i]->getNumberOfControls());
            else
                break;
        }
        if (index != 0)
        {
            curves[i]->resizeByControl(index, dx, dy, x, y);
            if (index != curves[i]->getNumberOfControls() - 1)
            {
                vec3 p1 = curves[i]->getCoordOfPoint(0);
                vec3 p2 = curves[i]->getCoordOfPoint(index);
                vec3 p3(0.0f, 0.0f, 0.0f);
                if (index == 1)
                {
                    if (i == curves.size() - 1)
                    {
                        p3 = roads[0]->getCoordOfPoint(3);
                    }
                    else
                    {
                        p3 = roads[i + 1]->getCoordOfPoint(3);
                    }
                }
                else
                {
                    if (index == 2)
                        p3 = roads[i]->getCoordOfPoint(2);
                }
                float r1 = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
                float r2 = sqrt((p3.x - p1.x)*(p3.x - p1.x) + (p3.y - p1.y)*(p3.y - p1.y));
                float dx = (p3.x - p1.x)/r2 * r1;
                float dy = (p3.y - p1.y)/r2 * r1;
                curves[i]->setCoordForPoint(index, p1.x + dx, p1.y + dy, p1.z);
            }
            else
            {
                // Если потянули за дугу тротуара
                float width = curves[i]->getBoardWidth();
                roads[i]->setLeftBoardWidth(width);
                if (i == roads.size() - 1)
                    roads[0]->setRightBoardWidth(width);
                else
                    roads[i + 1]->setRightBoardWidth(width);
            }
        }
    }
    else
        if (index != roads[i]->getNumberOfControls())
        {
            roads[i]->resizeByControl(index, dx, dy, x, y);
            calculateRoadIntersections();
            // Если дорогу повернули
            calculateRoadForRounding(i, index);
            calculateRoadForAngle(i);


            switch (index)
            {
                // Поворот
            case 2:
                break;
                // Поворот
            case 3:
                break;
                // Поворот
            case 5:
                break;
                // Удлинение
            case 7:
                break;
                // Уширение
            case 8:
                break;
                // Уширение
            case 9:
                break;
            default:
                break;
            }

            if (index == 10)
            {
                float width = roads[i]->getRightBoardWidth();
                if (i == 0)
                {
                    curves[curves.size() - 1]->setBoardWidth(width);
                    roads[roads.size() - 1]->setLeftBoardWidth(width);
                }
                else
                {
                    curves[i - 1]->setBoardWidth(width);
                    roads[i - 1]->setLeftBoardWidth(width);
                }

            }
            else
                if (index == 11)
                {
                    float width = roads[i]->getLeftBoardWidth();
                    if (i == roads.size() - 1)
                        roads[0]->setRightBoardWidth(width);
                    else
                        roads[i + 1]->setRightBoardWidth(width);
                    curves[i]->setBoardWidth(width);
                }
        }
        else
        {
            roads[i]->move(dx, dy);
            vec2 p1 = roads[i]->getAxisPoint_1();
            vec2 p2 = roads[i]->getAxisPoint_2();
            float width = roads[i]->getWidth();
            /*
            float k1;

            if ((p1.x - p2.x) == 0)
                k1 = 0.0f;
            else
                k1 = (p1.y - p2.y)/(p1.x - p2.x);
            float b1 = p2.y - p2.x * k1;
            */
            float a1, a2, b1, b2, c1, c2;

            a1 = p2.y - p1.y;
            b1 = p1.x - p2.x;
            c1 = p1.y * (p2.x - p1.x) - p1.x * (p2.y - p1.y);



            float x1Res, y1Res, x2Res, y2Res, rRes = 1000000.0f;
            for (int j = 0; j < roads.size(); ++j)
            {
                if (j == i)
                {
                    continue;
                }
                else
                {
                    vec2 t1 = roads[j]->getAxisPoint_1();
                    vec2 t2 = roads[j]->getAxisPoint_2();

                    a2 = t2.y - t1.y;
                    b2 = t1.x - t2.x;
                    c2 = t1.y * (t2.x - t1.x) - t1.x * (t2.y - t1.y);
                    float x1Temp;
                    float y1Temp;

                    if (abs(a1) < 0.0001f)
                    {
                        y1Temp = p1.y;
                        if (abs(b2)  < 0.0001f)
                            x1Temp = t1.x;
                        else
                            x1Temp = (-1.0f) * (b2 * y1Temp + c2) / a2;
                    }
                    else if (abs(b1)  < 0.0001f)
                    {
                        x1Temp = p1.x;
                        if (abs(a2)  < 0.0001f)
                            y1Temp = t1.y;
                        else
                            y1Temp = (-1.0f) * (a2 * x1Temp + c2) / b2;
                    }
                    else if (abs(a2)  < 0.0001f)
                    {
                        y1Temp = t1.y;
                        x1Temp = (-1.0f) * (b1 * y1Temp + c1) / a1;
                    }
                    else if (abs(b2)  < 0.0001f)
                    {
                        x1Temp = t1.x;
                        y1Temp = (-1.0f) * (a1 * x1Temp + c1) / b1;
                    }
                    else
                    {
                        x1Temp = (c1 * b2 - c2 * b1) / (a2 * b1 - a1 * b2);
                        y1Temp = (-1.0f) * (a1 * x1Temp + c1) / b1;
                    }




                    /*
                    float k2;

                    if ((t1.x - t2.x) == 0)
                        k2 = 0.0f;
                    else
                        k2 = (t1.y - t2.y)/(t1.x - t2.x);
                    float b2 = t2.y - t2.x * k2;

                    float x1Temp = (b2 - b1) / (k1 - k2);
                    float y1Temp = k1 * x1Temp + b1;

                    */
                    // Если отрезки пересекаются
                    if (((p1.x >= x1Temp && p2.x <= x1Temp) || (p1.x <= x1Temp && p2.x >= x1Temp)) &&
                            ((t1.x >= x1Temp && t2.x <= x1Temp) || (t1.x <= x1Temp && t2.x >= x1Temp)) &&
                            ((p1.y >= y1Temp && p2.y <= y1Temp) || (p1.y <= y1Temp && p2.y >= y1Temp)) &&
                            ((t1.y >= y1Temp && t2.y <= y1Temp) || (t1.y <= y1Temp && t2.y >= y1Temp)))
                    {
                        x1Res = x1Temp;
                        y1Res = y1Temp;
                        x2Res = p2.x;
                        y2Res = p2.y;
                        break;
                    }
                    else
                    {
                        float dr = sqrt((p2.x - x1Temp)*(p2.x - x1Temp) + (p2.y - y1Temp)*(p2.y - y1Temp)) -
                                sqrt((p1.x - x1Temp)*(p1.x - x1Temp) + (p1.y - y1Temp)*(p1.y - y1Temp));
                        // Если отрезки не пересекаются, находим ближайший
                        if ((((t1.x <= x1Temp) && (t2.x >= x1Temp))||((t1.x >= x1Temp) && (t2.x <= x1Temp)) ||
                             (((t1.y <= y1Temp) && (t2.y >= y1Temp))||((t1.y >= y1Temp) && (t2.y <= y1Temp)))) &&
                                dr < rRes)
                        {
                            rRes = dr;
                            x1Res = x1Temp;
                            y1Res = y1Temp;
                            x2Res = p2.x;
                            y2Res = p2.y;
                        }
                        else
                        {
                            // x1Res = p1.x - dx;
                            // y1Res = p1.y - dy;
                            // x2Res = p2.x - dx;
                            // y2Res = p2.y - dy;
                        }

                    }


                }
            }

            roads[i]->setVertexArray(x1Res, y1Res, x2Res, y2Res, width);
            roads[i]->setTextureArray();

            //calculateRoadForRounding(i, index);
            //calculateRoadForAngle(i);
        }
    calculateRoadIntersections();
    calculateRoundings();
    setRoadsTextures();
    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
}

int Intersection::getNumberOfControls()
{
    int count = 0;
    for (int i = 0; i < roads.size(); ++i)
    {
        count += roads[i]->getNumberOfControls() + 1;
    }
    for (int i = 0; i < curves.size(); ++i)
        count += curves[i]->getNumberOfControls();
    return count;
}

int Intersection::controlsForPoint()
{
    return 1;
}

void Intersection::changeColorOfSelectedControl(int index)
{
}

void Intersection::getProperties(QFormLayout *layout, QGLWidget *render)
{
    /*
    this->layout = layout;
    this->render = render;
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
    */
    //clearProperties(layout);
    //QScrollArea *scrollArea = new QScrollArea();
    //scrollArea->setWidget(layout->widget());

    QCheckBox *fixedCheckBox = new QCheckBox();
    fixedCheckBox->setChecked(fixed);
    QObject::connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));
    layout->addRow("Зафиксировать", fixedCheckBox);



    for (int i = 0; i < roads.size(); ++i)
    {
        QPushButton *deletePushButton = new QPushButton("Удалить");
        if (roads.size() > 3)
            connect(deletePushButton, SIGNAL(clicked(bool)), this, SLOT(deleteRoad()));
        layout->addRow("Рукав " + QString::number(i + 1),deletePushButton);
        QDoubleSpinBox *widthDoubleSpinBox = new QDoubleSpinBox();
        widthDoubleSpinBox->setValue(roads[i]->getWidth());
        widthDoubleSpinBox->setMinimum(0.1);
        QObject::connect(roads[i], SIGNAL(widthChanged(double)), widthDoubleSpinBox, SLOT(setValue(double)));
        QObject::connect(widthDoubleSpinBox, SIGNAL(valueChanged(double)), roads[i], SLOT(setWidth(double)));
        if (render)
        {
            QObject::connect(widthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        }
        layout->addRow("Ширина", widthDoubleSpinBox);
        QPushButton *addLinePushButton = new QPushButton("+");

        //StepDialog *stepDialog = new StepDialog();
        connect(stepDialog, SIGNAL(lineTypeChanged(int)), roads[i], SLOT(setLineType(int)));
        connect(stepDialog, SIGNAL(rightSideChanged(bool)), roads[i], SLOT(setRightSide(bool)));
        connect(stepDialog, SIGNAL(stepChanged(double)), roads[i], SLOT(setStep(double)));
        connect(stepDialog, SIGNAL(beginStepChanged(double)), roads[i], SLOT(setBeginStep(double)));
        connect(stepDialog, SIGNAL(endStepChanged(double)), roads[i], SLOT(setEndStep(double)));
        connect(stepDialog, SIGNAL(beginSideChanged(bool)), roads[i], SLOT(setBeginSide(bool)));
        connect(stepDialog, SIGNAL(beginRoundingChanged(bool)), roads[i], SLOT(setBeginRounding(bool)));
        connect(stepDialog, SIGNAL(endRoundingChanged(bool)), roads[i], SLOT(setEndRounding(bool)));
        connect(stepDialog, SIGNAL(splitZoneWidthChanged(double)), roads[i], SLOT(setSplitZoneWidth(double)));
        connect(stepDialog, SIGNAL(differentDirectionsChanged(bool)), roads[i], SLOT(setDifferentDirections(bool)));
        connect(addLinePushButton, SIGNAL(clicked(bool)), stepDialog, SLOT(exec()));
        connect(stepDialog, SIGNAL(accepted()), roads[i], SLOT(addLine()));
        connect(stepDialog, SIGNAL(accepted()), this, SLOT(addLine()));
        for (int j = 0; j < roads[i]->lines.size(); ++j)
        {
            QPushButton* b = new QPushButton(QString::number(j + 1));
            connect(b, SIGNAL(clicked(bool)), roads[i + 1], SLOT(deleteLine()));
            connect(b, SIGNAL(clicked(bool)), this, SLOT(deleteLine()));
            layout->addRow("Удалить линию ",b);
        }

        layout->addRow("Добавить разметку", addLinePushButton);
    }

    QPushButton *addRoadPushButton = new QPushButton("+");
    QObject::connect(addRoadPushButton, SIGNAL(clicked(bool)), this, SLOT(addRoad()));
    QObject::connect(this, SIGNAL(roadAdded()), render, SLOT(updateGL()));
    layout->addRow("Добавить рукав", addRoadPushButton);

    for (int i = 0; i < curves.size(); ++i)
    {
        //QPushButton *deletePushButton = new QPushButton("Удалить");
        //layout->addRow("Закругление " + QString::number(i),deletePushButton);
        QDoubleSpinBox *leftLengthDoubleSpinBox = new QDoubleSpinBox();
        leftLengthDoubleSpinBox->setValue(curves[i]->getLeftLength());
        leftLengthDoubleSpinBox->setMinimum(0.0);
        QObject::connect(curves[i], SIGNAL(leftLengthChanged(double)), leftLengthDoubleSpinBox, SLOT(setValue(double)));
        QObject::connect(leftLengthDoubleSpinBox, SIGNAL(valueChanged(double)), curves[i], SLOT(setLeftLength(double)));

        QDoubleSpinBox *rightLengthDoubleSpinBox = new QDoubleSpinBox();
        rightLengthDoubleSpinBox->setValue(curves[i]->getRightLength());
        rightLengthDoubleSpinBox->setMinimum(0.0);
        QObject::connect(curves[i], SIGNAL(rightLengthChanged(double)), rightLengthDoubleSpinBox, SLOT(setValue(double)));
        QObject::connect(rightLengthDoubleSpinBox, SIGNAL(valueChanged(double)), curves[i], SLOT(setRightLength(double)));

        QCheckBox *showBoardCheckBox = new QCheckBox();
        showBoardCheckBox->setChecked(curves[i]->getBoardShowStatus());
        QObject::connect(showBoardCheckBox, SIGNAL(toggled(bool)), curves[i], SLOT(setBoardShowStatus(bool)));
        QObject::connect(showBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));

        QDoubleSpinBox *angleDoubleSpinBox = new QDoubleSpinBox();
        angleDoubleSpinBox->setObjectName(QString::number(i));
        angleDoubleSpinBox->setMinimum(0.0);
        angleDoubleSpinBox->setMaximum(720.0);
        angleDoubleSpinBox->setValue(curves[i]->angleRounding);
        connect(curves[i], SIGNAL(angleChanged(double)), angleDoubleSpinBox, SLOT(setValue(double)));
        connect(angleDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngle(double)));
        //connect(curves[i], SIGNAL(angleChanged(double)), this, SLOT(setAngle(double)));
        if (render)
        {
            connect(rightLengthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
            connect(leftLengthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
            connect(angleDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        }
        layout->addRow("Закругление " + QString::number(i + 1), showBoardCheckBox);
        //layout->addRow("Отобразить", showBoardCheckBox);
        layout->addRow("Левая сторона", leftLengthDoubleSpinBox);
        layout->addRow("Правая сторона", rightLengthDoubleSpinBox);
        layout->addRow("Угол", angleDoubleSpinBox);
    }

}

bool Intersection::isFixed()
{
    return fixed;
}

int Intersection::getLayer()
{
    return layer;
}

void Intersection::setRoadsTextures()
{
    for (int i = 0; i < roads.size(); ++i)
    {
        int index1 = i == 0 ? roads.size() - 1 : i - 1;
        int index2 = curves[i]->vertexArrayBoard.size() - 15;
        for (int j = 0; j < 15; ++j)
        {
            roads[i]->vertexArrayRight[j] = curves[index1]->vertexArrayBoard[j];
            roads[i]->vertexArrayLeft[j] = curves[i]->vertexArrayBoard[index2 + j];
            roads[i]->setTextureArray();
        }

    }
}

float Intersection::calculateAngle(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
        /*
        vec2 p1 = roads[i]->getAxisPoint_1();
        vec2 p2 = roads[i]->getAxisPoint_2();
        int j = i == (roads.size() - 1) ? 0 : i + 1;
        vec2 p3 = roads[j]->getAxisPoint_1();
        vec2 p4 = roads[j]->getAxisPoint_2();
        */
        float dx1 = p2.x - p1.x;
        float dy1 = p2.y - p1.y;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = p4.x - p3.x;
        float dy2 = p4.y - p3.y;
        float r2 = sqrt(dx2*dx2 + dy2*dy2);
        float angle = acos((dx1*dx2 + dy1*dy2) / (r1 * r2));
        float res = dx1*dy2 - dx2*dy1;
        float pi = 3.14159265f;
        if (res < 0)
            angle = 2.0f * pi - angle;
        return angle;

}

void Intersection::calculateRoadForAngle(int i)
{
    vec2 p1 = roads[i]->getAxisPoint_1();
    vec2 p2 = roads[i]->getAxisPoint_2();
    int j = i == (roads.size() - 1) ? 0 : i + 1;
    vec2 p3 = roads[j]->getAxisPoint_1();
    vec2 p4 = roads[j]->getAxisPoint_2();
    float angle = calculateAngle(p1, p2, p3, p4);
    float pi = 3.14159265f;
    if (angle > pi)
    {
        float dx1 = p2.x - p1.x;
        float dy1 = p2.y - p1.y;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = p4.x - p3.x;
        float dy2 = p4.y - p3.y;
        float r2 = sqrt(dx2*dx2 + dy2*dy2);

        float dx = dx2 / r2 * r1;
        float dy = dy2 / r2 * r1;
        roads[i]->setCoordForAxisPoint(1, p1.x - dx, p1.y - dy);
        roads[i]->setVertexArray();
        roads[i]->setTextureArray();
    }


    j = i == 0 ? roads.size() - 1 : i - 1;
    p3 = roads[j]->getAxisPoint_1();
    p4 = roads[j]->getAxisPoint_2();
    angle = calculateAngle(p3, p4, p1, p2);
    if (angle > pi)
    {
        float dx1 = p2.x - p1.x;
        float dy1 = p2.y - p1.y;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = p4.x - p3.x;
        float dy2 = p4.y - p3.y;
        float r2 = sqrt(dx2*dx2 + dy2*dy2);

        float dx = dx2 / r2 * r1;
        float dy = dy2 / r2 * r1;
        roads[i]->setCoordForAxisPoint(1, p1.x - dx, p1.y - dy);
        roads[i]->setVertexArray();
        roads[i]->setTextureArray();
    }
}

void Intersection::calculateRoadForRounding(int i, int index)
{
    // Левая сторона рукава
    vec3 p1 = roads[i]->getCoordOfPoint(1);
    vec3 p2 = roads[i]->getCoordOfPoint(2);
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float r1 = sqrt(dx1*dx1 + dy1*dy1);
    int j = i == (roads.size() - 1) ? 0 : i + 1;
    vec3 p3 = roads[j]->getCoordOfPoint(0);
    vec3 p4 = roads[j]->getCoordOfPoint(3);
    float dx2 = p4.x - p3.x;
    float dy2 = p4.y - p3.y;
    float r2 = sqrt(dx2*dx2 + dy2*dy2);
    float leftLength = curves[i]->getLeftLength();
    float rightLength = curves[i]->getRightLength();
    float dx, dy;
    if (r1 < rightLength)
    {
        dx = dx1 / r1 * (rightLength - r1);
        dy = dy1 / r1 * (rightLength - r1);
        roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
    }
    if (r2 < leftLength)
    {
        dx = dx2 / r2 * (r2 - leftLength);
        dy = dy2 / r2 * (r2 - leftLength);
        roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
    }
    p1 = roads[i]->getCoordOfPoint(0);
    p2 = roads[i]->getCoordOfPoint(3);
    dx1 = p2.x - p1.x;
    dy1 = p2.y - p1.y;
    r1 = sqrt(dx1*dx1 + dy1*dy1);
    j = i == 0 ? roads.size() - 1 : i - 1;
    p3 = roads[j]->getCoordOfPoint(1);
    p4 = roads[j]->getCoordOfPoint(2);
    dx2 = p4.x - p3.x;
    dy2 = p4.y - p3.y;
    r2 = sqrt(dx2*dx2 + dy2*dy2);
    leftLength = curves[i]->getLeftLength();
    rightLength = curves[i]->getRightLength();
    if (r1 < leftLength)
    {
        dx = dx1 / r1 * (leftLength - r1);
        dy = dy1 / r1 * (leftLength - r1);
        roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
    }
    if (r2 < rightLength)
    {
        dx = dx2 / r2 * (r2 - rightLength);
        dy = dy2 / r2 * (r2 - rightLength);
        roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
    }
}

void Intersection::calculateRoadIntersections()
{    
    for (int i = 0; i < roads.size(); ++i)
        roads[i]->setVertexArray();
    for (int i = 1; i < roads.size() * 2; i += 2)
    {

        vec3 p1 = roads[i / 2]->getCoordOfPoint(1);
        vec3 p2 = roads[i / 2]->getCoordOfPoint(2);

        vec3 t1(0.0f, 0.0f, 0.0f);
        vec3 t2(0.0f, 0.0f, 0.0f);

        if (i == roads.size() * 2 - 1)
        {
            t1 = roads[0]->getCoordOfPoint(0);
            t2 = roads[0]->getCoordOfPoint(3);
        }
        else
        {
            t1 = roads[i / 2 + 1]->getCoordOfPoint(0);
            t2 = roads[i / 2 + 1]->getCoordOfPoint(3);
        }


        float xTemp, yTemp;
        float a1, a2, b1, b2, c1, c2;

        a1 = p2.y - p1.y;
        b1 = p1.x - p2.x;
        c1 = p1.y * (p2.x - p1.x) - p1.x * (p2.y - p1.y);

        a2 = t2.y - t1.y;
        b2 = t1.x - t2.x;
        c2 = t1.y * (t2.x - t1.x) - t1.x * (t2.y - t1.y);

        if (abs(a1) < 0.0001f)
        {
            yTemp = p1.y;
            if (abs(b2)  < 0.0001f)
                xTemp = t1.x;
            else
                xTemp = (-1.0f) * (b2 * yTemp + c2) / a2;
        }
        else if (abs(b1)  < 0.0001f)
        {
            xTemp = p1.x;
            if (abs(a2)  < 0.0001f)
                yTemp = t1.y;
            else
                yTemp = (-1.0f) * (a2 * xTemp + c2) / b2;
        }
        else if (abs(a2)  < 0.0001f)
        {
            yTemp = t1.y;
            xTemp = (-1.0f) * (b1 * yTemp + c1) / a1;
        }
        else if (abs(b2)  < 0.0001f)
        {
            xTemp = t1.x;
            yTemp = (-1.0f) * (a1 * xTemp + c1) / b1;
        }
        else
        {
            xTemp = (c1 * b2 - c2 * b1) / (a2 * b1 - a1 * b2);
            yTemp = (-1.0f) * (a1 * xTemp + c1) / b1;
        }

        roads[i / 2]->setCoordForPoint(1, xTemp, yTemp, 0.0f);
        if (i == roads.size() * 2 - 1)
        {
            roads[0]->setCoordForPoint(0, xTemp, yTemp, 0.0f);
        }
        else
        {
            roads[i / 2 + 1]->setCoordForPoint(0, xTemp, yTemp, 0.0f);
        }
    }
    setVertexArray();
    for (int i = 0; i < roads.size(); ++i)
        roads[i]->resetLines();
    setTextureArray(texture1USize, texture1VSize);
    //emit intersectionsChanged();


}

void Intersection::calculateRoundings()
{
    if (roads.size() == 0)
        return;

    for (int i = 0; i < roads.size(); ++i)
    {
        vec3 p0 = roads[i]->getCoordOfPoint(1);
        vec3 p1 = roads[i]->getCoordOfPoint(2);
        vec3 p2(0.0f, 0.0f, 0.0f);
        if (i == roads.size() - 1)
            p2 = roads[0]->getCoordOfPoint(3);
        else
            p2 = roads[i + 1]->getCoordOfPoint(3);
        float leftLength = curves[i]->getLeftLength();
        float rightLength = curves[i]->getRightLength();
        float leftR = sqrt((p2.x - p0.x)*(p2.x - p0.x) + (p2.y - p0.y)*(p2.y - p0.y));
        float rightR = sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y));
        float dx = leftLength * (p2.x - p0.x) / leftR;
        float dy = leftLength * (p2.y - p0.y) / leftR;
        curves[i]->setCoordForPoint(0, p0.x, p0.y, p0.z);
        curves[i]->setCoordForPoint(1, p0.x + dx, p0.y + dy, p0.z);

        dx = rightLength * (p1.x - p0.x) / rightR;
        dy = rightLength * (p1.y - p0.y) / rightR;

        curves[i]->setCoordForPoint(2, p0.x + dx, p0.y + dy, p0.z);
        curves[i]->setAngleVertexArray();
        curves[i]->calculateAngle();
        //curves[i]->setAngle(curves[i]->angleRounding);

    }
}

void Intersection::setVertexArray()
{
    vertexArray.clear();

    vec3 p = roads[0]->getCoordOfPoint(0);
    vertexArray.push_back(p.x);
    vertexArray.push_back(p.y);
    vertexArray.push_back(p.z);

    for (int i = 0; i < roads.size(); ++i)
    {
        p = roads[i]->getCoordOfPoint(1);
        vertexArray.push_back(p.x);
        vertexArray.push_back(p.y);
        vertexArray.push_back(p.z);
    }
}

void Intersection::setIndexArray()
{
    indexArray.clear();
    for (int i = 2; i < vertexArray.size() / 3; ++i)
    {
        indexArray.push_back(0);
        indexArray.push_back(i - 1);
        indexArray.push_back(i);
    }
}

void Intersection::setTextureArray(float textureUSize, float textureVSize)
{
    textureArray.clear();

    textureArray.push_back(0.0f);
    textureArray.push_back(0.0f);

    float x1, y1;
    float x2, y2;
    float r1;
    x1 = vertexArray[0];
    y1 = vertexArray[1];
    x2 = vertexArray[3];
    y2 = vertexArray[4];
    r1 = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));

    textureArray.push_back(r1 / textureUSize);
    textureArray.push_back(0.0f);

    for (int i = 2; i < vertexArray.size() / 3; ++i)
    {
        float x3 = vertexArray[i * 3];
        float y3 = vertexArray[i * 3 + 1];
        float r2 = sqrt((x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1));
        float angle = acos(((x3 - x1)*(x2 - x1) + (y3 - y1)*(y2 - y1))/(r1 * r2));
        float dU = r2 * cos(angle);
        float dV = r2 * sin(angle);
        textureArray.push_back(dU / textureUSize);
        textureArray.push_back(dV / textureVSize);
    }
}

void Intersection::addRoad()
{
    int numberOfRoads = roads.size() + 1;
    float x = roads[0]->getAxisPoint_1().x;
    float y = roads[0]->getAxisPoint_1().y;
    float pi = 3.1415926f;

    for (int i = 0; i < roads.size(); ++i)
    {
        delete roads[i];
        delete curves[i];
        curves[i] = NULL;
        roads[i] = NULL;
    }
    roads.clear();
    curves.clear();

    float r = 10.0f;
    for (int i = 0; i < numberOfRoads; ++i)
    {
        float angle = 2.0f * pi / float(numberOfRoads) * float(i);
        float dx = r * cosf(angle);
        float dy = r * sinf(angle);
        RoadSimple *road = new RoadSimple(x, y, x + dx, y + dy, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                          "RoadSimple", 0,
                                          QString("Рукав №") + QString::number(i + 1));
        road->setLeftBoardShowStatus(true);
        road->setRightBoardShowStatus(true);
        //QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(calculateRoadIntersections()));
        QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(recalculateRoads()));
        roads.push_back(road);

    }
    calculateRoadIntersections();

    for (int i = 0; i < roads.size(); ++i)
    {
        vec3 p0 = roads[i]->getCoordOfPoint(1);
        vec3 p1 = roads[i]->getCoordOfPoint(2);
        vec3 p2(0.0f, 0.0f, 0.0f);
        if (i == roads.size() - 1)
            p2 = roads[0]->getCoordOfPoint(3);
        else
            p2 = roads[i + 1]->getCoordOfPoint(3);
        float leftLength = 5.0f;
        float rightLength = 5.0f;
        float leftR = sqrt((p2.x - p0.x)*(p2.x - p0.x) + (p2.y - p0.y)*(p2.y - p0.y));
        float rightR = sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y));
        float dx1 = leftLength * (p2.x - p0.x) / leftR;
        float dy1 = leftLength * (p2.y - p0.y) / leftR;
        float dx2 = rightLength * (p1.x - p0.x) / rightR;
        float dy2 = rightLength * (p1.y - p0.y) / rightR;
        Curve *curve = new Curve(p0.x, p0.y, p0.z,
                                 p0.x + dx1, p0.y + dy1, p0.z,
                                 p0.x + dx2, p0.y + dy2, p0.z,
                                 QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                 QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                 10);
        QObject::connect(curve, SIGNAL(angleChanged(double)), this, SLOT(setAngle(double)));
        showBoardStatus.push_back(false);
        curve->setSelectedStatus(false);
        curves.push_back(curve);
    }

    setRoadsTextures();

    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
    emit roadAdded();
    emit intersectionsChanged(layout, render);
}

void Intersection::deleteRoad()
{
    int numberOfRoads = roads.size() - 1;
    float x = roads[0]->getAxisPoint_1().x;
    float y = roads[0]->getAxisPoint_1().y;
    float pi = 3.1415926f;

    for (int i = 0; i < roads.size(); ++i)
    {
        delete roads[i];
        delete curves[i];
        curves[i] = NULL;
        roads[i] = NULL;
    }
    roads.clear();
    curves.clear();

    float r = 10.0f;
    for (int i = 0; i < numberOfRoads; ++i)
    {
        float angle = 2.0f * pi / float(numberOfRoads) * float(i);
        float dx = r * cosf(angle);
        float dy = r * sinf(angle);
        RoadSimple *road = new RoadSimple(x, y, x + dx, y + dy, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                          "RoadSimple", 0,
                                          QString("Рукав №") + QString::number(i + 1));
        road->setLeftBoardShowStatus(true);
        road->setRightBoardShowStatus(true);
        //QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(calculateRoadIntersections()));
        QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(recalculateRoads()));
        roads.push_back(road);

    }
    calculateRoadIntersections();

    for (int i = 0; i < roads.size(); ++i)
    {
        vec3 p0 = roads[i]->getCoordOfPoint(1);
        vec3 p1 = roads[i]->getCoordOfPoint(2);
        vec3 p2(0.0f, 0.0f, 0.0f);
        if (i == roads.size() - 1)
            p2 = roads[0]->getCoordOfPoint(3);
        else
            p2 = roads[i + 1]->getCoordOfPoint(3);
        float leftLength = 5.0f;
        float rightLength = 5.0f;
        float leftR = sqrt((p2.x - p0.x)*(p2.x - p0.x) + (p2.y - p0.y)*(p2.y - p0.y));
        float rightR = sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y));
        float dx1 = leftLength * (p2.x - p0.x) / leftR;
        float dy1 = leftLength * (p2.y - p0.y) / leftR;
        float dx2 = rightLength * (p1.x - p0.x) / rightR;
        float dy2 = rightLength * (p1.y - p0.y) / rightR;
        Curve *curve = new Curve(p0.x, p0.y, p0.z,
                                 p0.x + dx1, p0.y + dy1, p0.z,
                                 p0.x + dx2, p0.y + dy2, p0.z,
                                 QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                 QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                 10);
        QObject::connect(curve, SIGNAL(angleChanged(double)), this, SLOT(setAngle(double)));
        showBoardStatus.push_back(false);
        curve->setSelectedStatus(false);
        curves.push_back(curve);
    }

    setRoadsTextures();

    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
    emit roadAdded();
    emit intersectionsChanged(layout, render);
}

void Intersection::recalculateRoads()
{
    calculateRoadIntersections();
    calculateRoundings();
    setRoadsTextures();
    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    //qDebug() << "Intersection::recalculateRoads()";
}

void Intersection::setAngle(double angle)
{
    int i = qobject_cast<QDoubleSpinBox*>(sender())->objectName().toInt();
   // Curve * curve = qobject_cast<Curve*>(sender());
    Curve * curve = curves[i];
    for (i = 0; i < curves.size(); ++i)
    {
        if (curves[i] == curve)
            break;
    }
    curve->setAngle(angle);
    vec3 p1 = curve->getCoordOfPoint(0);
    vec3 p2 = curve->getCoordOfPoint(1);
    float xCenter = p1.x;
    float yCenter = p1.y;

    float xLeft = p2.x;
    float yLeft = p2.y;

    float dx = xLeft - xCenter;
    float dy = yLeft - yCenter;

    float r = sqrt(dx*dx + dy*dy);

    vec2 s1, s2;
    RoadSimple* road;
    if (i == curves.size() - 1)
    {
        road = roads[0];
    }
    else
    {
        road = roads[i + 1];
    }

    s1 = road->getAxisPoint_1();
    s2 = road->getAxisPoint_2();
    float dx1 = s2.x - s1.x;
    float dy1 = s2.y - s1.y;
    float r1 = sqrt(dx1*dx1 + dy1*dy1);
    road->setCoordForAxisPoint(1, s1.x + dx / r * r1, s1.y + dy / r * r1);
    /*
    float an = acos(dx / r);
    if (dy < 0)
        an = 2.0f * 3.1415926f - an;

    float r1 = sqrt((s2.x - s1.x)*(s2.x - s1.x) + (s2.y - s1.y)*(s2.y - s1.y));
    float x = s1.x + r1 * cos(an);
    float y = s1.y + r1 * sin(an);

    road->setCoordForAxisPoint(1, x, y);
    */
    road->setVertexArray();
    road->setTextureArray();
    road->setTextureArrayBoard();

    recalculateRoads();

}

void Intersection::setAngle(double angle, int index)
{

}

void Intersection::deleteLine()
{
    emit linesChanged(layout, render);
}

void Intersection::addLine()
{
    emit linesChanged(layout, render);
}

bool Intersection::setFixed(bool fixed)
{
    this->fixed = fixed;
}


void Intersection::clear()
{
}


void Intersection::clearProperties(QLayout *layout)
{
    for (int i = 0; i < roads.size(); ++i)
    {
        disconnect(stepDialog, SIGNAL(lineTypeChanged(int)), roads[i], SLOT(setLineType(int)));
        disconnect(stepDialog, SIGNAL(rightSideChanged(bool)), roads[i], SLOT(setRightSide(bool)));
        disconnect(stepDialog, SIGNAL(stepChanged(double)), roads[i], SLOT(setStep(double)));
        disconnect(stepDialog, SIGNAL(beginStepChanged(double)), roads[i], SLOT(setBeginStep(double)));
        disconnect(stepDialog, SIGNAL(endStepChanged(double)), roads[i], SLOT(setEndStep(double)));
        disconnect(stepDialog, SIGNAL(beginSideChanged(bool)), roads[i], SLOT(setBeginSide(bool)));
        disconnect(stepDialog, SIGNAL(beginRoundingChanged(bool)), roads[i], SLOT(setBeginRounding(bool)));
        disconnect(stepDialog, SIGNAL(endRoundingChanged(bool)), roads[i], SLOT(setEndRounding(bool)));
        disconnect(stepDialog, SIGNAL(splitZoneWidthChanged(double)), roads[i], SLOT(setSplitZoneWidth(double)));
        disconnect(stepDialog, SIGNAL(differentDirectionsChanged(bool)), roads[i], SLOT(setDifferentDirections(bool)));
        disconnect(stepDialog, SIGNAL(accepted()), roads[i], SLOT(addLine()));
        disconnect(stepDialog, SIGNAL(accepted()), this, SLOT(addLine()));
    }
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
}
