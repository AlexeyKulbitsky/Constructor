#include <QtGui>
#include <math.h>
#include "scene2D.h"
#include <iostream>
#include <QImage>
#include "yandexmapsview.h"
#include "googlemapsview.h"
#include "osmview.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QProgressDialog>

bool Scene2D::log = true;

GLint viewport[4]; // декларируем матрицу поля просмотра

Scene2D::Scene2D(QWidget* parent) : QGLWidget(parent), widget(0)
{
    gridList = 0;
    recompileGrid = true;
    firstTime = true;
    delay = 0;
    widgetWidth = widgetHeight = 1.0f;
    scaleImage = false;
    fixedScale = false;
    widget = NULL;
    sceneActive = true;
    showScene = true;
    showMaps = false;
    scaleStep = 1.05f;
    gridStep = 1.0f;
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
    settings = NULL;
}

Scene2D::Scene2D(QSettings *settings, QWidget *parent) : QGLWidget(parent)
{
    gridList = 0;
    recompileGrid = true;
    firstTime = true;
    delay = 0;
    widgetWidth = widgetHeight = 1.0f;
    scaleImage = false;
    fixedScale = false;
    widget = NULL;
    sceneActive = true;
    showScene = true;
    showMaps = false;
    scaleStep = 1.05f;
    gridStep = 1.0f;
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
    this->settings = settings;
    loadSettings();
}



Scene2D::~Scene2D()
{
    // saveSettings();
    stateManager = NULL;
    model = NULL;
    properties = NULL;
}

void Scene2D::initializeGL()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::initializeGL\n";
    glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_NORMALIZE);

}

void Scene2D::resizeGL(int nWidth, int nHeight)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::resizeGL(int nWidth, int nHeight)"
                                       << " nWidth = " << nWidth
                                       << " nHeight = " << nHeight << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::paintGL()\n";

    if (firstTime)
    {
        resizeGL(width(), height());
        firstTime = false;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawModel();
}


void Scene2D::mousePressEvent(QMouseEvent* pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::mousePressEvent(QMouseEvent* pe)\n";
    if (pe->button() == Qt::RightButton)
        rightButtonIsPressed = true;
    this->setFocus();
    if (stateManager)
        stateManager->mousePressEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::mousePressEvent(QMouseEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::mousePressEvent(QMouseEvent* pe) stopped\n";
        QApplication::exit(0);
    }
    //    if (widget && rightButtonIsPressed)
    //    {
    //        QMouseEvent* event = new QMouseEvent(QEvent::MouseButtonPress, pe->pos(), pe->globalPos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    //        //qobject_cast<YandexMapsView*>(widget)->mousePre(event);
    //        QCoreApplication::sendEvent(widget, event);
    //    }
}

void Scene2D::mouseReleaseEvent(QMouseEvent* pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::mouseReleaseEvent(QMouseEvent* pe)\n";
    if (stateManager)
        stateManager->mouseReleaseEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::mouseReleaseEvent(QMouseEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::mouseReleaseEvent(QMouseEvent* pe) stopped\n";
        QApplication::exit(0);
    }
    //    if (widget && rightButtonIsPressed)
    //    {
    //        QMouseEvent* event = new QMouseEvent(QEvent::MouseButtonPress, pe->pos(), pe->globalPos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    //        //qobject_cast<YandexMapsView*>(widget)->mouseRel(event);
    //        QCoreApplication::sendEvent(widget, event);
    //        rightButtonIsPressed = false;
    //    }
}

void Scene2D::mouseMoveEvent(QMouseEvent* pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::mouseMoveEvent(QMouseEvent* pe)\n";
    this->setFocus();
    if (stateManager)
        stateManager->mouseMoveEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::mouseMoveEvent(QMouseEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::mouseMoveEvent(QMouseEvent* pe) stopped\n";
        QApplication::exit(0);
    }
    //    if (widget && rightButtonIsPressed)
    //    {
    //        QMouseEvent* event = new QMouseEvent(QEvent::MouseMove, pe->pos(), pe->globalPos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    //        //widget->setMouseTracking(false);
    //        //qobject_cast<YandexMapsView*>(widget)->mouseMo(event);
    //        QCoreApplication::sendEvent(widget, event);

    //        if (widget->objectName() == "YandexMaps")
    //        {
    //            float dx = float(this->width());
    //            float dy = float(this->height());
    //            float r = qobject_cast<YandexMapsView*>(widget)->getDiagonal();
    //            float s = sqrt(dx * dx + dy * dy) / (this->width() * r * ratio) * 2.0f;
    //            this->setScale(s);
    //        }

    //    }
}

void Scene2D::wheelEvent(QWheelEvent* pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::wheelEvent(QWheelEvent* pe)\n";
    if (fixedScale)
        return;
    if (sceneActive)
    {
        if (stateManager)
            stateManager->wheelEvent(pe);
        else
        {
            QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::wheelEvent(QWheelEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
            if (log)
                Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::wheelEvent(QWheelEvent* pe) stopped\n";
            QApplication::exit(0);
        }
        //        if (widget)
        //        {
        ////            if ((pe->delta())>0)
        ////            {
        ////                if (widget->objectName() == "YandexMaps")
        ////                {
        ////                    YandexMapsView* yandex = qobject_cast<YandexMapsView*>(widget);
        ////                    int zoom = yandex->getZoom().toInt();
        ////                    if (zoom == 19)
        ////                    {
        ////                        QSize s = widgetImage.size();
        ////                        s *= scaleStep;
        ////                        widgetImage = widgetImage.scaled(s);
        ////                        scaleImage = true;
        ////                    }
        ////                    else
        ////                    {
        ////                        scaleImage = false;
        ////                        QCoreApplication::sendEvent(widget, pe);
        ////                    }
        ////                }
        ////            }
        //            if (widget->objectName() == "YandexMaps")
        //                qobject_cast<YandexMapsView*>(widget)->wheelEv(pe);
        //            else
        //                if (widget->objectName() == "GoogleMaps")
        //                    qobject_cast<GoogleMapsView*>(widget)->wheelEv(pe);
        //            //QCoreApplication::sendEvent(widget, pe);
        //        }
    }


}

void Scene2D::keyPressEvent(QKeyEvent* pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::keyPressEvent(QKeyEvent* pe)\n";
    if (stateManager)
        stateManager->keyPressEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::keyPressEvent(QKeyEvent* pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::keyPressEvent(QKeyEvent* pe) stopped\n";
        QApplication::exit(0);
    }
}

void Scene2D::scalePlus()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::scalePlus()\n";
    nSca = nSca*scaleStep;
    emit scaleChanged(nSca);
}

void Scene2D::scaleMinus()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::scaleMinus()\n";
    nSca = nSca/scaleStep;
    emit scaleChanged(nSca);
}

void Scene2D::rotateUp()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::rotateUp()\n";
    xRot += 1.0;
}

void Scene2D::rotateDown()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::rotateDown()\n";
    xRot -= 1.0;
}

void Scene2D::rotateLeft()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::rotateLeft()\n";
    zRot += 1.0;
}

void Scene2D::rotateRight()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::rotateRight()\n";
    zRot -= 1.0;
}

void Scene2D::translateDown()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::translateDown()\n";
    zTra -= 0.05;
}

void Scene2D::translateUp()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::translateUp()\n";
    zTra += 0.05;
}

void Scene2D::defaultScene()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::defaultScene()\n";
    xRot=0; yRot=0; zRot=0; zTra=0; nSca=1;
}



void Scene2D::getWindowCoord(double, double, double, double&, double&, double&)
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
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::getWorldCoord\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::drawAxis()\n";
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
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Scene2D::drawRect(QPoint p1, QPoint p2)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::drawRect(QPoint p1, QPoint p2)\n";
    GLdouble x1, y1, z1;
    GLdouble x2, y2, z2;
    getWorldCoord(p1.x(), p1.y(), 0.2f, x1, y1, z1);
    getWorldCoord(p2.x(), p2.y(), 0.2f, x2, y2, z2);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
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
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void Scene2D::dragEnterEvent(QDragEnterEvent* event)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::dragEnterEvent(QDragEnterEvent* event)\n";
    if (stateManager)
        stateManager->dragEnterEvent(event);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::dragEnterEvent(QDragEnterEvent* event) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::dragEnterEvent(QDragEnterEvent* event) stopped\n";
        QApplication::exit(0);
    }
}

void Scene2D::dropEvent(QDropEvent* event)
{
    Logger::getLogger()->infoLog() << "Scene2D::dropEvent(QDropEvent* event)\n";
    if (stateManager)
        stateManager->dropEvent(event);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::dropEvent(QDropEvent* event) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::dropEvent(QDropEvent* event) stopped\n";
        QApplication::exit(0);
    }
}

void Scene2D::drawGrid()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::drawGrid()\n";
    if (gridStep <= 0.0f)
        return;
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
//    if (showGrid)
//    {
//        glLineWidth(1.0);
//        for (double i = -1000.0; i < 1001.0; i += gridStep)
//        {
//            glBegin(GL_LINES);
//            glColor3d(0.7,0.7,0.7);
//            glVertex3d(i, -1000.0, 0.005f);
//            glVertex3d(i, 1000.0, 0.005f);
//            glEnd();
//        }

//        for (double i = -1000.0; i < 1001.0; i += gridStep)
//        {
//            glBegin(GL_LINES);
//            glColor3d(0.7,0.7,0.7);
//            glVertex3d(-1000.0, i, 0.005f);
//            glVertex3d(1000.0, i, 0.005f);
//            glEnd();
//        }
//    }
//    glEnable(GL_LIGHTING);
//    glEnable(GL_DEPTH_TEST);

    if (recompileGrid)
    {
        if (gridList > 0)
            glDeleteLists(gridList, 1);

        gridList = glGenLists(1);
        glNewList(gridList, GL_COMPILE);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glLineWidth(1.0);
        for (double i = -1000.0; i < 1001.0; i += gridStep)
        {
            glBegin(GL_LINES);
            glColor3d(0.7,0.7,0.7);
            glVertex3d(i, -1000.0, 0.005f);
            glVertex3d(i, 1000.0, 0.005f);
            glEnd();
        }

        for (double i = -1000.0; i < 1001.0; i += gridStep)
        {
            glBegin(GL_LINES);
            glColor3d(0.7,0.7,0.7);
            glVertex3d(-1000.0, i, 0.005f);
            glVertex3d(1000.0, i, 0.005f);
            glEnd();
        }
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glEndList();
        recompileGrid = false;
    }
    if (showGrid)
    {
        glCallList(gridList);
    }
}

bool Scene2D::isRulerActive()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::isRulerActive()\n";
    return rulerIsActive;
}


void Scene2D::setModel(Model *model)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setModel(Model *model)\n";
    if (model)
    {
        this->model = model;
        connect(model, SIGNAL(visibilityChanged()), this, SLOT(updateGL()));
        if (properties)
        {
            stateManager = new StateManager(this->model, this, properties);
            connect(this, SIGNAL(rulerStatusChanged(bool)), stateManager, SLOT(setRulerActive(bool)));
        }
        else
        {
            QMessageBox::critical(0, "Ошибка", "Scene2D::properties = NULL,\n cannot create StateManager", QMessageBox::Yes | QMessageBox::Default);
            if (log)
                Logger::getLogger()->errorLog() << "Scene2D::properties = NULL, cannot create StateManager\n";
            QApplication::exit(0);
        }
    }
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::setModel(Model *model), model = NULL,\n cannot work with models", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::setModel(Model *model), model = NULL, cannot work with models\n";
        QApplication::exit(0);
    }

}

void Scene2D::setCamera(CameraView *camera)
{
    stateManager->camera = camera;
    stateManager->fileManagerJSON->setCameraView(camera);
}


void Scene2D::setProperties(QVBoxLayout *properties)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setProperties(QFormLayout *properties)\n";
    if (properties)
        this->properties = properties;
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::setProperties(QFormLayout *properties), properties = NULL,\n cannot use properties for object Inspector", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::setProperties(QFormLayout *properties), properties = NULL, cannot use properties for object Inspector\n";
        QApplication::exit(0);
    }

}

void Scene2D::drawBackground(QPainter*)
{
    //painter->setPen(Qt::NoPen);
    //painter->setBrush(gradient);
    //painter->drawRect(rect());
}

void Scene2D::setDrawRectStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setDrawRectStatus(bool status)\n";
    drawRectStatus = status;
}

void Scene2D::setSettings(QSettings *settings)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setDrawRectStatus(bool status)\n";
    this->settings = settings;
}

void Scene2D::setOverlayWidget(QWebView *widget)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setOverlayWidget(QWidget *widget)\n";
    if (widget == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::setOverlayWidget(QWidget *widget) widget = NULL, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::setOverlayWidget(QWidget *widget) widget = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->widget = widget;
    //widgetWidth = widget->width();
    //widgetHeight = widget->height();
    //setShowMapsStatus(widget->isVisible());
    //connect(this, SIGNAL(showMapsStatusChanged(bool)), widget, SLOT(setVisible(bool)));
}

void Scene2D::drawModel()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::drawModel()\n";
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения фонового света ( НОВОЕ )
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения диффузного света ( НОВОЕ )

    GLfloat LightPosition[]= { 0.0f, 0.0f, 200.0f, 0.0f };     // Позиция света ( НОВОЕ )
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // Установка Фонового Света
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glScalef(nSca, nSca, nSca);

    gluLookAt(xDelta,yDelta,0.5,
              xDelta,yDelta,-10,
              0,1,0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

    // Отрисовка спутниковой подложки
    if (model->isGroupVisible(model->getNumberOfGroups() - 1))
    {
        for (int i = 0; i < model->getGroup(model->getNumberOfGroups() - 1).size(); ++i)
        {
            model->getGroup(model->getNumberOfGroups() - 1)[i]->drawFigure(this);
        }
    }

    // Отрисовка координатной сетки
    drawGrid();
    // Отрисовка осей
    drawAxis();

    // Отрисовка моделей, кроме спутника
    if (model)
    {
        for (int i =  0; i < model->getNumberOfGroups() - 1; ++i)
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

    // Отрисвока размеров (все, кроме линейки и спутниковой подложки)
    for (int i =  0; i < model->getNumberOfGroups() - 2; ++i)
    {
        if (model->isGroupVisible(i) == true)
        {
            for(QList<RoadElement*>::iterator it = model->getGroup(i).begin();
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
    // Отрисовка линейки
    int count = model->getNumberOfGroups() - 2;
    if (model->isGroupVisible(count) == true)
    {
        for(QList<RoadElement*>::iterator it = model->getGroup(count).begin();
            it != model->getGroup(count).end(); ++it)
        {
            if ((*it)->getName() == "Ruler")
                (*it)->drawMeasurements(this);
        }
    }


    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHTING);

    if (drawRectStatus == true)
        drawRect(rectPoint1, rectPoint2);

    glPopMatrix();
}

void Scene2D::loadSettings()
{
    settings->beginGroup("/Settings/View/Scene2D");

    nSca = settings->value("/Scale", 0.2f).toFloat();

    int red = settings->value("/Substrate/Color/Red",200).toInt();
    int green = settings->value("/Substrate/Color/Green",200).toInt();
    int blue = settings->value("/Substrate/Color/Blue",200).toInt();
    substrateColor.setRgb(red, green, blue);
    substrateLength = settings->value("/Substrate/Length",1000.0f).toFloat();
    substrateWidth = settings->value("/Substrate/Width",1000.0f).toFloat();
    drawSubstrateStatus = settings->value("/Substrate/Status",true).toBool();

    settings->endGroup();
}

void Scene2D::saveSettings()
{
    settings->beginGroup("/Settings/View/Scene2D");

    settings->setValue("/Scale",nSca);

    settings->setValue("/Substrate/Color/Red",substrateColor.red());
    settings->setValue("/Substrate/Color/Green",substrateColor.green());
    settings->setValue("/Substrate/Color/Blue",substrateColor.blue());
    settings->setValue("/Substrate/Length",substrateLength);
    settings->setValue("/Substrate/Width",substrateWidth);
    settings->setValue("/Substrate/Status",drawSubstrateStatus);

    settings->endGroup();
}

bool Scene2D::getLogging()
{
    return log;
}

void Scene2D::drawSubstrate()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::drawSubstrate()\n";
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glBegin(GL_TRIANGLES);
    qglColor(substrateColor);
    glVertex3f(substrateLength / -2.0f, substrateWidth / -2.0f, 0.0f);
    qglColor(substrateColor);
    glVertex3f(substrateLength / 2.0f, substrateWidth / 2.0f, 0.0f);
    qglColor(substrateColor);
    glVertex3f(substrateLength / -2.0f, substrateWidth / 2.0f, 0.0f);

    qglColor(substrateColor);
    glVertex3f(substrateLength / -2.0f, substrateWidth / -2.0f, 0.0f);
    qglColor(substrateColor);
    glVertex3f(substrateLength / 2.0f, substrateWidth / -2.0f, 0.0f);
    qglColor(substrateColor);
    glVertex3f(substrateLength / 2.0f, substrateWidth / 2.0f, 0.0f);
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

}

void Scene2D::getWorldCoord(QPoint p, float &x, float &y)
{
    double x1, y1, z1, wx, wy, wz;
    x1 = p.x();
    y1 = p.y();
    z1 = 0.0;
    RoadElement::getWorldCoord(x1, y1, z1, wx, wy, wz);
    x = float(wx) / nSca + xDelta;
    y = float(wy) / nSca + yDelta;
}

void Scene2D::copy()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::copy()\n";
    if (stateManager)
        stateManager->copy();
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::copy() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::copy() stopped\n";
        QApplication::exit(0);
    }
}

void Scene2D::paste()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::paste()\n";
    if (stateManager)
        stateManager->paste();
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::paste() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::paste() stopped\n";
        QApplication::exit(0);
    }
}

void Scene2D::cut()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::cut()\n";
    if (stateManager)
        stateManager->cut();
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::cut() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::cut() stopped\n";
        QApplication::exit(0);
    }
}

void Scene2D::del()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::del()\n";
    if (stateManager)
        stateManager->del();
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::del() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::del() stopped\n";
        QApplication::exit(0);
    }
}

void Scene2D::saveToPresets()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::saveToPresets()\n";
    if (stateManager)
        stateManager->saveToPresets();
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::saveToPresets() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::saveToPresets() stopped\n";
        QApplication::exit(0);
    }
}

void Scene2D::setDrawSubstrateStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setDrawSubstrateStatus(bool status)"
                                       << " status = " << status << "\n";
    drawSubstrateStatus = status;
}

void Scene2D::setSubstrateLength(double length)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setSubstrateLength(double length)"
                                       << " length = " << length << "\n";
    substrateLength = length;
}

void Scene2D::setSubstrateWidth(double length)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setSubstrateWidth(double length)"
                                       << " length = " << length << "\n";
    substrateWidth = length;
}

void Scene2D::setSubstrateColor()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setSubstrateColor()\n";
    substrateColor = QColorDialog::getColor(substrateColor, 0, "Цвет подложки");
    updateGL();
}

void Scene2D::setMapType(int index)
{
    switch(index)
    {
    case 0:
        emit currentIndexChanged(1);
        break;
    case 1:
        emit currentIndexChanged(2);
        break;
    case 2:
        emit currentIndexChanged(3);
        break;
    default:
        break;
    }
}

void Scene2D::setWidgetWidth(int w)
{
    if (widgetWidth == w)
        return;
    widgetWidth = w;
    emit widgetWidthChanged(w);
}

void Scene2D::setWidgetHeight(int h)
{
    if (widgetHeight == h)
        return;
    widgetHeight = h;
    emit widgetHeightChanged(h);
}

void Scene2D::setWidgetWidthScaleFactor(double factor)
{
    if (widgetWidthScaleFactor == factor)
        return;
    widgetWidthScaleFactor = factor;
    float w = widgetWidth;
    w *= factor;
    setWidgetWidth(int(w));
}

void Scene2D::setWidgetHeightScaleFactor(double factor)
{
    if (widgetHeightScaleFactor == factor)
        return;
    widgetHeightScaleFactor = factor;
    float h = widgetHeight;
    h *= factor;
    setWidgetHeight(int(h));
}

void Scene2D::setDelay(int seconds)
{
    if (delay == seconds)
        return;
    delay = seconds;
    emit delayChanged(seconds);
}

void Scene2D::saveImage()
{
    QProgressDialog progress("Захват карты...", "Отменить", 0, 4);
    progress.setCancelButton(0);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    int w = widget->width();
    int h = widget->height();

    progress.setValue(0);
    widget->resize(widgetWidth, widgetHeight);
    progress.setValue(1);
    qSleep(delay * 1000);
    widgetImage = QImage(widget->size(),QImage::Format_RGB32);
    progress.setValue(2);
    if (widget->objectName() == "YandexMaps")
    {
        YandexMapsView* yandex = qobject_cast<YandexMapsView*>(widget);
        yandex->setActive(false);
        widget->render(&widgetImage);
        float dx = float(widgetImage.width());
        float dy = float(widgetImage.height());
        float r = yandex->getDiagonal();
        float s = sqrt(dx * dx + dy * dy);
        float factor = r / s;
        float w = dx * factor;
        float h = dy * factor;
        MapPlane *mapPlane = new MapPlane(w, h, widgetImage);
        for (int i = 0; i < model->getGroup(mapPlane->getLayer()).size(); ++i)
        {
            delete model->getGroup(mapPlane->getLayer())[i];
        }
        model->getGroup(mapPlane->getLayer()).clear();
        model->getGroup(mapPlane->getLayer()).push_back(mapPlane);
        yandex->setActive(true);
        progress.setValue(3);
    }
    else
        if (widget->objectName() == "GoogleMaps")
        {
            GoogleMapsView* google = qobject_cast<GoogleMapsView*>(widget);
            google->setActive(false);
            widget->render(&widgetImage);
            float dx = float(widgetImage.width());
            float dy = float(widgetImage.height());
            float r = google->getDiagonal();
            float s = sqrt(dx * dx + dy * dy);
            float factor = r / s;
            float w = dx * factor;
            float h = dy * factor;
            MapPlane *mapPlane = new MapPlane(w, h, widgetImage);
            for (int i = 0; i < model->getGroup(mapPlane->getLayer()).size(); ++i)
            {
                delete model->getGroup(mapPlane->getLayer())[i];
            }
            model->getGroup(mapPlane->getLayer()).clear();
            model->getGroup(mapPlane->getLayer()).push_back(mapPlane);
            google->setActive(true);
            progress.setValue(3);
        }
        else
            if (widget->objectName() == "OSM")
            {
                OSMview *osm = qobject_cast<OSMview*>(widget);
                widget->render(&widgetImage);
                float dx = float(widgetImage.width());
                float dy = float(widgetImage.height());
                float r = osm->getDiagonal();
                float s = sqrt(dx * dx + dy * dy);
                float factor = r / s;
                float w = dx * factor;
                float h = dy * factor;
                MapPlane *mapPlane = new MapPlane(w, h, widgetImage);
                for (int i = 0; i < model->getGroup(mapPlane->getLayer()).size(); ++i)
                {
                    delete model->getGroup(mapPlane->getLayer())[i];
                }
                model->getGroup(mapPlane->getLayer()).clear();
                model->getGroup(mapPlane->getLayer()).push_back(mapPlane);
                progress.setValue(3);
            }
    widget->resize(w, h);
    progress.setValue(4);
}

void Scene2D::qSleep(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}



void Scene2D::activateRuler()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::activateRuler()\n";
    rulerIsActive = true;
}

void Scene2D::deActivateRuler()
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::deActivateRuler()\n";
    rulerIsActive = false;
}

void Scene2D::setRulerActive(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setRulerActive(bool status)\n";
    if (rulerIsActive == status)
        return;
    rulerIsActive = status;
    emit rulerStatusChanged(status);
}



void Scene2D::setShowGrid(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "setShowGrid(bool status)"
                                       << " status = " << status << "\n";
    showGrid = status;
}

void Scene2D::setScale(double scale)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setScale(double scale)"
                                       << " scale = " << scale << "\n";
    if (this->nSca == scale)
        return;
    this->nSca = scale;
//    if (this->nSca <= 0.0f)
//        this->nSca = 0.001f;
    updateGL();
    emit scaleChanged(scale);
}

void Scene2D::setScaleStep(double step)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setScaleStep(double step)"
                                       << " step = " << step << "\n";
    if (this->scaleStep == step)
        return;
    this->scaleStep = step;
    emit scaleStepChanged(step);
}

void Scene2D::setGridStep(double step)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setGridStep(double step)"
                                       << " step = " << step << "\n";
    if (this->gridStep == step)
        return;
    this->gridStep = step;
    if (gridStep <= 0.0f)
        gridStep = 0.0f;
    recompileGrid = true;
    updateGL();
    emit gridStepChanged(step);
}

void Scene2D::getProperties(QFormLayout *layout)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::getProperties(QFormLayout *layout)\n";
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::getProperties(QFormLayout *layout) layout = NULL",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::getProperties(QFormLayout *layout) layout = NULL\n";
        QApplication::exit(0);
    }
    firstTime = true;
    //disconnect(stepDialog, 0, this, 0);
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }

    QDoubleSpinBox* scaleSpinBox = new QDoubleSpinBox();
    scaleSpinBox->setKeyboardTracking(false);
    scaleSpinBox->setMinimum(0.001);
    scaleSpinBox->setDecimals(5);
    scaleSpinBox->setValue(nSca);
    connect(scaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setScale(double)));
    connect(this, SIGNAL(scaleChanged(double)), scaleSpinBox, SLOT(setValue(double)));
    scaleSpinBox->setToolTip("Изменение текущего мастаба");
//    QLabel *scaleLabel = new QLabel("Масштаб");
//    QHBoxLayout *scaleLayout = new QHBoxLayout();
//    scaleLayout->addWidget(scaleLabel);
//    scaleLayout->addWidget(scaleSpinBox);

    QDoubleSpinBox* scaleStepSpinBox = new QDoubleSpinBox();
    scaleStepSpinBox->setKeyboardTracking(false);
    scaleStepSpinBox->setMinimum(0.001);
    scaleStepSpinBox->setDecimals(5);
    scaleStepSpinBox->setValue(scaleStep);
    connect(scaleStepSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setScaleStep(double)));
    connect(this, SIGNAL(scaleStepChanged(double)), scaleStepSpinBox, SLOT(setValue(double)));
    scaleStepSpinBox->setToolTip("Изменение шага масштабирования при прокручивании колесом мыши");
//    QLabel *scaleStepLabel = new QLabel("Шаг масштабирования");
//    QHBoxLayout *scaleStepLayout = new QHBoxLayout();
//    scaleStepLayout->addWidget(scaleStepLabel);
//    scaleStepLayout->addWidget(scaleStepSpinBox);

    QDoubleSpinBox* gridStepSpinBox = new QDoubleSpinBox();
    gridStepSpinBox->setKeyboardTracking(false);
    gridStepSpinBox->setMinimum(0.0);
    gridStepSpinBox->setMaximum(1000000.0);
    gridStepSpinBox->setDecimals(5);
    gridStepSpinBox->setValue(gridStep);
    connect(gridStepSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setGridStep(double)));
    connect(this, SIGNAL(gridStepChanged(double)), gridStepSpinBox, SLOT(setValue(double)));
    gridStepSpinBox->setToolTip("Изменение шага координатной сетки");
//    QLabel *gridStepLabel = new QLabel("Шаг сетки (м)");
//    QHBoxLayout *gridStepLayout = new QHBoxLayout();
//    gridStepLayout->addWidget(gridStepLabel);
//    gridStepLayout->addWidget(gridStepSpinBox);

    QComboBox* mapsTypeComboBox = new QComboBox();
    mapsTypeComboBox->addItem("Яндекс.Карты");
    mapsTypeComboBox->addItem("Google Maps");
    mapsTypeComboBox->addItem("OpenStreetMaps");
    connect(mapsTypeComboBox, SIGNAL(activated(int)), this, SLOT(setMapType(int)));
//    QLabel *mapsTypeLabel = new QLabel("Ресурс карт");
//    QHBoxLayout *mapsTypeLayout = new QHBoxLayout();
//    mapsTypeLayout->addWidget(mapsTypeLabel);
//    mapsTypeLayout->addWidget(mapsTypeComboBox);


    QSpinBox* widthSpinBox = new QSpinBox();
    widthSpinBox->setMinimum(0);
    widthSpinBox->setMaximum(1000000);
    connect(widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setWidgetWidth(int)));
    connect(this, SIGNAL(widgetWidthChanged(int)), widthSpinBox, SLOT(setValue(int)));
    widthSpinBox->setValue(width());
    widthSpinBox->setToolTip("Ширина захватываемого участка карты");
//    QLabel *widthLabel = new QLabel("Ширина слоя (пикс)");
//    QHBoxLayout *widthLayout = new QHBoxLayout();
//    widthLayout->addWidget(widthLabel);
//    widthLayout->addWidget(widthSpinBox);

    QSpinBox* heightSpinBox = new QSpinBox();
    heightSpinBox->setMinimum(0);
    heightSpinBox->setMaximum(1000000);
    connect(heightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setWidgetHeight(int)));
    connect(this, SIGNAL(widgetHeightChanged(int)), heightSpinBox, SLOT(setValue(int)));
    heightSpinBox->setValue(height());
    heightSpinBox->setToolTip("Высота захватываемого участка карты");
//    QLabel *height = new QLabel("Высота слоя (пикс)");
//    QHBoxLayout *heightLayout = new QHBoxLayout();
//    heightLayout->addWidget(height);
//    heightLayout->addWidget(heightSpinBox);

    QSpinBox *delaySpinBox = new QSpinBox();
    delaySpinBox->setMinimum(0);
    delaySpinBox->setMaximum(1000);
    connect(delaySpinBox, SIGNAL(valueChanged(int)), this, SLOT(setDelay(int)));
    connect(this, SIGNAL(delayChanged(int)), delaySpinBox, SLOT(setValue(int)));
    delaySpinBox->setValue(delay);
    delaySpinBox->setToolTip("Пауза выставляется для случаев, когда Вы хотите захватить большой участок карты (который больше участка, отображаемого на экране).\nЭто нужно для того, чтобы изображение нужного размера подгрузилось перед его захватом.\nКонкретные значения паузы зависят от размера захватываемого участка карты и скорости соединения с сетью.");
//    QLabel *delayLabel = new QLabel("Пауза (сек)");
//    QHBoxLayout *delayLayout = new QHBoxLayout();
//    delayLayout->addWidget(delayLabel);
//    delayLayout->addWidget(delaySpinBox);

    QPushButton* saveImageButton = new QPushButton();
    connect(saveImageButton, SIGNAL(clicked(bool)), this, SLOT(saveImage()));
//    QLabel *saveLabel = new QLabel("Захват карты");
//    QHBoxLayout *saveLayout = new QHBoxLayout();
//    saveLayout->addWidget(saveLabel);
//    saveLayout->addWidget(saveImageButton);

    QRadioButton* mapsOnTopRadioButton = new QRadioButton();
//    QLabel *mapsOnTopLabel = new QLabel("Активная карта");
//    QHBoxLayout *mapsOnTopLayout = new QHBoxLayout();
//    mapsOnTopLayout->addWidget(mapsOnTopLabel);
//    mapsOnTopLayout->addWidget(mapsOnTopRadioButton);

    QRadioButton* sceneOnTopRadioButton = new QRadioButton();
//    QLabel *sceneOnTopLabel = new QLabel("Активная сцена");
//    QHBoxLayout *sceneOnTopLayout = new QHBoxLayout();
//    sceneOnTopLayout->addWidget(sceneOnTopLabel);
//    sceneOnTopLayout->addWidget(sceneOnTopRadioButton);

    connect(sceneOnTopRadioButton, SIGNAL(toggled(bool)), gridStepSpinBox, SLOT(setEnabled(bool)));
    connect(sceneOnTopRadioButton, SIGNAL(toggled(bool)), scaleSpinBox, SLOT(setEnabled(bool)));
    connect(sceneOnTopRadioButton, SIGNAL(toggled(bool)), scaleStepSpinBox, SLOT(setEnabled(bool)));
    connect(sceneOnTopRadioButton, SIGNAL(toggled(bool)), widthSpinBox, SLOT(setDisabled(bool)));
    connect(sceneOnTopRadioButton, SIGNAL(toggled(bool)), heightSpinBox, SLOT(setDisabled(bool)));
    connect(sceneOnTopRadioButton, SIGNAL(toggled(bool)), saveImageButton, SLOT(setDisabled(bool)));
    connect(sceneOnTopRadioButton, SIGNAL(toggled(bool)), mapsTypeComboBox, SLOT(setDisabled(bool)));
    connect(sceneOnTopRadioButton, SIGNAL(toggled(bool)), delaySpinBox, SLOT(setDisabled(bool)));
    connect(sceneOnTopRadioButton, SIGNAL(toggled(bool)), this, SLOT(setSceneActive(bool)));
    if (sceneActive)
        sceneOnTopRadioButton->toggle();
    else
        mapsOnTopRadioButton->toggle();

    //layout->setSpacing(1);
//    layout->addLayout(scaleLayout);
//    layout->addLayout(scaleStepLayout);
//    layout->addLayout(gridStepLayout);
//    layout->addLayout(mapsTypeLayout);
//    layout->addLayout(sceneOnTopLayout);
//    layout->addLayout(mapsOnTopLayout);
//    layout->addLayout(widthLayout);
//    layout->addLayout(heightLayout);
//    layout->addLayout(delayLayout);
//    layout->addLayout(saveLayout);
//    layout->addStretch();


    layout->addRow("Масштаб", scaleSpinBox);
    layout->addRow("Шаг масштабирования", scaleStepSpinBox);
    layout->addRow("Шаг сетки (м)", gridStepSpinBox);
    layout->addRow("Ресурс карт", mapsTypeComboBox);
    layout->addRow("Активная сцена", sceneOnTopRadioButton);
    layout->addRow("Активная карта", mapsOnTopRadioButton);
    layout->addRow("Ширина слоя (пикс)", widthSpinBox);
    layout->addRow("Высота слоя (пикс)", heightSpinBox);
    //layout->addRow("Увеличение ширины", widgetWidthScaleFactorSpinBox);
    //layout->addRow("Увеличение высоты", widgetHeightScaleFactorSpinBox);
    layout->addRow("Пауза (сек)", delaySpinBox);
    layout->addRow("Захват карты", saveImageButton);
}

void Scene2D::setShowMapsStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setShowMapsStatus(bool status)"
                                       << " status = " << status << "\n";
    showMaps = status;
    updateGL();
}

void Scene2D::setShowSceneStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setShowSceneStatus(bool status)"
                                       << " status = " << status << "\n";
    showScene = status;
    updateGL();
}

void Scene2D::setSceneActive(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setSceneActive(bool status)"
                                       << " status = " << status << "\n";
    if (sceneActive == status)
        return;
    sceneActive = status;
    int index = -1;
    if (sceneActive)
    {
        index = 0;
        if (widget->objectName() == "YandexMaps")
        {
            qobject_cast<YandexMapsView*>(widget)->setActive(false);
        } else
            if (widget->objectName() == "GoogleMaps")
            {
                qobject_cast<GoogleMapsView*>(widget)->setActive(false);
            }
    }
    else
    {
        if (widget->objectName() == "YandexMaps")
        {
            index = 1;
            qobject_cast<YandexMapsView*>(widget)->setActive(true);
        }else
            if (widget->objectName() == "GoogleMaps")
            {
                index = 2;
                qobject_cast<GoogleMapsView*>(widget)->setActive(true);
            }
        xDelta = yDelta = 0.0f;
    }
    emit currentIndexChanged(index);
}

void Scene2D::setFixedScaleStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::setFixedScaleStatus(bool status)"
                                       << " ststus = " << status << "\n";
    fixedScale = status;
}

void Scene2D::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "Scene2D::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";

}


void Scene2D::keyReleaseEvent(QKeyEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::keyReleaseEvent(QKeyEvent *pe)\n";
    if (stateManager)
        stateManager->keyReleaseEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::keyReleaseEvent(QKeyEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::keyReleaseEvent(QKeyEvent *pe) stopped\n";
        QApplication::exit(0);
    }
}


void Scene2D::contextMenuEvent(QContextMenuEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "Scene2D::contextMenuEvent(QContextMenuEvent *pe)\n";
    if (stateManager)
        stateManager->contextMenuEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D::stateManager = NULL,\n Scene2D::contextMenuEvent(QContextMenuEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
            Logger::getLogger()->errorLog() << "Scene2D::stateManager = NULL, Scene2D::contextMenuEvent(QContextMenuEvent *pe) stopped\n";
        QApplication::exit(0);
    }
}






