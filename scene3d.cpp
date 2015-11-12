#include "scene3d.h"
#include <QtGui>
#include <math.h>
#include <QColorDialog>

bool Scene3D::log = true;

Scene3D::Scene3D(QWidget *parent, QGLWidget* shared) : QGLWidget(parent, shared)
{

    xRot = yRot = zRot = 0.0f;
    xTra = yTra = 0.0f;
    zTra = -3.0f;
    nSca = 0.5f;
    xDelta = 0.0f;
    yDelta = 0.0f;
    model = NULL;
    stateManager = NULL;
    leftButtonIsPressed = false;
    rightButtonIsPressed = false;
    middleButtonIsPressed = false;

}

Scene3D::~Scene3D()
{
    stateManager = NULL;
    model = NULL;
}


void Scene3D::setModel(Model *model)
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::setModel(Model *model)\n";
    if (model != NULL)
        this->model = model;
    else
    {
        QMessageBox::critical(0, "Ошибка", "Scene3D::setModel(Model *model), model = NULL,\n cannot work with models", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "Scene3D::setModel(Model *model), model = NULL, cannot work with models\n";
        QApplication::exit(0);
    }
}


void Scene3D::scalePlus()
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::scale_plus()\n";
    nSca = nSca * 1.05;
}

void Scene3D::scaleMinus()
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::scale_minus()\n";
    nSca = nSca / 1.05;
}

void Scene3D::rotateUp()
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::rotate_up()\n";
    xRot += 1.0;
}

void Scene3D::rotateDown()
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::rotate_down()\n";
    xRot -= 1.0;
}

void Scene3D::rotateLeft()
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::rotate_left()\n";
    zRot += 1.0;
}

void Scene3D::rotateRight()
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::rotate_right()\n";
    zRot -= 1.0;
}

void Scene3D::getProperties(QFormLayout *layout)
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::getProperties(QFormLayout *layout)\n";
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Scene3D::getProperties(QFormLayout *layout) layout = NULL",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "Scene3D::getProperties(QFormLayout *layout) layout = NULL\n";
        QApplication::exit(0);
    }
    this->layout = layout;
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }

    QPushButton* backgroundColorButton = new QPushButton();
    QString style = "background: rgba(%1, %2, %3, %4);";
    backgroundColorButton->setStyleSheet(style.arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue()).arg(backgroundColor.alpha()));
    connect(backgroundColorButton, SIGNAL(clicked(bool)), this, SLOT(setBackGroundColor()));

    layout->addRow("Цвет фона", backgroundColorButton);
}


void Scene3D::mousePressEvent(QMouseEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::mousePressEvent(QMouseEvent *pe)\n";
    ptrMousePosition = pe->pos();
    switch (pe->button())
    {
    case Qt::RightButton:
        rightButtonIsPressed = true;
        break;
    case Qt::LeftButton:
        leftButtonIsPressed = true;
        break;
    case Qt::MiddleButton:
        middleButtonIsPressed = true;
    default:
        break;
    }
}

void Scene3D::mouseReleaseEvent(QMouseEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::mouseReleaseEvent(QMouseEvent *pe)\n";
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    middleButtonIsPressed = false;
}

void Scene3D::mouseMoveEvent(QMouseEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::mouseMoveEvent(QMouseEvent *pe)\n";
    if (rightButtonIsPressed)
    {
        // Поворот сцены
        //xRot += 180/nSca*(GLfloat)(pe->y()-ptrMousePosition.y())/height(); // вычисляем углы поворота
        //zRot += 180/nSca*(GLfloat)(pe->x()-ptrMousePosition.x())/width();

        xRot += 180*(GLfloat)(pe->y()-ptrMousePosition.y())/height(); // вычисляем углы поворота
        zRot += 180*(GLfloat)(pe->x()-ptrMousePosition.x())/width();
    }
    if (leftButtonIsPressed)
    {

    }
    if (middleButtonIsPressed)
    {
        // Смещение сцены
        xTra += (GLfloat)(pe->x()-ptrMousePosition.x())/width()/
                ratio * 2.0f;
        yTra += (GLfloat)(-1)*(pe->y()-ptrMousePosition.y())/width()/
                ratio * 2.0f;
    }
    ptrMousePosition = pe->pos();
    updateGL();
}

void Scene3D::wheelEvent(QWheelEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::wheelEvent(QWheelEvent *pe)\n";
    if ((pe->delta())>0) scalePlus();
    else
        if ((pe->delta())<0) scaleMinus();
    updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent *)
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::keyPressEvent(QKeyEvent *)\n";
}

void Scene3D::keyReleaseEvent(QKeyEvent *)
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::keyReleaseEvent(QKeyEvent *)\n";
}

void Scene3D::initializeGL()
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::initializeGL()\n";
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
    //glCullFace (GL_FRONT_AND_BACK);
    //glEnable(GL_FRONT_AND_BACK);
    //glFrontFace (GL_CW);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glEnable(GL_LIGHTING);

    // Разрешение источника света номер один
    //glEnable(GL_LIGHT0);
    /*
    glEnable(GL_COLOR_MATERIAL);
    float pos[4] = {30,30,30,0.5};
    float dir[3] = {-1,-1,-1};
    GLfloat mat_specular[] = {1,1,1,1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0);


    glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, 0);
    glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 90);
    */

}

void Scene3D::resizeGL(int nWidth, int nHeight)
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::resizeGL(int nWidth, int nHeight)"
                                   << " nWidth = " << nWidth
                                   << " nHeight = " << nHeight << "\n";
    /*
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ratio=(GLfloat)nHeight/(GLfloat)nWidth;

    if (nWidth>=nHeight)
        //gluPerspective(45,1.0/ratio,1.0,500.0);
        //glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -100.0, 100.0);
        gluPerspective(45.0f, 1.0f/ratio, 1.0f, 1000.0f);
    //glFrustum(-1.0/ratio, 1.0/ratio, -1.0, 1.0, 100.0, -100.0);
    else
        //gluPerspective(45,ratio,1.0,500.0);
       //glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -100.0, 100.0);
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    //glFrustum(-1.0, 1.0, -1.0*ratio, 1.0*ratio, 100.0, -100.0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glGetIntegerv(GL_VIEWPORT, viewport); // извлечь матрицу поля просмотра в viewport
    */

    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        ratio=(GLfloat)nHeight/(GLfloat)nWidth;
        gluPerspective(70.0f, 1.0f/ratio, 0.1f, 1000.0f);


        //glMatrixMode(GL_MODELVIEW);


}

void Scene3D::paintGL()
{
    if (log)
    Logger::getLogger()->infoLog() << "Scene3D::paintGL()\n";
    //glClearColor(0.9, 0.9, 0.9, 1);
    qglClearColor(backgroundColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Позиция света


    //glEnable(GL_LIGHT1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения фонового света ( НОВОЕ )
    GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения фонового света ( НОВОЕ )
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения диффузного света ( НОВОЕ )
    GLfloat LightPosition[]= { 0.0f, 0.0f, 2000.0f, 0.0f };     // Позиция света ( НОВОЕ )


    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // Установка Фонового Света
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);    // Установка Диффузного Света
    glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

    glTranslatef(xTra, yTra, zTra);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f); // поворот по X
    glRotatef(yRot, 0.0f, 1.0f, 0.0f); // поворот по Y
    glRotatef(zRot, 0.0f, 0.0f, 1.0f); // поворот по Z
    glScalef(nSca, nSca, nSca);

    if (model)
    {
        for (int i =  0; i < model->getNumberOfGroups(); ++i)
        {
            if (model->isGroupVisible(i) == true)
            {
                for(QList<RoadElement*>::iterator it = model->getGroup(i).begin();
                    it != model->getGroup(i).end(); ++it)
                    (*it)->drawFigure(this);
            }

        }
    }


    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHTING);

    glPopMatrix();
}

void Scene3D::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "Scene3D::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}

void Scene3D::setBackGroundColor()
{
    backgroundColor = QColorDialog::getColor(backgroundColor);
    getProperties(layout);
    updateGL();
}

