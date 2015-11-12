#include "intersection.h"
#include "assert.h"
#include <QApplication>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include "mainwindow.h"

bool Intersection::log = true;

Intersection::Intersection()
{
    name = "Intersection";
    selected = false;
    fixed = false;
    layer = 0;
}

Intersection::Intersection(float x, float y)
{
    name = "Intersection";
    indexOfSelectedControl = -1;
    layer = 0;
    float r = 10.0f;
    float pi = 3.14159265f;
    for (int i = 0; i < 4; ++i)
    {
        float angle = pi / 2.0f * float(i);
        float dx = r * cosf(angle);
        float dy = r * sinf(angle);
        RoadSimple *road = new RoadSimple(x, y, x + dx, y + dy, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                          "RoadSimple", 0,
                                          QString("Рукав №") + QString::number(i + 1));
        road->setLeftBoardShowStatus(false);
        road->setRightBoardShowStatus(false);
        //QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(recalculateRoads()));
        connect(road, SIGNAL(widthChanged(double)), this, SLOT(resetWidth()));
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
            curve->setBoardShowStatus(true);
            showBoardStatus.push_back(true);
            curve->setSelectedStatus(false);
            curves.push_back(curve);
        }

    texture1USize = 6.0f;
    texture1VSize = 6.0f;
    texture2USize = 2.75f;
    texture2VSize = 6.0f;
    texture1 = QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg";
    texture2 = QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg";
    textureID[0] = TextureManager::getInstance()->getID(texture1);
    textureID[1] = TextureManager::getInstance()->getID(texture2);

    setRoadsTextures();

    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
    selected = fixed = false;
    //QObject::connect(this, SIGNAL(intersectionsChanged()),this, SLOT(calculateRoundings()));
    connect(this, SIGNAL(intersectionsChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

Intersection::Intersection(const Intersection &source)
{
    name = "Intersection";
    indexOfSelectedControl = source.indexOfSelectedControl;
    layer = source.layer;

    for (int i = 0; i < source.roads.size(); ++i)
    {
        RoadSimple* road = qobject_cast<RoadSimple*>(source.roads[i]->getCopy());
        connect(road, SIGNAL(widthChanged(double)), this, SLOT(resetWidth()));
        roads.push_back(road);
    }
    calculateRoadIntersections();

        for (int i = 0; i < source.curves.size(); ++i)
        {
            Curve *curve = qobject_cast<Curve*>(source.curves[i]->getCopy());
            curves.push_back(curve);
        }

    texture1USize = source.texture1USize;
    texture1VSize = source.texture1VSize;
    texture2USize = source.texture2USize;
    texture2VSize = source.texture2VSize;
    texture1 = source.texture1;
    texture2 = source.texture2;
    textureID[0] = source.textureID[0];
    textureID[1] = source.textureID[1];

    setRoadsTextures();

    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
    selected = source.selected;
    fixed = source.fixed;
    connect(this, SIGNAL(intersectionsChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

Intersection::Intersection(QVector<RoadSimple *> &roads, QVector<Curve *> &curves)
{
    name = "Intersection";
    indexOfSelectedControl = -1;
    layer = 0;

    for (int i = 0; i < roads.size(); ++i)
    {
        this->roads.push_back(roads[i]);
    }
    calculateRoadIntersections();
    for (int i = 0; i < curves.size(); ++i)
    {
        this->curves.push_back(curves[i]);
    }
    texture1USize = 6.0f;
    texture1VSize = 6.0f;
    texture2USize = 2.75f;
    texture2VSize = 6.0f;
    texture1 = QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg";
    texture2 = QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg";
    textureID[0] = TextureManager::getInstance()->getID(texture1);
    textureID[1] = TextureManager::getInstance()->getID(texture2);

    setRoadsTextures();

    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
    selected = fixed = false;
    connect(this, SIGNAL(intersectionsChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

Intersection::~Intersection()
{
    for (int i = 0; i < roads.size(); ++i)
    {
        if (roads[i])
            delete roads[i];
        roads[i] = NULL;
    }
        for (int i = 0; i < curves.size(); ++i)
        {
            if (curves[i])
            {
                delete curves[i];
            }
            curves[i] = NULL;
        }
    model = NULL;
    layout = NULL;
    render = NULL;
}



bool Intersection::isSelected()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::isSelected()\n";
    return selected;
}

void Intersection::setSelectedStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setSelectedStatus(bool status)"
                                       << " status = " << status << "\n";
    selected = status;
    for (int i = 0; i < roads.size(); ++i)
    {
        roads[i]->setSelectedStatus(status);
    }
        for (int i = 0; i < curves.size(); ++i)
        {
            curves[i]->setSelectedStatus(status);
        }

}

void Intersection::drawFigure(QGLWidget *render)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::drawFigure(QGLWidget *render)\n";

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
            curves[i]->drawFigure(render);
        }


    for (int i = 0; i < roads.size(); ++i)
    {        
        roads[i]->drawFigure();
    }

    if (indexOfSelectedControl >=0 && indexOfSelectedControl < getNumberOfControls())
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0f);
}

void Intersection::drawSelectionFrame()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::drawSelectionFrame()\n";
}

void Intersection::drawMeasurements(QGLWidget *render)
{
    if (!showMeasurements)
        return;
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::drawMeasurements(QGLWidget *render)\n";
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

    for (int i = 0; i < roads.size(); ++i)
    {

        roads[i]->drawDescription(render, 1.0f, 0.0f, 0.0f);
        roads[i]->drawMeasurements(render);
    }
}

void Intersection::move(float dx, float dy, float dz)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::move(float dx, float dy, float dz)"
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " dz = " << dz << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::drawControlElement(int index, float lineWidth, float pointSize)"
                                       << " index = " << index
                                       << " lineWidth = " << lineWidth
                                       << " pointSize = " << pointSize << "\n";
    int i;
    for (i = 0; i < roads.size(); ++i)
    {
        if (index >= (roads[i]->getNumberOfControls() + 1))
            index -= (roads[i]->getNumberOfControls() + 1);
        else
            break;
    }
        if (i >= roads.size())
        {
            for (i = 0; i < curves.size(); ++i)
            {
                if (index >= (curves[i]->getNumberOfControls() + 1))
                    index -= (curves[i]->getNumberOfControls() + 1);
                else
                    curves[i]->drawControlElement(index, lineWidth, pointSize);
            }
        }
        else
    if (index == roads[i]->getNumberOfControls())
        roads[i]->drawFigure();
    else
    {
        roads[i]->drawControlElement(index, lineWidth, pointSize);
    }

}

QCursor Intersection::getCursorForControlElement(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::getCursorForControlElement(int index)"
                                       << " index = " << index << "\n";
    return Qt::CrossCursor;
}

void Intersection::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::resizeByControl(int index, float dx, float dy, float x, float y)"
                                       << " index = " << index
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " x = " << x
                                       << " y = " << y << "\n";
    if (fixed)
        return;

    int i;
    for (i = 0; i < roads.size(); ++i)
    {
        if (index >= (roads[i]->getNumberOfControls() + 1))
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
        int count = roads[i]->getNumberOfControls() - 12;
        if (index == count + 5)
        {
            // Поворот
            roads[i]->resizeByControl(index, dx, dy, x, y);
        }else
            if (index == count + 7)
            {
                // Длина
                roads[i]->resizeByControl(index, dx, dy, x, y);
            }else
                if (index == count + 8)
                {
                    // Ширина правая
                    roads[i]->resizeByControl(index, dx, dy, x, y);
                }else
                    if (index == count + 9)
                    {
                        // Ширина левая
                        roads[i]->resizeByControl(index, dx, dy, x, y);
                    }else
                        if (index == count + 10)
                        {
                            // Ширина травого тротуара
                            roads[i]->resizeByControl(index, dx, dy, x, y);
                        }else
                            if (index == count + 11)
                            {
                                // Ширина левого тротуара
                                roads[i]->resizeByControl(index, dx, dy, x, y);
                            }
        // Если дорогу повернули
        //calculateRoadIntersections(i, index);
        //calculateRoadIntersections();
        //        calculateRoadForRounding(i, index);
        calculateRoadForAngle(i, index);



         //Изменение ширины тротуаров у рукава
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
        //        calculateRoadForMoving(i, dx, dy);
        vec2 p1 = roads[i]->getAxisPoint_1();
        vec2 p2 = roads[i]->getAxisPoint_2();
        float width = roads[i]->getWidth();

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

                /*
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
                */

                if (calculateLinesIntersection(a1, b1, c1, a2, b2, c2, x1Temp, y1Temp))

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
        //calculateRoundings();
        //calculateRoadForMoving(i, dx, dy);
    }


    calculateRoadIntersections();
    calculateRoundings();
    setRoadsTextures();
    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
}

int Intersection::getNumberOfControls()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::getNumberOfControls()\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::controlsForPoint()\n";
    return 1;
}

void Intersection::changeColorOfSelectedControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::changeColorOfSelectedControl(int index)"
                                       << " index = " << index << "\n";
    indexOfSelectedControl = index;
}

void Intersection::getProperties(QFormLayout *layout, QGLWidget *render)
{
        if (log)
            Logger::getLogger()->infoLog() << "Intersection::getProperties(QFormLayout *layout, QGLWidget *render)\n";
        if (layout == NULL)
        {
            QMessageBox::critical(0, "Ошибка", "Intersection::getProperties(QFormLayout *layout, QGLWidget *render) layout = NULL",
                                  QMessageBox::Yes);
            if (log)
                Logger::getLogger()->errorLog() << "Intersection::getProperties(QFormLayout *layout, QGLWidget *render) layout = NULL\n";
            QApplication::exit(0);
        }
        this->layout = layout;
        this->render = render;

        clearProperties(layout);


        QCheckBox *fixedCheckBox = new QCheckBox();
        fixedCheckBox->setChecked(fixed);
        QObject::connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));
        layout->addRow("Зафиксировать", fixedCheckBox);

        QCheckBox *showMeasurementsCheckBox = new QCheckBox();
        showMeasurementsCheckBox->setChecked(showMeasurements);
        connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowMeasurements(bool)));
        connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
        layout->addRow("Размеры", showMeasurementsCheckBox);

        for (int i = 0; i < roads.size(); ++i)
        {
            QPushButton *deletePushButton = new QPushButton("Удалить");
            deletePushButton->setEnabled(false);
            layout->addRow("Рукав " + QString::number(i + 1),deletePushButton);
            if (roads.size() > 3)
            {
                deletePushButton->setEnabled(true);
                connect(deletePushButton, SIGNAL(clicked(bool)), this, SLOT(deleteRoad()));

            }

            QDoubleSpinBox *widthDoubleSpinBox = new QDoubleSpinBox();
            widthDoubleSpinBox->setKeyboardTracking(false);
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

            connect(stepDialogs[i], SIGNAL(lineTypeChanged(int)), roads[i], SLOT(setLineType(int)));
            connect(stepDialogs[i], SIGNAL(rightSideChanged(bool)), roads[i], SLOT(setRightSide(bool)));
            connect(stepDialogs[i], SIGNAL(stepChanged(double)), roads[i], SLOT(setStep(double)));
            connect(stepDialogs[i], SIGNAL(beginStepChanged(double)), roads[i], SLOT(setBeginStep(double)));
            connect(stepDialogs[i], SIGNAL(endStepChanged(double)), roads[i], SLOT(setEndStep(double)));
            connect(stepDialogs[i], SIGNAL(beginSideChanged(bool)), roads[i], SLOT(setBeginSide(bool)));
            connect(stepDialogs[i], SIGNAL(beginRoundingChanged(bool)), roads[i], SLOT(setBeginRounding(bool)));
            connect(stepDialogs[i], SIGNAL(endRoundingChanged(bool)), roads[i], SLOT(setEndRounding(bool)));
            connect(stepDialogs[i], SIGNAL(splitZoneWidthChanged(double)), roads[i], SLOT(setSplitZoneWidth(double)));
            connect(stepDialogs[i], SIGNAL(differentDirectionsChanged(bool)), roads[i], SLOT(setDifferentDirections(bool)));
            connect(stepDialogs[i], SIGNAL(singleWayChanged(bool)), roads[i], SLOT(setSingleWay(bool)));
            connect(stepDialogs[i], SIGNAL(axisStepChanged(double)), roads[i], SLOT(setAxisStep(double)));
            connect(stepDialogs[i], SIGNAL(splitZoneTypeChanged(int)), roads[i], SLOT(setSplitZoneType(int)));
            connect(stepDialogs[i], SIGNAL(splitZoneHeightChanged(double)), roads[i], SLOT(setSplitZoneHeight(double)));
            connect(addLinePushButton, SIGNAL(clicked(bool)), stepDialogs[i], SLOT(exec()));
            connect(stepDialogs[i], SIGNAL(accepted()), roads[i], SLOT(addLine()));
            connect(stepDialogs[i], SIGNAL(accepted()), this, SLOT(addLine()));
            if (render)
            {
                connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)), render, SLOT(updateGL()));
            }
            for (int j = 0; j < roads[i]->lines.size(); ++j)
            {
                QPushButton* b = new QPushButton(QString::number(j + 1));
                connect(b, SIGNAL(clicked(bool)), roads[i], SLOT(deleteLine()));
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
            leftLengthDoubleSpinBox->setKeyboardTracking(false);
            leftLengthDoubleSpinBox->setValue(curves[i]->getLeftLength());
            leftLengthDoubleSpinBox->setMinimum(0.0);
            QObject::connect(curves[i], SIGNAL(leftLengthChanged(double)), leftLengthDoubleSpinBox, SLOT(setValue(double)));
            QObject::connect(leftLengthDoubleSpinBox, SIGNAL(valueChanged(double)), curves[i], SLOT(setLeftLength(double)));

            QDoubleSpinBox *rightLengthDoubleSpinBox = new QDoubleSpinBox();
            rightLengthDoubleSpinBox->setKeyboardTracking(false);
            rightLengthDoubleSpinBox->setValue(curves[i]->getRightLength());
            rightLengthDoubleSpinBox->setMinimum(0.0);
            QObject::connect(curves[i], SIGNAL(rightLengthChanged(double)), rightLengthDoubleSpinBox, SLOT(setValue(double)));
            QObject::connect(rightLengthDoubleSpinBox, SIGNAL(valueChanged(double)), curves[i], SLOT(setRightLength(double)));

            QCheckBox *showBoardCheckBox = new QCheckBox();
            showBoardCheckBox->setChecked(curves[i]->getBoardShowStatus());
            QObject::connect(showBoardCheckBox, SIGNAL(toggled(bool)), curves[i], SLOT(setBoardShowStatus(bool)));
            QObject::connect(showBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));

            QDoubleSpinBox *angleDoubleSpinBox = new QDoubleSpinBox();
            angleDoubleSpinBox->setKeyboardTracking(false);
            angleDoubleSpinBox->setObjectName(QString::number(i));
            angleDoubleSpinBox->setMinimum(0.0);
            angleDoubleSpinBox->setMaximum(720.0);
            angleDoubleSpinBox->setValue(curves[i]->angleRounding);
            connect(curves[i], SIGNAL(angleChanged(double)), angleDoubleSpinBox, SLOT(setValue(double)));
            connect(angleDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngle(double)));
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
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::isFixed()\n";
    return fixed;
}

int Intersection::getLayer()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::getLayer()\n";
    return layer;
}

void Intersection::setRoadsTextures()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setRoadsTextures()\n";
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

bool Intersection::calculateLinesIntersection(float a1, float b1, float c1,
                                              float a2, float b2, float c2,
                                              float &x, float &y)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::calculateLinesIntersection(float a1, float b1, float c1,float a2, float b2, float c2,float &x, float &y)"
                                       << " a1 = " << a1 << " b1 = " << b1 << " c1 = " << c1
                                       << " a2 = " << a2 << " b2 = " << b2 << " c2 = " << c2 << "\n";
    float eps = 1e-9;
    float den = a1 * b2 - a2 * b1;
    float numX = c1 * b2 - c2 * b1;
    float numY = a1 * c2 - a2 * c1;
    if (fabs(den) < eps)
        return false;
    if (fabs(numX) < eps && fabs(numY) < eps)
        return false;
    x = (-1.0f) * numX / den;
    y = (-1.0f) * numY / den;

    return true;
}

float Intersection::calculateAngle(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{    
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::calculateAngle(vec2 p1, vec2 p2, vec2 p3, vec2 p4)"
                                       << " p1.x = " << p1.x << " p1.y = " << p1.y
                                       << " p2.x = " << p2.x << " p2.y = " << p2.y
                                       << " p3.x = " << p3.x << " p3.y = " << p3.y
                                       << " p4.x = " << p4.x << " p4.y = " << p4.y << "\n";
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float r1 = sqrt(dx1*dx1 + dy1*dy1);
    float dx2 = p4.x - p3.x;
    float dy2 = p4.y - p3.y;
    float r2 = sqrt(dx2*dx2 + dy2*dy2);
    float t = (dx1*dx2 + dy1*dy2) / (r1 * r2);
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float angle = acos(t);
    float res = dx1*dy2 - dx2*dy1;
    float pi = 3.14159265f;
    if (res < 0)
        angle = 2.0f * pi - angle;
    return angle;

}

void Intersection::calculateRoadForAngle(int i, int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::calculateRoadForAngle(int i, int index)"
                                       << " i = " << i
                                       << " index = " << index << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::calculateRoadForRounding(int i, int index)"
                                       << " i = " << i
                                       << " index = " << index << "\n";
    if (index < (roads[i]->getNumberOfControls() - 12))
        return;
    int num = 12 - (roads[i]->getNumberOfControls() - index);
    switch (num)
    {
    // Повороты
    case 2:
    case 3:
    case 5:
    {
        // Левая и правая стороны соседних руковов
        int j = i == (roads.size() - 1) ? 0 : i + 1;
        vec3 p1 = roads[j]->getCoordOfPoint(0);
        vec3 p2 = roads[j]->getCoordOfPoint(3);
        float dx1 = p2.x - p1.x;
        float dy1 = p2.y - p1.y;
        float dx, dy;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float leftLength = curves[i]->getLeftLength();
        if (r1 < leftLength)
        {
            dx = dx1 / r1 * (r1 - leftLength);
            dy = dy1 / r1 * (r1 - leftLength);
            roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
        }

        j = i == 0 ? roads.size() - 1 : i - 1;
        p1 = roads[j]->getCoordOfPoint(1);
        p2 = roads[j]->getCoordOfPoint(2);
        dx1 = p2.x - p1.x;
        dy1 = p2.y - p1.y;
        r1 = sqrt(dx1*dx1 + dy1*dy1);
        float rightLength = curves[j]->getRightLength();
        if (r1 < rightLength)
        {
            dx = dx1 / r1 * (r1 - rightLength);
            dy = dy1 / r1 * (r1 - rightLength);
            roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
        }

        // Левая и правая стороны текущего рукава
        p1 = roads[i]->getCoordOfPoint(1);
        p2 = roads[i]->getCoordOfPoint(2);
        dx1 = p2.x - p1.x;
        dy1 = p2.y - p1.y;
        r1 = sqrt(dx1*dx1 + dy1*dy1);
        rightLength = curves[i]->getRightLength();
        if (r1 < rightLength)
        {
            int j = i == (roads.size() - 1) ? 0 : i + 1;
            vec3 p3 = roads[j]->getCoordOfPoint(0);
            vec3 p4 = roads[j]->getCoordOfPoint(3);
            float dx2 = p3.x - p4.x;
            float dy2 = p3.y - p4.y;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            dx = dx2 / r2 * (rightLength - r1);
            dy = dy2 / r2 * (rightLength - r1);
            roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
        }

        p1 = roads[i]->getCoordOfPoint(0);
        p2 = roads[i]->getCoordOfPoint(3);
        dx1 = p2.x - p1.x;
        dy1 = p2.y - p1.y;
        r1 = sqrt(dx1*dx1 + dy1*dy1);
        j = i == 0 ? roads.size() - 1 : i - 1;
        leftLength = curves[j]->getLeftLength();
        rightLength = curves[j]->getRightLength();
        if (r1 < leftLength)
        {
            vec3 p3 = roads[j]->getCoordOfPoint(1);
            vec3 p4 = roads[j]->getCoordOfPoint(2);
            float dx2 = p3.x - p4.x;
            float dy2 = p3.y - p4.y;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            dx = dx2 / r2 * (leftLength - r1);
            dy = dy2 / r2 * (leftLength - r1);
            roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
        }
    }
        break;
        // Торец
    case 7:
    {
        vec3 p1 = roads[i]->getCoordOfPoint(1);
        vec3 p2 = roads[i]->getCoordOfPoint(2);
        float dx1 = p2.x - p1.x;
        float dy1 = p2.y - p1.y;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx, dy;
        float rightLength = curves[i]->getRightLength();
        if (r1 < rightLength)
        {
            dx = -1.0f * dx1 / r1 * (r1 - rightLength);
            dy = -1.0f * dy1 / r1 * (r1 - rightLength);
            roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
        }
        float j = i == 0 ? roads.size() - 1 : i - 1;
        vec3 p3 = roads[i]->getCoordOfPoint(0);
        vec3 p4 = roads[i]->getCoordOfPoint(3);
        float dx2 = p4.x - p3.x;
        float dy2 = p4.y - p3.y;
        float r2 = sqrt(dx2*dx2 + dy2*dy2);
        float leftLength = curves[j]->getLeftLength();
        if (r2 < leftLength)
        {
            dx = -1.0f * dx2 / r2 * (r2 - leftLength);
            dy = -1.0f * dy2 / r2 * (r2 - leftLength);
            roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
        }
    }
        break;
        // Боковые стороны
    case 8:
    {
        float j = i == 0 ? roads.size() - 1 : i - 1;
        vec3 p1 = roads[j]->getCoordOfPoint(1);
        vec3 p2 = roads[j]->getCoordOfPoint(2);
        float dx1 = p2.x - p1.x;
        float dy1 = p2.y - p1.y;
        float dx, dy;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float rightLength = curves[i]->getRightLength();
        if (r1 < rightLength)
        {
            dx = dx1 / r1 * (r1 - rightLength);
            dy = dy1 / r1 * (r1 - rightLength);
            roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
        }
    }
        break;
    case 9:
    {
        int j = i == (roads.size() - 1) ? 0 : i + 1;
        vec3 p1 = roads[j]->getCoordOfPoint(0);
        vec3 p2 = roads[j]->getCoordOfPoint(3);
        float dx1 = p2.x - p1.x;
        float dy1 = p2.y - p1.y;
        float dx, dy;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float leftLength = curves[i]->getLeftLength();
        if (r1 < leftLength)
        {
            dx = dx1 / r1 * (r1 - leftLength);
            dy = dy1 / r1 * (r1 - leftLength);
            roads[i]->resizeByControl(index, dx, dy, p2.x, p2.y);
        }
    }
        break;
dafault:
        break;
    }


}

void Intersection::calculateRoadForMoving(int i, float dx, float dy)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::calculateRoadForMoving(int i, float dx, float dy)"
                                       << " i = " << i
                                       << " dx = " << dx
                                       << " dy = " << dy << "\n";
    float r = sqrt(dx*dx + dy*dy);
    int j = i == (roads.size() - 1) ? 0 : i + 1;
    vec3 p1 = roads[j]->getCoordOfPoint(0);
    vec3 p2 = roads[j]->getCoordOfPoint(3);
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float r1 = sqrt(dx1*dx1 + dy1*dy1);
    float leftLength = curves[i]->getLeftLength();
    if (r1 < leftLength)
    {
        roads[i]->move((-1.0f)*dx / r * (leftLength - r1),
                       (-1.0f)*dy / r * (leftLength - r1));
    }

    j = i == 0 ? roads.size() - 1 : i - 1;
    p1 = roads[j]->getCoordOfPoint(1);
    p2 = roads[j]->getCoordOfPoint(2);
    dx1 = p2.x - p1.x;
    dy1 = p2.y - p1.y;
    r1 = sqrt(dx1*dx1 + dy1*dy1);
    float rightLength = curves[j]->getRightLength();
    if (r1 < rightLength)
    {
        roads[i]->move((-1.0f)*dx / r * (rightLength - r1),
                       (-1.0f)*dy / r * (rightLength - r1));
    }

    p1 = roads[i]->getCoordOfPoint(1);
    p2 = roads[i]->getCoordOfPoint(2);
    dx1 = p2.x - p1.x;
    dy1 = p2.y - p1.y;
    r1 = sqrt(dx1*dx1 + dy1*dy1);
    rightLength = curves[i]->getRightLength();
    if (r1 < rightLength)
    {
        roads[i]->move((-1.0f)*dx / r * (rightLength - r1),
                       (-1.0f)*dy / r * (rightLength - r1));
    }

    j = i == 0 ? roads.size() - 1 : i - 1;
    p1 = roads[i]->getCoordOfPoint(0);
    p2 = roads[i]->getCoordOfPoint(3);
    dx1 = p2.x - p1.x;
    dy1 = p2.y - p1.y;
    r1 = sqrt(dx1*dx1 + dy1*dy1);
    leftLength = curves[j]->getLeftLength();
    if (r1 < rightLength)
    {
        roads[i]->move((-1.0f)*dx / r * (leftLength - r1),
                       (-1.0f)*dy / r * (leftLength - r1));
    }


    /*
    vec3 p1 = roads[i]->getCoordOfPoint(1);
    vec3 p2 = roads[i]->getCoordOfPoint(2);
    int j = i == roads.size() - 1 ? 0 : i + 1;
    vec3 p3 = roads[j]->getCoordOfPoint(0);
    vec3 p4 = roads[j]->getCoordOfPoint(3);
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float dx2 = p4.x - p1.x;
    float dy2 = p4.y - p1.y;
    float res = dx1 * dy2 - dx2 * dy1;
    if (res < 0)
    {
        float a1, b1, c1;
        float a2, b2, c2;
        float x, y;
        a1 = p1.y - p2.y;
        b1 = p2.x - p1.x;
        c1 = p1.x * p2.y - p2.x * p1.y;

        a2 = p3.y - p4.y;
        b2 = p4.x - p3.x;
        c2 = p3.x * p4.y - p4.x * p3.y;

        if (!calculateLinesIntersection(a1, b1, c1, a2, b2, c2, x, y))
            return;
        dx = p4.x - x;
        dy = p4.y - y;
        roads[i]->move(dx, dy);

    }

    p1 = roads[i]->getCoordOfPoint(0);
    p2 = roads[i]->getCoordOfPoint(3);
    j = i == 0 ? roads.size() - 1 : i - 1;
    p3 = roads[j]->getCoordOfPoint(1);
    p4 = roads[j]->getCoordOfPoint(2);
    dx1 = p2.x - p1.x;
    dy1 = p2.y - p1.y;
    dx2 = p4.x - p1.x;
    dy2 = p4.y - p1.y;
    res = dx1 * dy2 - dx2 * dy1;
    if (res > 0)
    {
        float a1, b1, c1;
        float a2, b2, c2;
        float x, y;
        a1 = p1.y - p2.y;
        b1 = p2.x - p1.x;
        c1 = p1.x * p2.y - p2.x * p1.y;

        a2 = p3.y - p4.y;
        b2 = p4.x - p3.x;
        c2 = p3.x * p4.y - p4.x * p3.y;

        if (!calculateLinesIntersection(a1, b1, c1, a2, b2, c2, x, y))
            return;
        dx = p4.x - x;
        dy = p4.y - y;
        roads[i]->move(dx, dy);

    }
    */
}

bool Intersection::calculateRoadIntersections()
{    
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::calculateRoadIntersections()\n";
    for (int i = 0; i < roads.size(); ++i)
        roads[i]->setVertexArray();
    for (int i = 0; i < roads.size(); ++i)
    {

        vec3 p1 = roads[i]->getCoordOfPoint(1);
        vec3 p2 = roads[i]->getCoordOfPoint(2);

        int j = i == roads.size() - 1 ? 0 : i + 1;

        vec3 t1 = roads[j]->getCoordOfPoint(0);
        vec3 t2 = roads[j]->getCoordOfPoint(3);

        float xTemp, yTemp;
        float a1, a2, b1, b2, c1, c2;

        a1 = p1.y - p2.y;
        b1 = p2.x - p1.x;
        c1 = p1.x * p2.y - p2.x * p1.y;

        a2 = t1.y - t2.y;
        b2 = t2.x - t1.x;
        c2 = t1.x * t2.y - t2.x * t1.y;


        /*
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
        */

        // Если не нашли точку пересечения, то выходим из цикла и продолжаем работу
        if (!calculateLinesIntersection(a1, b1, c1,
                                        a2, b2, c2,
                                        xTemp, yTemp))
            break;


        roads[i]->setCoordForPoint(1, xTemp, yTemp, 0.0f);
        roads[j]->setCoordForPoint(0, xTemp, yTemp, 0.0f);

    }
    //setVertexArray();
    for (int i = 0; i < roads.size(); ++i)
    {
        roads[i]->setTextureArray();
    }

    //setTextureArray(texture1USize, texture1VSize);
    //emit intersectionsChanged();
    return true;

}

void Intersection::calculateRoadIntersections(int roadIndex, int controlIndex)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::calculateRoadIntersections(int roadIndex, int controlIndex)"
                                       << " roadIndex = " << roadIndex
                                       << " controlIndex = " << controlIndex << "\n";
    for (int i = 0; i < roads.size(); ++i)
        roads[i]->setVertexArray();


    vec3 p1 = roads[roadIndex]->getCoordOfPoint(0);
    vec3 p2 = roads[roadIndex]->getCoordOfPoint(3);

    int i = roadIndex == 0 ? roads.size() - 1  : roadIndex - 1;

    vec3 t1 = roads[i]->getCoordOfPoint(1);
    vec3 t2 = roads[i]->getCoordOfPoint(2);

    float xTemp, yTemp;
    float a1, a2, b1, b2, c1, c2;

    a1 = p1.y - p2.y;
    b1 = p2.x - p1.x;
    c1 = p1.x * p2.y - p2.x * p1.y;

    a2 = t1.y - t2.y;
    b2 = t2.x - t1.x;
    c2 = t1.x * t2.y - t2.x * t1.y;

    // Если не нашли точку пересечения, то выходим из цикла и продолжаем работу
    calculateLinesIntersection(a1, b1, c1,
                               a2, b2, c2,
                               xTemp, yTemp);
    // Если отрезки пересекаются
    bool xToP = (p1.x >= xTemp && p2.x <= xTemp) || (p1.x <= xTemp && p2.x >= xTemp);
    bool yToP = (p1.y >= yTemp && p2.y <= yTemp) || (p1.y <= yTemp && p2.y >= yTemp);
    bool xToT = (t1.x >= xTemp && t2.x <= xTemp) || (t1.x <= xTemp && t2.x >= xTemp);
    bool yToT = (t1.y >= yTemp && t2.y <= yTemp) || (t1.y <= yTemp && t2.y >= yTemp);

    if (!xToP && !yToP && !xToT && !yToT)
    {
        vec2 axisPoint = roads[roadIndex]->getAxisPoint_1();
        float dx = p2.x - axisPoint.x;
        float dy = p2.y - axisPoint.y;
        float l = sqrt(dx*dx + dy*dy);
        dx = t2.x - axisPoint.x;
        dy = t2.y - axisPoint.y;
        float r = sqrt(dx*dx + dy*dy);
        float dx1 = dx / r * l;
        float dy1 = dy / r * l;
        float x1 = axisPoint.x + dx1;
        float y1 = axisPoint.y + dy1;
        dx = x1 - p2.x;
        dy = y1 - p2.y;
        roads[roadIndex]->resizeByControl(controlIndex,dx,dy,p2.x,p2.y);
    }
    else
    {
        roads[i]->setCoordForPoint(1, xTemp, yTemp, 0.0f);
        roads[roadIndex]->setCoordForPoint(0, xTemp, yTemp, 0.0f);
    }

    p1 = roads[roadIndex]->getCoordOfPoint(1);
    p2 = roads[roadIndex]->getCoordOfPoint(2);

    i = roadIndex == roads.size() - 1 ?  0 : roadIndex + 1;

    t1 = roads[i]->getCoordOfPoint(0);
    t2 = roads[i]->getCoordOfPoint(3);


    a1 = p1.y - p2.y;
    b1 = p2.x - p1.x;
    c1 = p1.x * p2.y - p2.x * p1.y;

    a2 = t1.y - t2.y;
    b2 = t2.x - t1.x;
    c2 = t1.x * t2.y - t2.x * t1.y;

    // Если не нашли точку пересечения, то выходим из цикла и продолжаем работу
    calculateLinesIntersection(a1, b1, c1,
                                    a2, b2, c2,
                                    xTemp, yTemp);

    // Если отрезки пересекаются
    xToP = (p1.x >= xTemp && p2.x <= xTemp) || (p1.x <= xTemp && p2.x >= xTemp);
    yToP = (p1.y >= yTemp && p2.y <= yTemp) || (p1.y <= yTemp && p2.y >= yTemp);
    xToT = (t1.x >= xTemp && t2.x <= xTemp) || (t1.x <= xTemp && t2.x >= xTemp);
    yToT = (t1.y >= yTemp && t2.y <= yTemp) || (t1.y <= yTemp && t2.y >= yTemp);

    if (!xToP && !yToP && !xToT && !yToT)
    {
        vec2 axisPoint = roads[roadIndex]->getAxisPoint_1();
        float dx = p2.x - axisPoint.x;
        float dy = p2.y - axisPoint.y;
        float l = sqrt(dx*dx + dy*dy);
        dx = t2.x - axisPoint.x;
        dy = t2.y - axisPoint.y;
        float r = sqrt(dx*dx + dy*dy);
        float dx1 = dx / r * l;
        float dy1 = dy / r * l;
        float x1 = axisPoint.x + dx1;
        float y1 = axisPoint.y + dy1;
        dx = x1 - p2.x;
        dy = y1 - p2.y;
        roads[roadIndex]->resizeByControl(controlIndex,dx,dy,p2.x,p2.y);
    }
    else
    {
        roads[i]->setCoordForPoint(0, xTemp, yTemp, 0.0f);
        roads[roadIndex]->setCoordForPoint(1, xTemp, yTemp, 0.0f);
    }

    for (int i = 0; i < roads.size(); ++i)
    {
        roads[i]->setTextureArray();
    }
}

void Intersection::calculateRoundings()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::calculateRoundings()\n";
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
        curves[i]->calculateAngle();
        curves[i]->setAngleVertexArray();

        ////qDebug() << "Angle" << i << ":" << curves[i]->angleRounding;
        //curves[i]->setAngle(curves[i]->angleRounding);

    }

}

void Intersection::setVertexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setVertexArray()\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setIndexArray()\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setTextureArray(float textureUSize, float textureVSize)"
                                       << " textureUSize = " << textureUSize
                                       << " textureVSize = " << textureVSize << "\n";
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
        float t = ((x3 - x1)*(x2 - x1) + (y3 - y1)*(y2 - y1))/(r1 * r2);
        if (t > 1)
            t = 1.0f;
        if (t < -1)
            t = -1.0f;
        float angle = acos(t);
        float dU = r2 * cos(angle);
        float dV = r2 * sin(angle);
        textureArray.push_back(dU / textureUSize);
        textureArray.push_back(dV / textureVSize);
    }

}

void Intersection::addRoad()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::addRoad()\n";
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
        road->setSelectedStatus(true);
        //QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(calculateRoadIntersections()));
        QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(resetWidth()));
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
        showBoardStatus.push_back(false);
        curve->setSelectedStatus(true);
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
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::deleteRoad()\n";
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
        road->setSelectedStatus(true);
        //QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(calculateRoadIntersections()));
        QObject::connect(road, SIGNAL(widthChanged(double)), this, SLOT(resetWidth()));
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
        showBoardStatus.push_back(false);
        curve->setSelectedStatus(true);
        curves.push_back(curve);
    }


    setRoadsTextures();

    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
    this->setSelectedStatus(true);
    emit roadAdded();
    emit intersectionsChanged(layout, render);

}

void Intersection::recalculateRoads()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::recalculateRoads()\n";
    calculateRoadIntersections();
    calculateRoundings();
    setRoadsTextures();
    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);

}

void Intersection::resetWidth()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::resetWidth()\n";
    RoadSimple* road = qobject_cast<RoadSimple*>(sender());
    int index = -1;
    for (int i = 0; i < roads.size(); ++i)
    {
        if (road == roads[i])
        {
            index = i;
            break;
        }
    }
    if (index < 0)
        return;

    vec3 p1 = roads[index]->getCoordOfPoint(1);
    vec3 p2 = roads[index]->getCoordOfPoint(2);
    int j = index == roads.size() - 1 ? 0 : index + 1;
    vec3 p3 = roads[j]->getCoordOfPoint(0);
    vec3 p4 = roads[j]->getCoordOfPoint(3);
    float leftLength = curves[index]->getLeftLength();
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float r1 = sqrt(dx1*dx1 + dy1*dy1);
    float dx2 = p4.x - p3.x;
    float dy2 = p4.y - p3.y;
    float r2 = sqrt(dx2*dx2 + dy2*dy2);
    float a1, b1, c1;
    float a2, b2, c2;
    float x, y;
    a1 = p1.y - p2.y;
    b1 = p2.x - p1.x;
    c1 = p1.x * p2.y - p2.x * p1.y;

    a2 = p3.y - p4.y;
    b2 = p4.x - p3.x;
    c2 = p3.x * p4.y - p4.x * p3.y;

    if (!calculateLinesIntersection(a1, b1, c1, a2, b2, c2, x, y))
        return;
    x += dx2 / r2 * (leftLength + 0.01f);
    y += dy2 / r2 * (leftLength + 0.01f);
    float res = (p4.x - p2.x) * (y - p2.y) - (x - p2.x) * (p4.y - p2.y);
    if (res < 0)
    {
        float dx = x - p4.x;
        float dy = y - p4.y;

        int count = roads[j]->getNumberOfControls() - 12;
        roads[j]->resizeByControl(count + 7, dx, dy, 0.0f, 0.0f);
    }



    p1 = roads[index]->getCoordOfPoint(0);
    p2 = roads[index]->getCoordOfPoint(3);
    j = index == 0 ? roads.size() - 1 : index - 1;
    p3 = roads[j]->getCoordOfPoint(1);
    p4 = roads[j]->getCoordOfPoint(2);
    float rightLength = curves[j]->getRightLength();
    dx1 = p2.x - p1.x;
    dy1 = p2.y - p1.y;
    r1 = sqrt(dx1*dx1 + dy1*dy1);
    dx2 = p4.x - p3.x;
    dy2 = p4.y - p3.y;
    r2 = sqrt(dx2*dx2 + dy2*dy2);
    a1 = p1.y - p2.y;
    b1 = p2.x - p1.x;
    c1 = p1.x * p2.y - p2.x * p1.y;

    a2 = p3.y - p4.y;
    b2 = p4.x - p3.x;
    c2 = p3.x * p4.y - p4.x * p3.y;

    if (!calculateLinesIntersection(a1, b1, c1, a2, b2, c2, x, y))
        return;
    x += dx2 / r2 * (rightLength + 0.01f);
    y += dy2 / r2 * (rightLength + 0.01f);
    res = (p4.x - p2.x) * (y - p2.y) - (x - p2.x) * (p4.y - p2.y);
    if (res > 0)
    {
        float dx = x - p4.x;
        float dy = y - p4.y;

        int count = roads[j]->getNumberOfControls() - 12;
        roads[j]->resizeByControl(count + 7, dx, dy, 0.0f, 0.0f);
    }
    recalculateRoads();
}

void Intersection::setAngle(double angle)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setAngle(double angle)"
                                       << " angle = " << angle << "\n";
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


    road->setVertexArray();
    road->setTextureArray();
    road->setTextureArrayBoard();

    recalculateRoads();

}

void Intersection::setAngle(double angle, int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setAngle(double angle, int index)"
                                       << " angle = " << angle
                                       << " index = " << index<< "\n";
}

void Intersection::deleteLine()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::deleteLine()\n";
    emit linesChanged(layout, render);
}

void Intersection::addLine()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::addLine()\n";
    emit linesChanged(layout, render);
}

bool Intersection::setFixed(bool fixed)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setFixed(bool fixed)"
                                       << " fixed = " << fixed << "\n";
    this->fixed = fixed;
}


void Intersection::clear()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::clear()\n";
}

void Intersection::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "Intersection::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}


void Intersection::clearProperties(QLayout *layout)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::clearProperties(QLayout *layout)\n";
    for (int i = 0; i < 10; ++i)
        disconnect(stepDialogs[i], 0, 0, 0);
    disconnect(stepDialog, 0, 0, 0);
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


void Intersection::setModel(Model *model)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setModel(Model *model)\n";
    this->model = model;
    for (int i = 0; i < roads.size(); ++i)
        roads[i]->setModel(model);
}


std::vector<vec3> Intersection::getCoordOfControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::getCoordOfControl(int index)"
                                       << " index = " << index << "\n";
    std::vector<vec3> res;

    int i;
    for (i = 0; i < roads.size(); ++i)
    {
        if (index >= (roads[i]->getNumberOfControls() + 1))
            index -= (roads[i]->getNumberOfControls() + 1);
        else
            break;
    }
    if (i >= roads.size())
    {
                for (i = 0; i < curves.size(); ++i)
                {
                    if (index >= (curves[i]->getNumberOfControls() + 1))
                        index -= (curves[i]->getNumberOfControls() + 1);
                    else
                    {
                        for (int j = 0; j < curves[i]->getCoordOfControl(index).size(); ++j)
                        {
                            vec3 p = curves[i]->getCoordOfControl(index)[j];
                            res.push_back(p);
                        }
                    }
                }
    }
    else
        if (index == roads[i]->getNumberOfControls())
        {
            vec3 p;
            p.x = roads[i]->getElementX();
            p.y = roads[i]->getElementY();
            p.z = 0.0f;
            res.push_back(p);
        }
        else
        {
            for (int j = 0; j < roads[i]->getCoordOfControl(index).size(); ++j)
            {
                vec3 p = roads[i]->getCoordOfControl(index)[j];
                res.push_back(p);
            }
        }
    return res;
}


RoadElement *Intersection::getCopy()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::getCopy()\n";
    Intersection* copyElement = new Intersection(*this);
    return copyElement;
}


void Intersection::setCoordForControl(int index, std::vector<vec3> &controls)
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::setCoordForControl(int index, std::vector<vec3> &controls)"
                                       << " index = " << index << "\n";
    int INDEX = index;
    int i;
    for (i = 0; i < roads.size(); ++i)
    {
        if (index >= (roads[i]->getNumberOfControls() + 1))
            index -= (roads[i]->getNumberOfControls() + 1);
        else
            break;
    }
    if (i >= roads.size())
    {
                for (i = 0; i < curves.size(); ++i)
                {
                    if (index >= (curves[i]->getNumberOfControls() + 1))
                        index -= (curves[i]->getNumberOfControls() + 1);
                    else
                    {

                        vec3 p = curves[i]->getCoordOfControl(index)[0];
                        float x, y;
                        x = p.x;
                        y = p.y;
                        resizeByControl(INDEX, controls[0].x - x, controls[0].y - y, x, y);

                    }
                }
    }
    else
        if (index == roads[i]->getNumberOfControls())
        {
            float x, y;
            x = roads[i]->getElementX();
            y = roads[i]->getElementY();
            resizeByControl(INDEX, controls[0].x - x, controls[0].y - y, x, y);
        }
        else
        {

            vec3 p = roads[i]->getCoordOfControl(index)[0];
            float x, y;
            x = p.x;
            y = p.y;
            resizeByControl(INDEX, controls[0].x - x, controls[0].y - y, x, y);

        }
    calculateRoadIntersections();
    calculateRoundings();
    setRoadsTextures();
    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
}


QJsonObject Intersection::getJSONInfo()
{
    if (log)
        Logger::getLogger()->infoLog() << "Intersection::getJSONInfo()\n";
    QJsonObject element;
    element["Name"] = name;
    element["Layer"] = layer;
    element["Id"] = Id;

    QJsonArray tempRoads;
    for (int i = 0; i < roads.size(); ++i)
    {
        tempRoads.append(roads[i]->getJSONInfo());
    }
    element["Roads"] = tempRoads;

    QJsonArray tempCurves;
    for (int i = 0; i < curves.size(); ++i)
    {
        tempCurves.append(curves[i]->getJSONInfo());
    }
    element["Curves"] = tempCurves;

    element["Fixed"] = fixed;

    return element;
}
