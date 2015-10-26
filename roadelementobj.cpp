#include "roadelementobj.h"

RoadElementOBJ::RoadElementOBJ()
{
    selected = false;
    fixed = false;
    layer = 2;
    listNumber = -1;
    deltaX = deltaY = 0.0f;
    xRot = yRot = zRot = 0.0f;
    xScale = yScale = zScale = 1.0f;
    indexOfSelectedControl = -1;
    figureList = selectedFigureList = 0;
}

RoadElementOBJ::RoadElementOBJ(float x, float y)
{
    deltaX = x;
    deltaY = y;
    selected = false;
    fixed = false;
    layer = 2;
    listNumber = -1;
    xRot = yRot = zRot = 0.0f;
    xScale = yScale = zScale = 1.0f;
    indexOfSelectedControl = -1;
    figureList = selectedFigureList = 0;
}

RoadElementOBJ::~RoadElementOBJ()
{

}



bool RoadElementOBJ::isSelected()
{
    return selected;
}

void RoadElementOBJ::setSelectedStatus(bool status)
{
    selected = status;
}

void RoadElementOBJ::drawFigure(QGLWidget *render)
{
    //glFrontFace(GL_CW);
    glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
    glPushMatrix();

    glTranslatef(deltaX, deltaY, 0.0f);

    //glRotatef(xRot, 1.0f, 0.0f, 0.0f); // поворот по X
    //glRotatef(90, 0.0f, 1.0f, 0.0f); // поворот по Y
    glRotatef(zRot, 0.0f, 0.0f, 1.0f); // поворот по Z
    //glRotatef(3000, 0.0f, 0.0f, 1.0f);
    //glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // поворот по X

    glScalef(scaleFactor, scaleFactor, scaleFactor);
    //glScalef(xScale, yScale, zScale);
    //glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);


    ////qDebug() << "Number of meshes: " << meshes.size();
    for (int i = 0; i < meshes.size(); ++i)
   {

   if (!selected)
   {
       GLfloat materialAmbient[]= { meshes[i]->Ka[0], meshes[i]->Ka[1], meshes[i]->Ka[2], 1.0f }; // Значения фонового света ( НОВОЕ )
       GLfloat materialDiffuse[]= { meshes[i]->Kd[0], meshes[i]->Kd[1], meshes[i]->Kd[2], 1.0f }; // Значения диффузного света ( НОВОЕ )
       GLfloat materialSpecular[]= { meshes[i]->Ks[0], meshes[i]->Ks[1], meshes[i]->Ks[2], 1.0f };


       if (meshes[i]->map_Ka_ID > 0)
       {
           glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, meshes[i]->map_Ka_ID);
           ////qDebug() << meshes[i]->map_Ka_ID;
       }
       else if (meshes[i]->map_Ks_ID > 0)
       {
           glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, meshes[i]->map_Ks_ID);
           ////qDebug() << meshes[i]->map_Ks_ID;
       }
       else
           if (meshes[i]->map_Kd_ID > 0)
       {
               glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, meshes[i]->map_Kd_ID);
           ////qDebug() << "Texture ID ID" <<  meshes[i]->map_Kd_ID;
       }

           glDisableClientState(GL_COLOR_ARRAY);

           glEnableClientState(GL_TEXTURE_COORD_ARRAY);
           glEnableClientState(GL_NORMAL_ARRAY);
           glEnable(GL_LIGHTING);

       glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
       glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
       glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
       glMaterialf(GL_FRONT, GL_SHININESS, meshes[i]->Ns);

       glTexCoordPointer(2, GL_FLOAT,sizeof(Vertex),&meshes[i]->vertices.data()->texture);
       glNormalPointer(GL_FLOAT, sizeof(Vertex), &meshes[i]->vertices.data()->normal);

   }
   else
   {

       glDisableClientState(GL_NORMAL_ARRAY);
       glDisable(GL_TEXTURE_2D);
       glDisableClientState(GL_TEXTURE_COORD_ARRAY);

       glEnableClientState(GL_COLOR_ARRAY);
       //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
       //glLineWidth(1.0f);
       glColorPointer(3, GL_FLOAT, sizeof(Vertex), &meshes[i]->vertices.data()->color);

   }
   glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &meshes[i]->vertices.data()->position);
   glDrawArrays(GL_TRIANGLES, 0, meshes[i]->vertices.size());

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glDisable(GL_TEXTURE_2D);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);

   glDisable(GL_LIGHTING);
   glEnableClientState(GL_COLOR_ARRAY);
}

    GLfloat materialAmbient[]= { 0.2f, 0.2f, 0.2f, 1.0f }; // Значения фонового света ( НОВОЕ )
    GLfloat materialDiffuse[]= { 0.8f, 0.8f, 0.8f, 1.0f }; // Значения диффузного света ( НОВОЕ )
    GLfloat materialSpecular[]= { 0.0f, 0.0f, 0.0f, 1.0f };


    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

    glPopMatrix();
}

void RoadElementOBJ::drawSelectionFrame()
{
}

void RoadElementOBJ::drawMeasurements(QGLWidget *render)
{
}

void RoadElementOBJ::move(float dx, float dy, float dz)
{
    deltaX += dx;
    deltaY += dy;
}

void RoadElementOBJ::drawControlElement(int index, float lineWidth, float pointSize)
{
}

QCursor RoadElementOBJ::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void RoadElementOBJ::resizeByControl(int index, float dx, float dy, float x, float y)
{
}

int RoadElementOBJ::getNumberOfControls()
{
    return 0;
}

int RoadElementOBJ::controlsForPoint()
{
    return 1;
}

void RoadElementOBJ::changeColorOfSelectedControl(int index)
{
}

void RoadElementOBJ::getProperties(QFormLayout *layout, QGLWidget *render)
{
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }

    QDoubleSpinBox* rotationSpinBox = new QDoubleSpinBox();
    rotationSpinBox->setKeyboardTracking(false);
    rotationSpinBox->setMinimum(0.0f);
    rotationSpinBox->setMaximum(360.0f);
    rotationSpinBox->setValue(zRot);
    connect(rotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZRotation(double)));
    connect(this, SIGNAL(zRotationChanged(double)), rotationSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* scaleSpinBox = new QDoubleSpinBox();
    scaleSpinBox->setKeyboardTracking(false);
    scaleSpinBox->setMinimum(0.01);
    scaleSpinBox->setDecimals(5);
    scaleSpinBox->setValue(scaleFactor);
    connect(scaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setScale(double)));
    connect(this, SIGNAL(scaleChanged(double)), scaleSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* xScaleSpinBox = new QDoubleSpinBox();
    xScaleSpinBox->setKeyboardTracking(false);
    xScaleSpinBox->setMinimum(0.01);
    xScaleSpinBox->setValue(xScale);
    connect(xScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setXScale(double)));
    connect(this, SIGNAL(xScaleChanged(double)), xScaleSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* yScaleSpinBox = new QDoubleSpinBox();
    yScaleSpinBox->setKeyboardTracking(false);
    yScaleSpinBox->setMinimum(0.01);
    yScaleSpinBox->setValue(yScale);
    connect(yScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setYScale(double)));
    connect(this, SIGNAL(yScaleChanged(double)), yScaleSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* zScaleSpinBox = new QDoubleSpinBox();
    zScaleSpinBox->setKeyboardTracking(false);
    zScaleSpinBox->setMinimum(0.01);
    zScaleSpinBox->setValue(zScale);
    connect(zScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZScale(double)));
    connect(this, SIGNAL(zScaleChanged(double)), zScaleSpinBox, SLOT(setValue(double)));

    if (render)
    {
        connect(rotationSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(scaleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(xScaleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(yScaleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(zScaleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    }

    layout->addRow("Поворот:", rotationSpinBox);
    layout->addRow("Масштабирование", scaleSpinBox);
    layout->addRow("Масштаб по X", xScaleSpinBox);
    layout->addRow("Масштаб по Y", yScaleSpinBox);
    layout->addRow("Масштаб по Z", zScaleSpinBox);
}

bool RoadElementOBJ::isFixed()
{
    return fixed;
}

int RoadElementOBJ::getLayer()
{
    return layer;
}

bool RoadElementOBJ::setFixed(bool fixed)
{
    this->fixed = fixed;
}

void RoadElementOBJ::setZRotation(double value)
{
    if (zRot != value)
    {
        zRot = value;
        emit zRotationChanged(value);
    }
}

void RoadElementOBJ::setXScale(double value)
{
    if (xScale != value)
    {
        xScale = value;
        emit xScaleChanged(xScale);
    }
}

void RoadElementOBJ::setYScale(double value)
{
    if (yScale != value)
    {
        yScale = value;
        emit yScaleChanged(yScale);
    }
}

void RoadElementOBJ::setZScale(double value)
{
    if (zScale != value)
    {
        zScale = value;
        emit zScaleChanged(zScale);
    }
}

void RoadElementOBJ::setScale(double scale)
{
    if (scale != scaleFactor)
    {
        scaleFactor = scale;
        emit scaleChanged(scaleFactor);
    }
}


void RoadElementOBJ::clear()
{
}
