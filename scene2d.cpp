#include <QtGui>
#include <math.h>
#include "scene2D.h"
#include <iostream>


GLint viewport[4]; // декларируем матрицу поля просмотра

Scene2D::Scene2D(QWidget* parent) : QGLWidget(parent)
{
    showGrid = true;
    this->setFocusPolicy(Qt::StrongFocus);
    nSca = 0.2f;
    xRot=0; yRot=0; zRot=0; zTra=0;
    xDelta = 0.0f;
    yDelta = 0.0f;
    dX = 0.0f;
    dY = 0.0f;
    figureIsSelected = false;
    controlIsSelected = false;
    drawRectStatus = false;
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    setAcceptDrops(true);
    model = NULL;
    stateManager = NULL;
}



Scene2D::~Scene2D()
{
    stateManager = NULL;
    model = NULL;
    properties = NULL;
}

void Scene2D::initializeGL()
{
    Logger::getLogger()->writeLog("Scene2D::initializeGL");
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_LIGHT0);

    //int	maxTextureUnits;

    //glGetIntegerv ( GL_MAX_TEXTURE_UNITS_ARB, &maxTextureUnits );
    ////qDebug() << isExtensionSupported ( "GL_ARB_multitexture" )
    ////qDebug() << maxTextureUnits;
}

void Scene2D::resizeGL(int nWidth, int nHeight)
{
   Logger::getLogger()->writeLog("Scene2D::resizeGL(" + QString::number(nWidth) +
                                  ", " + QString::number(nHeight) + ")");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ratio=(GLfloat)nHeight/(GLfloat)nWidth;

    if (nWidth>=nHeight)
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 1.0);
    else
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);



    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    glGetIntegerv(GL_VIEWPORT, viewport); // извлечь матрицу поля просмотра в viewport


}

void Scene2D::paintGL()
{
    Logger::getLogger()->writeLog("Scene2D::paintGL()");
    glClearColor(0.9, 0.9, 0.9, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения фонового света ( НОВОЕ )
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения диффузного света ( НОВОЕ )

    GLfloat LightPosition[]= { 0.0f, 0.0f, 200.0f, 0.0f };     // Позиция света ( НОВОЕ )
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // Установка Фонового Света
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glEnable(GL_LIGHT1);
    glScalef(nSca, nSca, nSca);



    gluLookAt(xDelta,yDelta,0.5,
              xDelta,yDelta,-10,
              0,1,0);
    drawGrid();
    drawAxis();
      // Установка Диффузного Света

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT1);


    if (model)
    {
    for (int i =  0; i < model->getNumberOfGroups(); ++i)
    {
        if (model->isGroupVisible(i) == true)
        {
        for(QList<RoadElement*>::iterator it = model->getGroup(i).begin();
            it != model->getGroup(i).end(); ++it)
        {
            (*it)->drawFigure(this);
        }
        }

    }
    }
    ////////////////////////////
    /*
    for (int i =  0; i < model->getNumberOfGroups(); ++i)
    {
        if (model->isGroupVisible(i) == true)
        {
        for(std::list<RoadElement*>::iterator it = model->getGroup(i).begin();
            it != model->getGroup(i).end(); ++it)
        {
            if ((*it)->isSelected() == true)
            {
                glDisable(GL_DEPTH_TEST);
                (*it)->drawMeasurements(this);
                glEnable(GL_DEPTH_TEST);
            }
        }
        }

    }
    /////////////////////////////
        int count = model->getNumberOfGroups();
        if (model->isGroupVisible(count - 1) == true)
        {
        for(std::list<RoadElement*>::iterator it = model->getGroup(count - 1).begin();
            it != model->getGroup(count - 1).end(); ++it)
        {
            if ((*it)->getName() == "Ruler")
                (*it)->drawMeasurements(this);
        }
        }

    }
    //shrift = QFont("Times", 15, QFont::Black);
    // renderText (ptrMousePosition.x(), ptrMousePosition.y(), "HELLO", shrift);

    //glDisable(GL_LIGHT1);
    //glDisable(GL_LIGHTING);
    */
    if (drawRectStatus == true)
        drawRect(rectPoint1, rectPoint2);

    glPopMatrix();
}

void Scene2D::mousePressEvent(QMouseEvent* pe)
{
    Logger::getLogger()->writeLog("Scene2D::mousePressEvent(QMouseEvent* pe)");
    this->setFocus();
    if (stateManager)
        stateManager->mousePressEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::mousePressEvent(QMouseEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::stateManager = NULL, Scene2D::mousePressEvent(QMouseEvent* pe) stopped");
    }
}

void Scene2D::mouseReleaseEvent(QMouseEvent* pe)
{
    Logger::getLogger()->writeLog("Scene2D::mouseReleaseEvent(QMouseEvent* pe)");
    if (stateManager)
        stateManager->mouseReleaseEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::mouseReleaseEvent(QMouseEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::stateManager = NULL, Scene2D::mouseReleaseEvent(QMouseEvent* pe) stopped");
    }
}

void Scene2D::mouseMoveEvent(QMouseEvent* pe)
{

    Logger::getLogger()->writeLog("Scene2D::mouseMoveEvent(QMouseEvent* pe)");
    this->setFocus();
    if (stateManager)
        stateManager->mouseMoveEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::mouseMoveEvent(QMouseEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::stateManager = NULL, Scene2D::mouseMoveEvent(QMouseEvent* pe) stopped");
    }
}

void Scene2D::wheelEvent(QWheelEvent* pe)
{
    Logger::getLogger()->writeLog("Scene2D::wheelEvent(QWheelEvent* pe)");
    if (stateManager)
        stateManager->wheelEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::wheelEvent(QWheelEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::stateManager = NULL, Scene2D::wheelEvent(QWheelEvent* pe) stopped");
    }
}

void Scene2D::keyPressEvent(QKeyEvent* pe)
{
    Logger::getLogger()->writeLog("Scene2D::keyPressEvent(QKeyEvent* pe)");
    if (stateManager)
        stateManager->keyPressEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::keyPressEvent(QKeyEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::stateManager = NULL, Scene2D::keyPressEvent(QKeyEvent* pe) stopped");
    }
}

void Scene2D::scalePlus()
{
    Logger::getLogger()->writeLog("Scene2D::scalePlus()");
    nSca = nSca*1.1;
}

void Scene2D::scaleMinus()
{
    Logger::getLogger()->writeLog("Scene2D::scaleMinus()");
    nSca = nSca/1.1;
}

void Scene2D::rotateUp()
{
    Logger::getLogger()->writeLog("Scene2D::rotateUp()");
    xRot += 1.0;
}

void Scene2D::rotateDown()
{
    Logger::getLogger()->writeLog("Scene2D::rotateDown()");
    xRot -= 1.0;
}

void Scene2D::rotateLeft()
{
    Logger::getLogger()->writeLog("Scene2D::rotateLeft()");
    zRot += 1.0;
}

void Scene2D::rotateRight()
{
    Logger::getLogger()->writeLog("Scene2D::rotateRight()");
    zRot -= 1.0;
}

void Scene2D::translateDown()
{
    Logger::getLogger()->writeLog("Scene2D::translateDown()");
    zTra -= 0.05;
}

void Scene2D::translateUp()
{
    Logger::getLogger()->writeLog("Scene2D::translateUp()");
    zTra += 0.05;
}

void Scene2D::defaultScene()
{
    Logger::getLogger()->writeLog("Scene2D::defaultScene()");
    xRot=0; yRot=0; zRot=0; zTra=0; nSca=1;
}


/*
bool Scene2D::tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement *element, int &index)
{

    Logger::getLogger()->writeLog("Scene2D::tryToSelectControlsInSelectedFigure");
    GLint viewport[4]; // декларируем матрицу поля просмотра
    glGetIntegerv(GL_VIEWPORT, viewport); // извлечь матрицу поля просмотра в viewport
    GLuint selectBuffer[16]; // буфер выбора (буфер совпадений)
    GLint hitsForControl = 0;

    glSelectBuffer(16, selectBuffer); // использовать указанный буфер выбора
    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPushMatrix(); // поместить текущую матрицу в стек матриц
    glRenderMode(GL_SELECT); // переход в режим выбора
    glLoadIdentity(); // загрузить единичную матрицу

    // новый объём под указателем мыши
    gluPickMatrix((GLdouble)mp.x(), (GLdouble)(viewport[3]-mp.y()), 10.0, 10.0, viewport);
    // мировое окно
    if (width() >= height())
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 1.0);
    else
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);


    glMatrixMode(GL_MODELVIEW); // модельно-видовая матрица стала активной
    glLoadIdentity();           // загружается единичная матрица моделирования

    glInitNames(); // инициализируется и очищается стек имён
    glPushName(0); // в стек имён помещается значение 0 (обязательно должен храниться хотя бы один элемент)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    for (int i = 1; i < element->getNumberOfControls() + 1; ++i)
    {
        glPushMatrix();
        glScalef(nSca, nSca, nSca);
        gluLookAt(xDelta,yDelta,0.5,
                  xDelta,yDelta,-10,
                  0,1,0);
        glLoadName(i); // загрузить имя на вершину стека имён
        element->drawControlElement(i - 1, 15.0f, 30.0f);
        glPopMatrix();
    }


    hitsForControl = glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования

    if (hitsForControl > 0) // есть совпадания и нет ошибок
    {
        index = selectBuffer[3] - 1;
        glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
        glPopMatrix(); // извлечь матрицу из стека матриц
        updateGL(); // обновить изображение
        return true;
    }

    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPopMatrix(); // извлечь матрицу из стека матриц
    updateGL(); // обновить изображение
    return false;
}
*/


/*
bool Scene2D::tryToSelectFigures(QPoint mp, RoadElement *&element)
{
    Logger::getLogger()->writeLog("Scene2D::tryToSelectFigures");
    GLint viewport[4]; // декларируем матрицу поля просмотра
    glGetIntegerv(GL_VIEWPORT, viewport); // извлечь матрицу поля просмотра в viewport
    GLuint selectBuffer[40]; // буфер выбора (буфер совпадений)
    GLint hitsForFigure = 0;

    glSelectBuffer(40, selectBuffer); // использовать указанный буфер выбора
    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPushMatrix(); // поместить текущую матрицу в стек матриц
    glRenderMode(GL_SELECT); // переход в режим выбора
    glLoadIdentity(); // загрузить единичную матрицу

    // новый объём под указателем мыши
    gluPickMatrix((GLdouble)mp.x(), (GLdouble)(viewport[3]-mp.y()), 1.0, 1.0, viewport);
    // мировое окно
    if (width() >= height())
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 1.0);
    else
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);


    glMatrixMode(GL_MODELVIEW); // модельно-видовая матрица стала активной
    glLoadIdentity();           // загружается единичная матрица моделирования

    glInitNames(); // инициализируется и очищается стек имён
    glPushName(0); // в стек имён помещается значение 0 (обязательно должен храниться хотя бы один элемент)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int i = 1;
    for (int j = model->getNumberOfGroups() - 1; j >= 0; --j)
    {
        for (std::list<RoadElement*>::iterator it = model->getGroup(j).begin();
             it != model->getGroup(j).end(); ++it)
        {
            glPushMatrix();
            glScalef(nSca, nSca, nSca);
            gluLookAt(xDelta,yDelta,0.5,
                      xDelta,yDelta,-10,
                      0,1,0);
            glLoadName(i++); // загрузить имя на вершину стека имён
            (*it)->drawFigure();
            glPopMatrix();

        }
    }

    hitsForFigure=glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования

    if (hitsForFigure > 0) // есть совпадания и нет ошибок
    {
        i = selectBuffer[3] - 1; // имя фигуры верхняя фигура
        for (int j = model->getNumberOfGroups() - 1; j >= 0; --j)
        {
            if (i < model->getGroup(j).size())
            {
                std::list<RoadElement*>::iterator it = model->getGroup(j).begin();
                for(int k = 0; k < i; ++k)
                    ++it;
                element = *it;
                glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
                glPopMatrix(); // извлечь матрицу из стека матриц
                updateGL(); // обновить изображение
                return true;
            }
            else
            {
                i -= model->getGroup(j).size();
            }
        }
    }

    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPopMatrix(); // извлечь матрицу из стека матриц
    updateGL(); // обновить изображение
    */
/*
    return false;
}
*/

void Scene2D::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    /*
    Logger::getLogger()->writeLog("Scene2D::getWindowCoord");
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
    */
}

void Scene2D::getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    Logger::getLogger()->writeLog("Scene2D::getWorldCoord");
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
    y = viewport[3]-y;
    gluUnProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
}



void Scene2D::drawAxis()
{
    Logger::getLogger()->writeLog("Scene2D::drawAxis()");
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glLineWidth(3.0f);

    glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f( 1.0f,  0.0f,  0.007f);
    glVertex3f(-1.0f,  0.0f,  0.007f);
    glEnd();

    QColor halfGreen(0, 128, 0, 255);
    qglColor(halfGreen);
    glColor4f(0.00f, 0.50f, 0.00f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f( 0.0f,  1.0f,  0.007f);
    glVertex3f( 0.0f, -1.0f,  0.007f);

    glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
    glVertex3f( 0.0f,  0.0f,  1.0f);
    glVertex3f( 0.0f,  0.0f, -1.0f);
    glEnd();
    glEnable(GL_DEPTH_TEST);
}

void Scene2D::drawRect(QPoint p1, QPoint p2)
{
    Logger::getLogger()->writeLog("Scene2D::drawRect(QPoint p1, QPoint p2)");
    GLdouble x1, y1, z1;
    GLdouble x2, y2, z2;
    getWorldCoord(p1.x(), p1.y(), 0, x1, y1, z1);
    getWorldCoord(p2.x(), p2.y(), 0, x2, y2, z2);
    glDisable(GL_DEPTH_TEST);

    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x1, y1, z1);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x1, y2, z1);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x2, y2, z2);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x2, y1, z2);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x1, y1, z1);
    glEnd();

    glPointSize(1.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x1, y1, z1);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x1, y2, z1);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x2, y2, z2);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x2, y1, z2);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x1, y1, z1);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    ////qDebug() << "drawRect";
}

void Scene2D::dragEnterEvent(QDragEnterEvent* event)
{
    Logger::getLogger()->writeLog("Scene2D::dragEnterEvent(QDragEnterEvent* event)");
    if (stateManager)
        stateManager->dragEnterEvent(event);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::dragEnterEvent(QDragEnterEvent* event) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::stateManager = NULL, Scene2D::dragEnterEvent(QDragEnterEvent* event) stopped");
    }
}

void Scene2D::dropEvent(QDropEvent* event)
{
    Logger::getLogger()->writeLog("Scene2D::dropEvent(QDropEvent* event)");
    if (stateManager)
        stateManager->dropEvent(event);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::dropEvent(QDropEvent* event) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::stateManager = NULL, Scene2D::dropEvent(QDropEvent* event) stopped");
    }
}

void Scene2D::drawGrid()
{
    Logger::getLogger()->writeLog("Scene2D::drawGrid()");
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    if (showGrid)
    {
    glLineWidth(1.0);
    for (int i = -1000; i < 1001; ++i)
    {
        glBegin(GL_LINES);
        glColor3d(0.7,0.7,0.7);
        glVertex3d(i, -1000.0, 0.005f);
        glVertex3d(i, 1000.0, 0.005f);
        glEnd();
    }

    for (int i = -1000; i < 1001; ++i)
    {
        glBegin(GL_LINES);
        glColor3d(0.7,0.7,0.7);
        glVertex3d(-1000.0, i, 0.005f);
        glVertex3d(1000.0, i, 0.005f);
        glEnd();
    }
    }
    ////qDebug() << "Draw grid";
    glEnable(GL_DEPTH_TEST);
}

bool Scene2D::isRulerActive()
{
    Logger::getLogger()->writeLog("Scene2D::isRulerActive() = " + QString::number(rulerIsActive));
    return rulerIsActive;
}


void Scene2D::setModel(Model *model)
{
    Logger::getLogger()->writeLog("Scene2D::setModel(Model *model)");
    if (model)
    {
        this->model = model;
        if (properties)
        {
            stateManager = new StateManager(this->model, this, properties);
            connect(this, SIGNAL(rulerStatusChanged(bool)), stateManager, SLOT(setRulerActive(bool)));
        }
        else
        {
            QMessageBox::critical(0, "Ошибка", "Scene2D::properties = NULL,\n cannot create StateManager", QMessageBox::Yes | QMessageBox::Default);
            Logger::getLogger()->writeLog("Scene2D::properties = NULL, cannot create StateManager");
        }
    }
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::setModel(Model *model), model = NULL,\n cannot work with models", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::setModel(Model *model), model = NULL, cannot work with models");
    }

}


void Scene2D::setProperties(QFormLayout *properties)
{
    Logger::getLogger()->writeLog("Scene2D::setProperties(QFormLayout *properties)");
    if (properties)
        this->properties = properties;
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::setProperties(QFormLayout *properties), properties = NULL,\n cannot use properties for object Inspector", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::setProperties(QFormLayout *properties), properties = NULL, cannot use properties for object Inspector");
    }

}

void Scene2D::drawBackground(QPainter *painter)
{
    //painter->setPen(Qt::NoPen);
    //painter->setBrush(gradient);
    //painter->drawRect(rect());

}

void Scene2D::setDrawRectStatus(bool status)
{
    Logger::getLogger()->writeLog("Scene2D::setDrawRectStatus(bool status)");
    drawRectStatus = status;
}

void Scene2D::activateRuler()
{
    Logger::getLogger()->writeLog("Scene2D::activateRuler()");
    rulerIsActive = true;
}

void Scene2D::deActivateRuler()
{
    Logger::getLogger()->writeLog("Scene2D::deActivateRuler()");
    rulerIsActive = false;
}

void Scene2D::setRulerActive(bool status)
{
    Logger::getLogger()->writeLog("Scene2D::setRulerActive(bool status)");
    if (rulerIsActive == status)
        return;
    rulerIsActive = status;
    emit rulerStatusChanged(status);
}

void Scene2D::listItemClicked(QListWidgetItem *item)
{
    /*
    Logger::getLogger()->writeLog("Scene2D::listItemClicked = " + item->text());
    if (stateManager)
    {
    if (item->text() == "Дорога ломаная")
    {
        setMouseTracking(true);
        stateManager->roadBuilderState->setName("RoadBroken");
        stateManager->roadBuilderState->setLayer(0);
        stateManager->setState(stateManager->roadBuilderState);
    }
    if (item->text() == "Сплошная")
    {
        stateManager->lineBuilderState->useColor = true;
        stateManager->lineBuilderState->setWidth(0.1);
        stateManager->lineBuilderState->setName("LineSolidBroken");
        stateManager->lineBuilderState->setLayer(1);
        setMouseTracking(true);
        stateManager->setState(stateManager->lineBuilderState);
    }
    if (item->text() == "Прерывистая")
    {
        stateManager->lineBuilderState->useColor = false;
        stateManager->lineBuilderState->setTexture(QString(":/textures/intermittent.png"), 0.8f);
        stateManager->lineBuilderState->setWidth(0.1);
        stateManager->lineBuilderState->setName("LineIntermittentBroken");
        stateManager->lineBuilderState->setLayer(1);
        setMouseTracking(true);
        stateManager->setState(stateManager->lineBuilderState);
    }
    if (item->text() == "Двойная сплошая")
    {
        stateManager->lineBuilderState->useColor = false;
        stateManager->lineBuilderState->setTexture(QString(":/textures/double_solid.png"), 1.0f);
        stateManager->lineBuilderState->setWidth(0.28);
        stateManager->lineBuilderState->setName("LineDoubleSolidBroken");
        stateManager->lineBuilderState->setLayer(1);
        setMouseTracking(true);
        stateManager->setState(stateManager->lineBuilderState);
    }
    if (item->text() == "Двойная прерывистая")
    {
        stateManager->lineBuilderState->useColor = false;
        stateManager->lineBuilderState->setTexture(QString(":/textures/double_solid_intermittent.png"), 0.8f);
        stateManager->lineBuilderState->setWidth(0.28);
        stateManager->lineBuilderState->setName("LineDoubleSolidIntermittentBroken");
        stateManager->lineBuilderState->setLayer(1);
        setMouseTracking(true);
        stateManager->setState(stateManager->lineBuilderState);
    }
    if (item->text() == "Трамвайные пути")
    {
        stateManager->lineBuilderState->useColor = false;
        stateManager->lineBuilderState->setTexture(QString(":/textures/tramways.png"), 1.5f);
        stateManager->lineBuilderState->setWidth(1.5);
        stateManager->lineBuilderState->setName("TramwaysBroken");
        stateManager->lineBuilderState->setLayer(1);
        setMouseTracking(true);
        stateManager->setState(stateManager->lineBuilderState);
    }
    if (item->text() == "Железная дорога")
    {
        stateManager->lineBuilderState->useColor = false;
        stateManager->lineBuilderState->setTexture(QString(":/textures/railroad.png"), 2.1f);
        stateManager->lineBuilderState->setWidth(2.1f);
        stateManager->lineBuilderState->setName("RailroadBroken");
        stateManager->lineBuilderState->setLayer(1);
        setMouseTracking(true);
        stateManager->setState(stateManager->lineBuilderState);
    } 
    }
    */
}

void Scene2D::setShowGrid(bool status)
{
    Logger::getLogger()->writeLog("Scene2D::setShowGrid = " + QString::number(status));
    showGrid = status;
}


void Scene2D::keyReleaseEvent(QKeyEvent *pe)
{
    Logger::getLogger()->writeLog("Scene2D::keyReleaseEvent(QKeyEvent *pe)");
    if (stateManager)
        stateManager->keyReleaseEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::keyReleaseEvent(QKeyEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::stateManager = NULL, Scene2D::keyReleaseEvent(QKeyEvent *pe) stopped");
    }
}


void Scene2D::contextMenuEvent(QContextMenuEvent *pe)
{
    Logger::getLogger()->writeLog("Scene2D::contextMenuEvent(QContextMenuEvent *pe)");
    if (stateManager)
        stateManager->contextMenuEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::contextMenuEvent(QContextMenuEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("Scene2D::stateManager = NULL, Scene2D::contextMenuEvent(QContextMenuEvent *pe) stopped");
    }
}
