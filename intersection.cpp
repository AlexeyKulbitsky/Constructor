#include "intersection.h"
#include "assert.h"
#include <QApplication>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QToolBox>
#include <QTableWidget>
#include "mainwindow.h"

bool Intersection::log = true;

Intersection::Intersection()
{
    name = "Intersection";
    selected = false;
    fixed = false;
    layer = 0;
}

Intersection::Intersection(float x, float y, int numberOfRoads)
{

    name = "Intersection";
    indexOfSelectedControl = -1;
    layer = 0;
    float r = 5.0f * numberOfRoads;
    float pi = 3.14159265f;
    if (numberOfRoads < 3)
        numberOfRoads = 3;
    for (int i = 0; i < numberOfRoads; ++i)
    {
        float angle = 2.0f * pi / numberOfRoads * float(i);
        float dx = r * cosf(angle);
        float dy = r * sinf(angle);
        RoadSimple *road = new RoadSimple(x, y, x + dx, y + dy, 6.0f,
                                          "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                          "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                          "RoadSimple", 0,
                                          QString("Рукав №") + QString::number(i + 1));
        road->setLeftBoardShowStatus(false);
        road->setRightBoardShowStatus(false);
        connect(road, SIGNAL(widthChanged(double)), this, SLOT(resetWidth()));
        roads.push_back(road);

    }
    for (int i = 0; i < roads.size(); ++i)
    {
        vec2 p1 = roads[i]->getAxisPoint_1();
        vec2 p2 = roads[i]->getAxisPoint_2();
        int j = i == roads.size() - 1 ? 0 : i + 1;
        vec2 t1 = roads[j]->getAxisPoint_1();
        vec2 t2 = roads[j]->getAxisPoint_2();
        float angle = calculateAngle(p1, p2, t1, t2) * 180.0f / pi;
        angles.push_back(new Angle(angle));
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
                                 "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                 "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                 10);
        curve->setDescirption(QString("Закругление") + QString::number(i + 1));
        curve->setBoardShowStatus(true);
        showBoardStatus.push_back(true);
        curve->setSelectedStatus(false);
        curves.push_back(curve);
        connect(curve, SIGNAL(boardWidthChanged(double)), roads[i], SLOT(setLeftBoardWidth(double)));
        int j = i == roads.size() - 1 ? 0 : i + 1;
        connect(curve, SIGNAL(boardWidthChanged(double)), roads[j], SLOT(setRightBoardWidth(double)));
    }

    texture1USize = 6.0f;
    texture1VSize = 6.0f;
    texture2USize = 2.75f;
    texture2VSize = 6.0f;
    texture1 = "/models/city_roads/nr_07C.jpg";
    texture2 = "/models/city_roads/bksid_11.jpg";
    textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture1);
    textureID[1] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture2);

    setRoadsTextures();

    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
    selected = fixed = false;
    connect(this, SIGNAL(intersectionsChanged(QVBoxLayout*,QGLWidget*)),SLOT(getProperties(QVBoxLayout*,QGLWidget*)));
    connect(this, SIGNAL(linesChanged(QVBoxLayout*,QGLWidget*)),SLOT(getProperties(QVBoxLayout*,QGLWidget*)));

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
    for (int i = 0; i < curves.size(); ++i)
    {
        connect(curves[i], SIGNAL(boardWidthChanged(double)), roads[i], SLOT(setLeftBoardWidth(double)));
        int j = i == roads.size() - 1 ? 0 : i + 1;
        connect(curves[i], SIGNAL(boardWidthChanged(double)), roads[j], SLOT(setRightBoardWidth(double)));
    }
    for (int i = 0; i < source.angles.size(); ++i)
    {
        angles.push_back(new Angle(source.angles[i]->getAngle()));
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
    elementX = source.elementX;
    elementY = source.elementY;
    connect(this, SIGNAL(intersectionsChanged(QVBoxLayout*,QGLWidget*)),SLOT(getProperties(QVBoxLayout*,QGLWidget*)));
    connect(this, SIGNAL(linesChanged(QVBoxLayout*,QGLWidget*)),SLOT(getProperties(QVBoxLayout*,QGLWidget*)));

}

Intersection::Intersection(QVector<RoadSimple *> &roads, QVector<Curve *> &curves)
{
    name = "Intersection";
    indexOfSelectedControl = -1;
    layer = 0;
    float pi = 3.14159265f;
    for (int i = 0; i < roads.size(); ++i)
    {
        this->roads.push_back(roads[i]);
    }
    for (int i = 0; i < roads.size(); ++i)
    {
        vec2 p1 = roads[i]->getAxisPoint_1();
        vec2 p2 = roads[i]->getAxisPoint_2();
        int j = i == roads.size() - 1 ? 0 : i + 1;
        vec2 t1 = roads[j]->getAxisPoint_1();
        vec2 t2 = roads[j]->getAxisPoint_2();
        float angle = calculateAngle(p1, p2, t1, t2) * 180.0f / pi;
        angles.push_back(new Angle(angle));
    }
    calculateRoadIntersections();
    for (int i = 0; i < curves.size(); ++i)
    {
        this->curves.push_back(curves[i]);

    }
    for (int i = 0; i < curves.size(); ++i)
    {
        connect(curves[i], SIGNAL(boardWidthChanged(double)), roads[i], SLOT(setLeftBoardWidth(double)));
        int j = i == roads.size() - 1 ? 0 : i + 1;
        connect(curves[i], SIGNAL(boardWidthChanged(double)), roads[j], SLOT(setRightBoardWidth(double)));
    }
    texture1USize = 6.0f;
    texture1VSize = 6.0f;
    texture2USize = 2.75f;
    texture2VSize = 6.0f;
    texture1 = "/models/city_roads/nr_07C.jpg";
    texture2 = "/models/city_roads/bksid_11.jpg";
    textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture1);
    textureID[1] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture2);

    setRoadsTextures();

    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
    selected = fixed = false;
    connect(this, SIGNAL(intersectionsChanged(QVBoxLayout*,QGLWidget*)),SLOT(getProperties(QVBoxLayout*,QGLWidget*)));
    connect(this, SIGNAL(linesChanged(QVBoxLayout*,QGLWidget*)),SLOT(getProperties(QVBoxLayout*,QGLWidget*)));

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
    //        for (int i = 0; i < curves.size(); ++i)
    //            curves[i]->drawMeasurements(render);


    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0FFF);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    for (int i = 0; i < roads.size(); ++i)
    {
        vec2 p1 = roads[i]->axis_1();
        vec2 p2 = roads[i]->axis_2();
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p1.x, p1.y, 0.03f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p2.x, p2.y, 0.03f);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glBegin(GL_LINES);
    for (int i = 0; i < roads.size(); ++i)
    {
        vec2 p1 = roads[i]->getAxisPoint_1();
        vec2 p2 = roads[i]->getAxisPoint_2();
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(p1.x, p1.y, 0.03f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(p2.x, p2.y, 0.03f);
    }
    glEnd();

    for (int i = 0; i < roads.size(); ++i)
    {

        roads[i]->drawDescription(render, 1.0f, 0.0f, 0.0f);
        roads[i]->drawMeasurements(render);
    }
    for (int i = 0; i < curves.size(); ++i)
    {
        curves[i]->drawMeasurements(render);
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
    elementX += dx;
    elementY += dy;
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
    if (index == roads[i]->getNumberOfControls())
    {
        bool right = roads[i]->getShowRightBoardStatus();
        bool left = roads[i]->getShowLeftBoardStatus();
        roads[i]->setRightBoardShowStatus(false);
        roads[i]->setLeftBoardShowStatus(false);
        roads[i]->drawFigure();
        roads[i]->setRightBoardShowStatus(right);
        roads[i]->setLeftBoardShowStatus(left);
    }
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
    //int INDEX = index;
    int i;
    for (i = 0; i < roads.size(); ++i)
    {
        if (index >= (roads[i]->getNumberOfControls() + 1))
            index -= (roads[i]->getNumberOfControls() + 1);
        else
            break;
    }

    if (index != roads[i]->getNumberOfControls())
    {
        int count = roads[i]->getNumberOfControls() - 12;
        if (index == count + 5)
        {
            // Поворот

            vec3 point1 = roads[i]->getCoordOfPoint(0);
            vec3 point2 = roads[i]->getCoordOfPoint(3);
            vec3 ax1 = point2 - point1;

            float prevLength = sqrt((point1.x - point2.x)*(point1.x - point2.x) +
                                    (point1.y - point2.y)*(point1.y - point2.y));

            roads[i]->resizeByControl(index, dx, dy, x, y);

            calculateRoadIntersections();
            point1 = roads[i]->getCoordOfPoint(0);
            point2 = roads[i]->getCoordOfPoint(3);
            float curLength = sqrt((point1.x - point2.x)*(point1.x - point2.x) +
                                   (point1.y - point2.y)*(point1.y - point2.y));
            int j = i == 0 ? roads.size() - 1 : i - 1;
            if (curLength < curves[j]->getLeftLength())
            {
                float factor = prevLength / curLength - prevLength /  curves[j]->getLeftLength();
                vec3 ax2 = point2 - point1;
                float angle = calculateAngle(ax1, ax2);
                roads[i]->rotate(angle * factor,
                                 roads[i]->getAxisPoint_1().x,
                                 roads[i]->getAxisPoint_1().y,
                                 0.0f);
            }

            float pi = 3.14159265f;
            vec2 p1 = roads[i]->getAxisPoint_1();
            vec2 p2 = roads[i]->getAxisPoint_2();
            j = i == roads.size() - 1 ? 0 : i + 1;
            vec2 t1 = roads[j]->getAxisPoint_1();
            vec2 t2 = roads[j]->getAxisPoint_2();
            float angle = calculateAngle(p1, p2, t1, t2) * 180.0f / pi;
            float delta = angle - angles[i]->getAngle();
            angles[i]->setAngle(angle);
            j = i == 0 ? roads.size() - 1 : i - 1;
            angles[j]->setAngle(angles[j]->getAngle() - delta);


            // Приведение длины следующего рукава к прежней длине
            float a1, a2, b1, b2, c1, c2;
            vec2 axis1 = roads[i]->axis_1();
            vec2 axis2 = roads[i]->axis_2();
            a1 = axis2.y - axis1.y;
            b1 = axis1.x - axis2.x;
            c1 = axis1.y * (axis2.x - axis1.x) - axis1.x * (axis2.y - axis1.y);

            j = i == 0 ? roads.size() - 1 : i - 1;
            vec2 axis3 = roads[j]->axis_1();
            vec2 axis4 = roads[j]->axis_2();

            a2 = axis4.y - axis3.y;
            b2 = axis3.x - axis4.x;
            c2 = axis3.y * (axis4.x - axis3.x) - axis3.x * (axis4.y - axis3.y);
            float xTemp, yTemp;
            if (calculateLinesIntersection(a1, b1, c1, a2, b2, c2, xTemp, yTemp))
            {

            }
            else
            {
                QMessageBox::critical(0, "Ошибка", "Невозможно найти пересечение осевых линий рукавов для изменения длины соседнего рукава (при повороте).");
                return;
            }
            delta = sqrt((axis3.x - axis4.x)*(axis3.x - axis4.x) + (axis3.y - axis4.y)*(axis3.y - axis4.y));
            roads[j]->setVertexArray(xTemp, yTemp,
                                     xTemp + (axis4.x - axis3.x) * roads[j]->getLength() / delta,
                                     yTemp + (axis4.y - axis3.y) * roads[j]->getLength() / delta,
                                     roads[j]->getWidth());

        }else
            if (index == count + 7)
            {
                // Длина
                float length = roads[i]->getLength();
                float dxL = roads[i]->getCoordOfPoint(2).x - roads[i]->getCoordOfPoint(1).x;
                float dyL = roads[i]->getCoordOfPoint(2).y - roads[i]->getCoordOfPoint(1).y;
                float dxR = roads[i]->getCoordOfPoint(3).x - roads[i]->getCoordOfPoint(0).x;
                float dyR = roads[i]->getCoordOfPoint(3).y - roads[i]->getCoordOfPoint(0).y;
                float leftLength = sqrt(dxL*dxL + dyL*dyL);
                float rightLength = sqrt(dxR*dxR + dyR*dyR);

                roads[i]->resizeByControl(index, dx, dy, x, y);

                float dx1 = roads[i]->getCoordOfPoint(2).x - roads[i]->getCoordOfPoint(1).x;
                float dy1 = roads[i]->getCoordOfPoint(2).y - roads[i]->getCoordOfPoint(1).y;
                float r = sqrt(dx1 * dx1 + dy1 * dy1);
                float l = curves[i]->getRightLength();
                if (l > (r - length + leftLength))
                {
                    roads[i]->resizeByControl(index, dx1 * (l - r + length - leftLength) / r, dy1 * (l - r + length - leftLength) / r, x + dx, y + dy);
                }
                dx1 = roads[i]->getCoordOfPoint(3).x - roads[i]->getCoordOfPoint(0).x;
                dy1 = roads[i]->getCoordOfPoint(3).y - roads[i]->getCoordOfPoint(0).y;
                r = sqrt(dx1 * dx1 + dy1 * dy1);
                int j = i == 0 ? roads.size() - 1 : i - 1;
                l = curves[j]->getLeftLength();
                if (l > r - length + rightLength)
                {
                    roads[i]->resizeByControl(index, dx1 * (l - r + length - rightLength) / r, dy1 * (l - r + length - rightLength) / r, x + dx, y + dy);
                }

            }else
                if (index == count + 8)
                {
                    // Ширина правая
                    roads[i]->resizeByControl(index, dx, dy, x, y);
                    //roads[i]->resizeByControlSelf(index, dx, dy, x, y);
                    int j = i == 0 ? roads.size() - 1 : i - 1;
                    vec3 p1 = roads[j]->getCoordOfPoint(1);
                    vec3 p2 = roads[j]->getCoordOfPoint(2);
                    float dx1 = p2.x - p1.x;
                    float dy1 = p2.y - p1.y;
                    vec3 s1 = roads[j]->getCoordOfPoint(0);
                    vec3 s2 = roads[j]->getCoordOfPoint(3);
                    float dx2 = s2.x - s1.x;
                    float dy2 = s2.y - s1.y;
                    float res = dx1*dx2 + dy1*dy2;
                    if (res < 0)
                    {
                        float s = sqrt(dx1*dx1 + dy1*dy1);
                        float l = curves[j]->getRightLength();
                        roads[i]->resizeByControl(index,
                                                 dx1 * (l + s) / s,
                                                 dy1 * (l + s) / s,
                                                  x + dx, y + dy);
                    }
                    else
                    {
                        float s = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
                        float l = curves[j]->getRightLength();
                        if (l > s)
                        {
                            roads[i]->resizeByControl(index,
                                                     (p1.x - p2.x) * (l - s) / s,
                                                     (p1.y - p2.y) * (l - s) / s,
                                                      x + dx, y + dy);
                        }
                    }


                }else
                    if (index == count + 9)
                    {
                        // Ширина левая
                        roads[i]->resizeByControl(index, dx, dy, x, y);
                        //roads[i]->resizeByControlSelf(index, dx, dy, x, y);

                        int j = i == roads.size() - 1 ? 0 : i + 1;
                        vec3 p1 = roads[j]->getCoordOfPoint(0);
                        vec3 p2 = roads[j]->getCoordOfPoint(3);
                        float dx1 = p2.x - p1.x;
                        float dy1 = p2.y - p1.y;
                        vec3 s1 = roads[j]->getCoordOfPoint(1);
                        vec3 s2 = roads[j]->getCoordOfPoint(2);
                        float dx2 = s2.x - s1.x;
                        float dy2 = s2.y - s1.y;
                        float res = dx1*dx2 + dy1*dy2;
                        if (res < 0)
                        {
                            float s = sqrt(dx1*dx1 + dy1*dy1);
                            float l = curves[j]->getRightLength();
                            roads[i]->resizeByControl(index,
                                                     dx1 * (l + s) / s,
                                                     dy1 * (l + s) / s,
                                                      x + dx, y + dy);
                        }
                        else
                        {
                            float s = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
                            float l = curves[i]->getLeftLength();
                            if (l > s)
                            {
                                roads[i]->resizeByControl(index,
                                                         (p1.x - p2.x) * (l - s) / s,
                                                         (p1.y - p2.y) * (l - s) / s,
                                                          x + dx, y + dy);
                            }
                        }
                    }else
                        if (index == count + 10)
                        {
                            // Ширина правого тротуара
                            roads[i]->resizeByControl(index, dx, dy, x, y);
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
                        }else
                            if (index == count + 11)
                            {
                                // Ширина левого тротуара
                                roads[i]->resizeByControl(index, dx, dy, x, y);
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
        int m = i == 0 ? roads.size() - 1 : i - 1;
        //roads[m]->move(dx, dy);
        // Проверка на выход за границы справа
        vec3 k1 = roads[i]->getCoordOfPoint(0);
        vec3 k2 = roads[i]->getCoordOfPoint(3);

        float a1, a2, b1, b2, c1, c2;

        a1 = k2.y - k1.y;
        b1 = k1.x - k2.x;
        c1 = k1.y * (k2.x - k1.x) - k1.x * (k2.y - k1.y);
        int j = i == 0 ? roads.size() - 1 : i - 1;
        vec3 t1 = roads[j]->getCoordOfPoint(1);
        vec3 t2 = roads[j]->getCoordOfPoint(2);

        a2 = t2.y - t1.y;
        b2 = t1.x - t2.x;
        c2 = t1.y * (t2.x - t1.x) - t1.x * (t2.y - t1.y);
        float xTemp;
        float yTemp;

        if (calculateLinesIntersection(a1, b1, c1, a2, b2, c2, xTemp, yTemp))
        {

        }
        else
        {
            QMessageBox::critical(0, "Ошибка", "Невозможно найти пересечение границ рукавов перекрестка для смещения вправо.");
            return;
        }
        j = i == 0 ? roads.size() - 1 : i - 1;
        //vec3 p1 = roads[j]->getCoordOfPoint(1);
        vec3 p1(xTemp, yTemp, 0.0f);
        vec3 p2 = roads[j]->getCoordOfPoint(2);
        float dx1 = p2.x - p1.x;
        float dy1 = p2.y - p1.y;
        vec3 s1 = roads[j]->getCoordOfPoint(0);
        vec3 s2 = roads[j]->getCoordOfPoint(3);
        float dx2 = s2.x - s1.x;
        float dy2 = s2.y - s1.y;
        float res = dx1*dx2 + dy1*dy2;
        if (res < 0)
        {
            float s = sqrt(dx1*dx1 + dy1*dy1);
            float l = curves[j]->getRightLength();
            roads[i]->move(dx1 * (l + s) / s, dy1 * (l + s) / s);
            //roads[m]->move(dx1 * (l + s) / s, dy1 * (l + s) / s);
        }
        else
        {
            float s = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
            float l = curves[j]->getRightLength();
            if (l > s)
            {
                roads[i]->move((p1.x - p2.x) * (l - s) / s, (p1.y - p2.y) * (l - s) / s);
                //roads[m]->move((p1.x - p2.x) * (l - s) / s, (p1.y - p2.y) * (l - s) / s);
            }
        }

        // Проверка на выход за границы слева

        k1 = roads[i]->getCoordOfPoint(1);
        k2 = roads[i]->getCoordOfPoint(2);


        a1 = k2.y - k1.y;
        b1 = k1.x - k2.x;
        c1 = k1.y * (k2.x - k1.x) - k1.x * (k2.y - k1.y);
        j = i == roads.size() - 1 ? 0 : i + 1;
        t1 = roads[j]->getCoordOfPoint(0);
        t2 = roads[j]->getCoordOfPoint(3);

        a2 = t2.y - t1.y;
        b2 = t1.x - t2.x;
        c2 = t1.y * (t2.x - t1.x) - t1.x * (t2.y - t1.y);

        if (calculateLinesIntersection(a1, b1, c1, a2, b2, c2, xTemp, yTemp))
        {

        }
        else
        {
            QMessageBox::critical(0, "Ошибка", "Невозможно найти пересечение границ рукавов перекрестка для смещения влево.");
            return;
        }

        j = i == roads.size() - 1 ? 0 : i + 1;
        //p1 = roads[j]->getCoordOfPoint(0);
        p1 = vec3(xTemp, yTemp, 0.0f);
        p2 = roads[j]->getCoordOfPoint(3);
        dx1 = p2.x - p1.x;
        dy1 = p2.y - p1.y;
        s1 = roads[j]->getCoordOfPoint(1);
        s2 = roads[j]->getCoordOfPoint(2);
        dx2 = s2.x - s1.x;
        dy2 = s2.y - s1.y;
        res = dx1*dx2 + dy1*dy2;
        if (res < 0)
        {
            float s = sqrt(dx1*dx1 + dy1*dy1);
            float l = curves[j]->getRightLength();
            roads[i]->move(dx1 * (l + s) / s, dy1 * (l + s) / s);
            //roads[m]->move(dx1 * (l + s) / s, dy1 * (l + s) / s);
        }
        else
        {
            float s = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
            float l = curves[i]->getLeftLength();
            if (l > s)
            {
                roads[i]->move((p1.x - p2.x) * (l - s) / s, (p1.y - p2.y) * (l - s) / s);
                //roads[m]->move((p1.x - p2.x) * (l - s) / s, (p1.y - p2.y) * (l - s) / s);
            }
        }

        // Приведение длины рукава к прежней длине
        vec2 axis1 = roads[i]->axis_1();
        vec2 axis2 = roads[i]->axis_2();
        a1 = axis2.y - axis1.y;
        b1 = axis1.x - axis2.x;
        c1 = axis1.y * (axis2.x - axis1.x) - axis1.x * (axis2.y - axis1.y);

        j = i == roads.size() - 1 ? 0 : i + 1;
        vec2 axis3 = roads[j]->axis_1();
        vec2 axis4 = roads[j]->axis_2();

        a2 = axis4.y - axis3.y;
        b2 = axis3.x - axis4.x;
        c2 = axis3.y * (axis4.x - axis3.x) - axis3.x * (axis4.y - axis3.y);
        if (calculateLinesIntersection(a1, b1, c1, a2, b2, c2, xTemp, yTemp))
        {

        }
        else
        {
            QMessageBox::critical(0, "Ошибка", "Невозможно найти пересечение осевых линий рукавов.");
            return;
        }
        float delta = sqrt((axis1.x - axis2.x)*(axis1.x - axis2.x) + (axis1.y - axis2.y)*(axis1.y - axis2.y));
        roads[i]->setVertexArray(xTemp, yTemp,
                                 xTemp + (axis2.x - axis1.x) * roads[i]->getLength() / delta,
                                 yTemp + (axis2.y - axis1.y) * roads[i]->getLength() / delta,
                                 roads[i]->getWidth());


        // Приведение длины следующего рукава к прежней длине
        j = i == 0 ? roads.size() - 1 : i - 1;
        axis3 = roads[j]->axis_1();
        axis4 = roads[j]->axis_2();

        a2 = axis4.y - axis3.y;
        b2 = axis3.x - axis4.x;
        c2 = axis3.y * (axis4.x - axis3.x) - axis3.x * (axis4.y - axis3.y);
        if (calculateLinesIntersection(a1, b1, c1, a2, b2, c2, xTemp, yTemp))
        {

        }
        else
        {
            QMessageBox::critical(0, "Ошибка", "Невозможно найти пересечение осевых линий рукавов для изменения длины соседнего рукава.");
            return;
        }
        delta = sqrt((axis3.x - axis4.x)*(axis3.x - axis4.x) + (axis3.y - axis4.y)*(axis3.y - axis4.y));
        roads[j]->setVertexArray(xTemp, yTemp,
                                 xTemp + (axis4.x - axis3.x) * roads[j]->getLength() / delta,
                                 yTemp + (axis4.y - axis3.y) * roads[j]->getLength() / delta,
                                 roads[j]->getWidth());

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

void Intersection::getProperties(QVBoxLayout *layout, QGLWidget *render)
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

    QFormLayout *l = new QFormLayout();

    QCheckBox *fixedCheckBox = new QCheckBox();
    fixedCheckBox->setChecked(fixed);
    QObject::connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));
    l->addRow("Зафиксировать", fixedCheckBox);

    QCheckBox *showMeasurementsCheckBox = new QCheckBox();
    showMeasurementsCheckBox->setChecked(showMeasurements);
    connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowMeasurements(bool)));
    connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
    l->addRow("Размеры", showMeasurementsCheckBox);

    layout->addLayout(l);

    for (int i = 0; i < roads.size(); ++i)
    {

        QWidget *widget = new QWidget(layout->parentWidget());
        QFormLayout *layer = new QFormLayout(widget);
        //widget->setLayout(layer);
        QListWidget *list = new QListWidget(layout->parentWidget());

//        list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//        list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//        list->setFixedSize(list->sizeHintForColumn(0) + 2 * list->frameWidth(),
//                          list->sizeHintForRow(0) * list->count() + 2 * list->frameWidth());
//        list->setMinimumHeight(list->sizeHintForColumn(0));
        QSizePolicy sp = list->sizePolicy();
        sp.setVerticalPolicy(QSizePolicy::Preferred);
        list->setSizePolicy(sp);

        QPushButton *showButton = new QPushButton("Рукав " + QString::number(i + 1));
        showButton->setCheckable(true);
        connect(showButton, SIGNAL(toggled(bool)), widget, SLOT(setVisible(bool)));
        connect(showButton, SIGNAL(toggled(bool)), list, SLOT(setVisible(bool)));
        showButton->toggle();

        layout->addWidget(showButton);
        layout->addWidget(widget);
        layout->addWidget(list);

        QDoubleSpinBox *widthDoubleSpinBox = new QDoubleSpinBox();
        widthDoubleSpinBox->setKeyboardTracking(false);
        widthDoubleSpinBox->setValue(roads[i]->getWidth());
        widthDoubleSpinBox->setMinimum(0.1);
        QObject::connect(roads[i], SIGNAL(widthChanged(double)), widthDoubleSpinBox, SLOT(setValue(double)));
        QObject::connect(widthDoubleSpinBox, SIGNAL(valueChanged(double)), roads[i], SLOT(setWidth(double)));
        if (render)
        {
            //QObject::connect(widthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        }
        layer->addRow("Ширина", widthDoubleSpinBox);
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
            connect(this, SIGNAL(linesChanged(QVBoxLayout*,QGLWidget*)), render, SLOT(updateGL()));
        }


        for (int j = 0; j < roads[i]->lines.size(); ++j)
        {
//            QPushButton* b = new QPushButton(QString::number(j + 1));
//            connect(b, SIGNAL(clicked(bool)), roads[i], SLOT(deleteLine()));
//            connect(b, SIGNAL(clicked(bool)), this, SLOT(deleteLine()));
//            layer->addRow("Удалить линию ",b);

            QPushButton *editButton = new QPushButton();
            editButton->setContentsMargins(0, 0, 0, 0);
            editButton->setText("Редактировать");
            QListWidgetItem *item = new QListWidgetItem();
            //item->setSizeHint(QSize(0,20));
            list->addItem(item);

            QWidget *itemWidget = new QWidget(list);
            QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
            itemLayout->setMargin(0);
            itemLayout->setContentsMargins(5, 0, 5, 0);
            itemWidget->setLayout(itemLayout);
            itemLayout->addWidget(new QLabel(QString("Линия №") + QString::number(j + 1)));
            itemLayout->addWidget(editButton);
            list->setItemWidget(item, itemWidget);
        }
        connect(roads[i], SIGNAL(lineDeleted()), this, SLOT(deleteLine()));
        layer->addRow("Добавить разметку", addLinePushButton);



    }



    for (int i = 0; i < curves.size(); ++i)
    {
        QWidget *widget = new QWidget(layout->parentWidget());
        QFormLayout *layer = new QFormLayout(widget);
        //widget->setLayout(layer);

        QPushButton *showButton = new QPushButton("Закругление " + QString::number(i + 1));
        showButton->setCheckable(true);
        connect(showButton, SIGNAL(toggled(bool)), widget, SLOT(setVisible(bool)));
        showButton->toggle();

        layout->addWidget(showButton);
        layout->addWidget(widget);

        QDoubleSpinBox *leftLengthDoubleSpinBox = new QDoubleSpinBox();
        leftLengthDoubleSpinBox->setKeyboardTracking(false);
        leftLengthDoubleSpinBox->setValue(curves[i]->getLeftLength());
        leftLengthDoubleSpinBox->setMinimum(0.0);
        connect(curves[i], SIGNAL(leftLengthChanged(double)), leftLengthDoubleSpinBox, SLOT(setValue(double)));
        connect(leftLengthDoubleSpinBox, SIGNAL(valueChanged(double)), curves[i], SLOT(setLeftLength(double)));
        connect(leftLengthDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRoadsTextures()));

        QDoubleSpinBox *rightLengthDoubleSpinBox = new QDoubleSpinBox();
        rightLengthDoubleSpinBox->setKeyboardTracking(false);
        rightLengthDoubleSpinBox->setValue(curves[i]->getRightLength());
        rightLengthDoubleSpinBox->setMinimum(0.0);
        connect(curves[i], SIGNAL(rightLengthChanged(double)), rightLengthDoubleSpinBox, SLOT(setValue(double)));
        connect(rightLengthDoubleSpinBox, SIGNAL(valueChanged(double)), curves[i], SLOT(setRightLength(double)));
        connect(rightLengthDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRoadsTextures()));

        QCheckBox *showBoardCheckBox = new QCheckBox();
        showBoardCheckBox->setChecked(curves[i]->getBoardShowStatus());
        connect(showBoardCheckBox, SIGNAL(toggled(bool)), curves[i], SLOT(setBoardShowStatus(bool)));
        connect(showBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));

        QDoubleSpinBox *angleDoubleSpinBox = new QDoubleSpinBox();
        angleDoubleSpinBox->setKeyboardTracking(false);
        angleDoubleSpinBox->setObjectName(QString::number(i));
        angleDoubleSpinBox->setMinimum(-180.0);
        angleDoubleSpinBox->setMaximum(720.0);
        angleDoubleSpinBox->setValue(angles[i]->getAngle());
        connect(angles[i], SIGNAL(angleChanged(double)), angleDoubleSpinBox, SLOT(setValue(double)));
        connect(angleDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngle(double)));

        QDoubleSpinBox *boardWidthDoubleSpinBox = new QDoubleSpinBox();
        boardWidthDoubleSpinBox->setKeyboardTracking(false);
        boardWidthDoubleSpinBox->setValue(curves[i]->getBoardWidth());
        boardWidthDoubleSpinBox->setMinimum(0.0);
        connect(boardWidthDoubleSpinBox, SIGNAL(valueChanged(double)), curves[i], SLOT(setBoardWidth(double)));
        connect(curves[i], SIGNAL(boardWidthChanged(double)), boardWidthDoubleSpinBox, SLOT(setValue(double)));
        connect(boardWidthDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(recalculateRoads()));

        if (render)
        {
            connect(rightLengthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
            connect(leftLengthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
            connect(angleDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
            connect(boardWidthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        }
        layer->addRow("Отображать", showBoardCheckBox);
        layer->addRow("Левая сторона", leftLengthDoubleSpinBox);
        layer->addRow("Правая сторона", rightLengthDoubleSpinBox);
        layer->addRow("Угол", angleDoubleSpinBox);
        layer->addRow("Ширина тротуара", boardWidthDoubleSpinBox);

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

        // Если не нашли точку пересечения, то выходим из цикла и продолжаем работу
        if (!calculateLinesIntersection(a1, b1, c1,
                                        a2, b2, c2,
                                        xTemp, yTemp))
            break;
        roads[i]->setCoordForPoint(1, xTemp, yTemp, 0.0f);
        roads[j]->setCoordForPoint(0, xTemp, yTemp, 0.0f);

    }
    for (int i = 0; i < roads.size(); ++i)
    {
        roads[i]->setTextureArray();
    }

    return true;
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
        float x1 = p0.x, y1 = p0.y, z1 = p0.z;
        float x2 = p0.x + dx, y2 = p0.y + dy, z2 = p0.z;
        //curves[i]->setCoordForPoint(0, p0.x, p0.y, p0.z);
        //curves[i]->setCoordForPoint(1, p0.x + dx, p0.y + dy, p0.z);

        dx = rightLength * (p1.x - p0.x) / rightR;
        dy = rightLength * (p1.y - p0.y) / rightR;
        float x3 = p0.x + dx, y3 = p0.y + dy, z3 = p0.z;
        curves[i]->setCoordsForControls(x1, y1, z1,
                                        x2, y2, z2,
                                        x3, y3, z3);
        //curves[i]->setCoordForPoint(2, p0.x + dx, p0.y + dy, p0.z);
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
    float sumX = 0.0f, sumY = 0.0f;
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        sumX += vertexArray[i * 3];
        sumY += vertexArray[i * 3 + 1];
    }
    elementX = sumX / float(vertexArray.size() / 3);
    elementY = sumY / float(vertexArray.size() / 3);
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
    qDebug() << "ResetWidth";
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
    qDebug() << "Index" << index;
    if (index < 0)
        return;

    int j = index == 0 ? roads.size() - 1 : index - 1;
    if (fabs(angles[j]->getAngle() - 180.0f) < 0.001f)
    {
        qDebug() << "Right";
        roads[j]->setLeftWidth(roads[index]->getRightWidth());
    }
    j = index == roads.size() - 1 ? 0 : index + 1;
    if (fabs(angles[index]->getAngle() - 180.0f) < 0.001f)
    {
        qDebug() << "Left";
        roads[j]->setRightWidth(roads[index]->getLeftWidth());
    }
    calculateRoadIntersections();
    calculateRoundings();
    setRoadsTextures();
    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
}

void Intersection::setAngle(double angle)
{
    float pi = 3.14159265f;
    if (angle == 180.0)
        angle = 180.01;
    int i = qobject_cast<QDoubleSpinBox*>(sender())->objectName().toInt();
    float oldAngle = angles[i]->getAngle() * pi / 180.0f;
    float newAngle = angle * pi / 180.0f;
    float delta = newAngle - oldAngle;
    int j = i == roads.size() - 1 ? 0 : i + 1;
    vec2 p = roads[j]->getAxisPoint_1();
    roads[j]->rotate(delta, p.x, p.y, 0.0f);

    angles[j]->setAngle(angles[j]->getAngle() - (angle - angles[i]->getAngle()));
    angles[i]->setAngle(angle);

    calculateRoadIntersections();
    calculateRoundings();
    setRoadsTextures();
    setVertexArray();
    setTextureArray(texture1USize, texture1VSize);
    setIndexArray();
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
    return true;
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
        if (item->layout() != NULL)
        {
            clearProperties(item->layout());
            delete item->layout();
        }
        if (item->widget() != NULL)
        {
            delete item->widget();
        }
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

    if (index == roads[i]->getNumberOfControls())
    {
        vec3 p;
        p.x = roads[i]->getAxisPoint_1().x;
        p.y = roads[i]->getAxisPoint_1().y;
        p.z = 0.0f;
        vec3 s;
        s.x = roads[i]->getAxisPoint_2().x;
        s.y = roads[i]->getAxisPoint_2().y;
        s.z = 0.0f;
        vec3 s1;
        s1.x = curves[i]->getLeftLength();
        s1.y = curves[i]->getRightLength();
        vec3 s2;
        int j = i == 0 ? roads.size() - 1 : i - 1;
        s2.x = curves[j]->getLeftLength();
        s2.y = curves[j]->getRightLength();
        res.push_back(p);
        res.push_back(s);
        res.push_back(s1);
        res.push_back(s2);
        return res;
    }
    else
    {
        return roads[i]->getCoordOfControl(index);
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
    if (index == roads[i]->getNumberOfControls())
    {
        vec2 p1;
        p1.x = controls[0].x;
        p1.y = controls[0].y;
        vec2 p2;
        p2.x = controls[1].x;
        p2.y = controls[1].y;
        roads[i]->setCoordForAxisPoint(0, p1.x, p1.y);
        roads[i]->setCoordForAxisPoint(1, p2.x, p2.y);
        float pi = 3.14159265f;
        for (int i = 0; i < roads.size(); ++i)
        {
            vec2 p1 = roads[i]->getAxisPoint_1();
            vec2 p2 = roads[i]->getAxisPoint_2();
            int j = i == roads.size() - 1 ? 0 : i + 1;
            vec2 t1 = roads[j]->getAxisPoint_1();
            vec2 t2 = roads[j]->getAxisPoint_2();
            float angle = calculateAngle(p1, p2, t1, t2) * 180.0f / pi;
            angles[i]->setAngle(angle);
        }

        float leftLength = controls[2].x;
        float rightLength = controls[2].y;
        curves[i]->setLeft(leftLength);
        curves[i]->setRight(rightLength);
        leftLength = controls[3].x;
        rightLength = controls[3].y;
        int j = i == 0 ? roads.size() - 1 : i - 1;
        curves[j]->setLeft(leftLength);
        curves[j]->setRight(rightLength);
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
