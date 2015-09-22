#include "scene3d.h"
#include <QtGui>
#include <math.h>

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

}

void Scene3D::setModel(Model *model)
{
    this->model = model;
}

void Scene3D::scale_plus()
{
    nSca = nSca * 1.05;
}

void Scene3D::scale_minus()
{
    nSca = nSca / 1.05;
}

void Scene3D::rotate_up()
{
    xRot += 1.0;
}

void Scene3D::rotate_down()
{
    xRot -= 1.0;
}

void Scene3D::rotate_left()
{
    zRot += 1.0;
}

void Scene3D::rotate_right()
{
    zRot -= 1.0;
}


void Scene3D::mousePressEvent(QMouseEvent *pe)
{
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
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    middleButtonIsPressed = false;
}

void Scene3D::mouseMoveEvent(QMouseEvent *pe)
{
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
    if ((pe->delta())>0) scale_plus();
    else
        if ((pe->delta())<0) scale_minus();
    updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent *)
{
}

void Scene3D::keyReleaseEvent(QKeyEvent *)
{
}

void Scene3D::initializeGL()
{

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
    glClearColor(0.9, 0.9, 0.9, 1);
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





    // масштабирование
    if (model)
    {
        for (int i =  0; i < model->getNumberOfGroups(); ++i)
        {
            if (model->isGroupVisible(i) == true)
            {
                for(std::list<RoadElement*>::iterator it = model->getGroup(i).begin();
                    it != model->getGroup(i).end(); ++it)
                    (*it)->drawFigure(this);
            }

        }
    }
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHTING);

    glPopMatrix();
}

