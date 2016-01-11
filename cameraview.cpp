#include "cameraview.h"
#include "model.h"
#include "camera.h"


CameraView::CameraView(QWidget *parent, QGLWidget *shared) : QGLWidget(parent, shared)
{
    m_height = 0.0f;
    m_horizontalAngle = 0.0f;
    m_verticalAngle = 0.0f;
    dx = dy = 0.0f;
    active = false;
    camera = NULL;
    firstTime = true;
    deltaAngle = 0.0f;
}



void CameraView::getProperties(QFormLayout *layout)
{


}

void CameraView::setModel(Model *model)
{
    this->model = model;
}

void CameraView::setTranslation(float dx, float dy, float dz)
{
    this->dx = dx;
    this->dy = dy;
    m_height = dz;
    emit heightChanged(m_height);
}

void CameraView::translate(float dx, float dy, float dz)
{
    this->dx += dx;
    this->dy += dy;
    m_height += dz;
    emit heightChanged(m_height);
}

void CameraView::setProjection(float FOV, float aspectRatio, float zNear, float zFar)
{
    m_FOV = FOV;
    m_aspectRatio = aspectRatio;
    m_zNear = zNear;
    m_zFar = zFar;

    if (active)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(m_FOV, m_aspectRatio, m_zNear, m_zFar);
        updateGL();
    }
}

void CameraView::setActive(bool status)
{
    active = status;
    if (active)
    {
        firstTime = true;
        if (camera)
        {
            dx = camera->getDX();
            dy = camera->getDY();
            m_height = camera->getHeight();
            m_horizontalAngle = camera->getHorizontalAngle();
            m_verticalAngle = camera->getVerticalAngle();
            m_FOV = camera->getFOV();
            deltaAngle = camera->getDeltaAngle();
            m_aspectRatio = camera->getAspectRatio();

        }
        else
        {
            dx = 0.0f;
            dy = 0.0f;
            m_height = 0.0f;
            m_horizontalAngle = 0.0f;
            m_verticalAngle = 0.0f;
            m_FOV = 0.0f;
            deltaAngle = 0.0f;
            m_aspectRatio = 0.0f;
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(m_FOV, m_aspectRatio, m_zNear, m_zFar);
        updateGL();
    }
    else
    {
        dx = 0.0f;
        dy = 0.0f;
        m_height = 0.0f;
        m_horizontalAngle = 0.0f;
        m_verticalAngle = 0.0f;
        m_FOV = 0.0f;
        deltaAngle = 0.0f;
        m_aspectRatio = 0.0f;
    }
}

void CameraView::setCamera(Camera *camera)
{
    this->camera = camera;
}


void CameraView::setHeight(double value)
{
    if (m_height == value)
        return;
    m_height = value;
    if (active)
    {
        updateGL();
    }
    emit heightChanged(value);
}

void CameraView::setDX(double value)
{
    if (dx == value)
        return;
    dx = value;
    if (active)
    {
        updateGL();
    }
    emit dxChanged(value);
}

void CameraView::setDY(double value)
{
    if (dy == value)
        return;
    dy = value;
    if (active)
    {
        updateGL();
    }
    emit dyChanged(value);
}

void CameraView::setHorizontalAngle(double value)
{
    if (m_horizontalAngle == value)
        return;
    m_horizontalAngle = value;
    if (active)
    {
        updateGL();
    }
    emit horizontalAngleChanged(value);
}

void CameraView::setVerticalAngle(double value)
{
    if (m_verticalAngle == value)
        return;
    m_verticalAngle = value;
    if (active)
    {
        updateGL();
    }
    emit verticalAngleChanged(value);
}

void CameraView::setFOV(double value)
{
    if (m_FOV == value)
        return;
    m_FOV = value;

    if (active)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(m_FOV, m_aspectRatio, m_zNear, m_zFar);
        updateGL();
    }
}

void CameraView::setAspectRatio(double value)
{
    m_aspectRatio = value;
    if (active)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(m_FOV, m_aspectRatio, m_zNear, m_zFar);
        updateGL();
    }
}

void CameraView::setDeltaAngle(double value)
{
    deltaAngle = value;
}


void CameraView::initializeGL()
{
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
}

void CameraView::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_FOV, m_aspectRatio, m_zNear, m_zFar);
}

void CameraView::paintGL()
{
    if (firstTime)
    {
        resizeGL(width(), height());
        firstTime = false;
    }
    glClearColor(0.9, 0.9, 0.9, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(m_verticalAngle/* - deltaAngle*/, 1.0f, 0.0f, 0.0f); // поворот по X
    glRotatef(-m_horizontalAngle, 0.0f, 0.0f, 1.0f); // поворот по Z
    glTranslatef(-dx, -dy, -m_height);

    if (model)
    {
        for (int i =  0; i < model->getNumberOfGroups(); ++i)
        {
            if (i == 3)
                continue;
            if (model->isGroupVisible(i) == true)
            {
                for(QList<RoadElement*>::iterator it = model->getGroup(i).begin();
                    it != model->getGroup(i).end(); ++it)
                {
                    bool selected = (*it)->isSelected();
                    (*it)->setSelectedStatus(false);
                    (*it)->drawFigure(this);
                    (*it)->setSelectedStatus(selected);
                }
            }
        }
    }

    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHTING);

    glPopMatrix();
}


void CameraView::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
        leftButton = true;
    mousePosition = ev->pos();
}

void CameraView::mouseReleaseEvent(QMouseEvent *ev)
{
    leftButton = false;
    mousePosition = ev->pos();
}

void CameraView::mouseMoveEvent(QMouseEvent *ev)
{
    if (leftButton && !camera->isFixed())
    {
        m_verticalAngle += 180*(GLfloat)(ev->y()-mousePosition.y())/height(); // вычисляем углы поворота
        m_horizontalAngle += 180*(GLfloat)(ev->x()-mousePosition.x())/width();

        if (camera)
        {
            camera->setHorizontalAngle(m_horizontalAngle);
            camera->setVerticalAngle(m_verticalAngle);
        }
    }

    mousePosition = ev->pos();
    updateGL();
}



void CameraView::wheelEvent(QWheelEvent *)
{
//    if (camera)
//        QCoreApplication::sendEvent(camera->focalLengthSpinBox, pe);
}
