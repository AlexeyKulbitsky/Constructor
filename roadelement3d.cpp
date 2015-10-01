#include "roadelement3d.h"
#include <QSlider>
RoadElement3D::RoadElement3D()
{
    selected = false;
    fixed = false;
    layer = 2;
    listNumber = -1;
    deltaX = deltaY = 0.0f;
    xRot = yRot = zRot = 0.0f;
    xScale = yScale = 1.0f;
    indexOfSelectedControl = -1;
    //bufferVBO = NULL;
}

RoadElement3D::RoadElement3D(float x, float y)
{
    selected = false;
    fixed = false;
    layer = 2;
    listNumber = -1;
    deltaX = x;
    deltaY = y;
    xRot = yRot = zRot = 0.0f;
    xScale = yScale = 1.0f;
    indexOfSelectedControl = -1;
}


bool RoadElement3D::isSelected()
{
    return selected;
}

void RoadElement3D::setSelectedStatus(bool status)
{
    selected = status;
}

void RoadElement3D::drawFigure(QGLWidget *render)
{
    //glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPushMatrix();

    glTranslatef(deltaX, deltaY, 0.0f);

    //glRotatef(xRot, 1.0f, 0.0f, 0.0f); // поворот по X
    //glRotatef(yRot, 0.0f, 1.0f, 0.0f); // поворот по Y
    glRotatef(zRot, 0.0f, 0.0f, 1.0f); // поворот по Z
    //glRotatef(3000, 0.0f, 0.0f, 1.0f);
    /////////////glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // поворот по X

    glScalef(xScale, yScale, 1.0f);


    /*

         for (int i = 0; i < meshes.size(); ++i)
        {
        if (!selected)
        {
            GLfloat materialAmbient[]= { meshes[i]->Ka[0], meshes[i]->Ka[1], meshes[i]->Ka[2], 1.0f }; // Значения фонового света ( НОВОЕ )
            GLfloat materialDiffuse[]= { meshes[i]->Kd[0], meshes[i]->Kd[1], meshes[i]->Kd[2], 1.0f }; // Значения диффузного света ( НОВОЕ )
            GLfloat materialSpecular[]= { meshes[i]->Ks[0], meshes[i]->Ks[1], meshes[i]->Ks[2], 1.0f };

            if (meshes[i]->map_Ka_ID > 0)
                glBindTexture(GL_TEXTURE_2D, meshes[i]->map_Ka_ID);
            else if (meshes[i]->map_Ks_ID > 0)
                glBindTexture(GL_TEXTURE_2D, meshes[i]->map_Ks_ID);
            else if (meshes[i]->map_Kd_ID > 0)
                glBindTexture(GL_TEXTURE_2D, meshes[i]->map_Kd_ID);

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
            glColorPointer(3, GL_FLOAT, sizeof(Vertex), &meshes[i]->vertices.data()->color);
        }
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &meshes[i]->vertices.data()->position);
        glDrawArrays(GL_TRIANGLES, 0, meshes[i]->vertices.size());
    }

    qDebug() << "meshes: " << meshes.size();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_LIGHTING);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (selected)
        drawControlElement(0, 0.0f, 10.0f);
    */


    //glDisable(GL_TEXTURE_2D);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisable(GL_LIGHTING);
    //glDisableClientState(GL_NORMAL_ARRAY);

    //glColor3f(0.3f, 0.3f, 0.3f);


    qDebug() << "Number of meshes " << meshes.size();
    if (!selected)
    {
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);

        //glEnable(GL_TEXTURE_2D);
        //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_LIGHTING);

        qDebug() << "NUMBER OF MESHES: " << meshes.size();
        for (int i = 0; i < meshes.size(); ++i)
        {


            glVertexPointer(3, GL_FLOAT,0, meshes[i]->vertices.begin());
            //glTexCoordPointer(2, GL_FLOAT,0,meshes[i]->textures.begin());
            qDebug() << "Drawing mesh: " << i;
            qDebug() << "Number of material groups: " << meshes[i]->materialMeshes.size();
            for (int j = 0; j < meshes[i]->materialMeshes.size(); ++j)
            {
                GLfloat materialAmbient[]= { materials[meshes[i]->materialMeshes[j].materialName].ambientColor[0],
                                             materials[meshes[i]->materialMeshes[j].materialName].ambientColor[1],
                                             materials[meshes[i]->materialMeshes[j].materialName].ambientColor[2],
                                             1.0f }; // Значения фонового света ( НОВОЕ )
                GLfloat materialDiffuse[]= { materials[meshes[i]->materialMeshes[j].materialName].diffuseColor[0],
                                             materials[meshes[i]->materialMeshes[j].materialName].diffuseColor[1],
                                             materials[meshes[i]->materialMeshes[j].materialName].diffuseColor[2],
                                             1.0f }; // Значения диффузного света ( НОВОЕ )
                GLfloat materialSpecular[]= { materials[meshes[i]->materialMeshes[j].materialName].specularColor[0],
                                              materials[meshes[i]->materialMeshes[j].materialName].specularColor[1],
                                              materials[meshes[i]->materialMeshes[j].materialName].specularColor[2],
                                              1.0f };

                glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

                if (materials[meshes[i]->materialMeshes[j].materialName].textureID > 0)
                {
                    glEnable(GL_TEXTURE_2D);
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    glTexCoordPointer(2, GL_FLOAT,0,meshes[i]->textures.begin());
                    glBindTexture(GL_TEXTURE_2D, materials[meshes[i]->materialMeshes[j].materialName].textureID);
                }
                else
                {
                    qDebug() << "No texture";
                    //glDisable(GL_LIGHTING);
                }
                // qDebug() << meshes[i]->materialMeshes[j].faces.size();

                glDrawElements(GL_TRIANGLES,meshes[i]->materialMeshes[j].faces.size(),GL_UNSIGNED_SHORT,meshes[i]->materialMeshes[j].faces.begin());
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisable(GL_TEXTURE_2D);
            }
            //glColorPointer(3, GL_FLOAT,0,meshes[i]->colors.begin());

            glDisable(GL_TEXTURE_2D);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_LIGHTING);
            glEnableClientState(GL_COLOR_ARRAY);

        }
    } else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);
        glColor3f(0.0f,0.0f,0.0f);
        for (int i = 0; i < meshes.size(); ++i)
        {

            glColorPointer(3, GL_FLOAT, 0, meshes[i]->colors.begin());
            glVertexPointer(3, GL_FLOAT,0, meshes[i]->vertices.begin());
            glDrawElements(GL_TRIANGLES,
                            meshes[i]->faces.size(),
                            GL_UNSIGNED_SHORT,
                            meshes[i]->faces.begin());
            glPointSize(7.0f);
            //glDrawArrays(GL_POINTS,0,meshes[i]->vertices.size());
            if (indexOfSelectedControl >= 0 && indexOfSelectedControl < getNumberOfControls())
                drawControlElement(indexOfSelectedControl, 5.0f, 10.0f);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glPopMatrix();
}

void RoadElement3D::drawSelectionFrame()
{
}

void RoadElement3D::drawMeasurements(QGLWidget *render)
{
}

void RoadElement3D::move(float dx, float dy, float dz)
{
    deltaX += dx;
    deltaY += dy;
}

void RoadElement3D::drawControlElement(int index, float lineWidth, float pointSize)
{
    /*
    if (index == 0)
    {
        glPushMatrix();
        glTranslatef(deltaX, deltaY, 0.0f);
        glRotatef(zRot, 0.0f, 0.0f, 1.0f);

        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 4.0f, 0.5f);
        glEnd();
        glPopMatrix();
    }
    */

  if (index < 0)
      return;
    int i;
    for (i = 0; i < meshes.size(); ++i)
    {
        if (index >= meshes[i]->vertices.size() / 3)
            index -= meshes[i]->vertices.size() / 3;
        else
            break;
    }
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(deltaX, deltaY, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f); // поворот по Z
    glScalef(xScale, yScale, 1.0f);

    glPointSize(pointSize);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(meshes[i]->vertices[index * 3],
            meshes[i]->vertices[index * 3 + 1],
            meshes[i]->vertices[index * 3 + 2]);
    glEnd();

    glPopMatrix();

}

QCursor RoadElement3D::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void RoadElement3D::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (index == 0)
        zRot += (GLfloat)(atan(dy/dx));
   // qDebug() << "Rotating element";
   // qDebug() << "Angel: " << zRot;
}

int RoadElement3D::getNumberOfControls()
{
    int count = 0;
    for (int i = 0; i < meshes.size(); ++i)
        count += meshes[i]->vertices.size() / 3;
    //qDebug() << count;
    return count;
}

int RoadElement3D::controlsForPoint()
{
    return 1;
}

void RoadElement3D::changeColorOfSelectedControl(int index)
{
    indexOfSelectedControl = index;
}

void RoadElement3D::getProperties(QFormLayout *layout, QGLWidget *render)
{
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }

    //QSlider *rotationSlider = new QSlider();
    //rotationSlider->setMinimum(0);
    //rotationSlider->setMaximum(360);

    QDoubleSpinBox* rotationSpinBox = new QDoubleSpinBox();
    rotationSpinBox->setMinimum(0.0f);
    rotationSpinBox->setMaximum(360.0f);
    rotationSpinBox->setValue(zRot);

    connect(rotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZRotation(double)));
    connect(this, SIGNAL(zRotationChanged(double)), rotationSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* xScaleSpinBox = new QDoubleSpinBox();
    xScaleSpinBox->setValue(xScale);
    connect(xScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setXScale(double)));
    connect(this, SIGNAL(xScaleChanged(double)), xScaleSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* yScaleSpinBox = new QDoubleSpinBox();
    yScaleSpinBox->setValue(yScale);
    connect(yScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setYScale(double)));
    connect(this, SIGNAL(yScaleChanged(double)), yScaleSpinBox, SLOT(setValue(double)));

    //rotationSlider->setValue(this->zRot);
    //connect(rotationSlider, SIGNAL(sliderMoved(int)), this, SLOT(setZRotation(int)));
    //connect(this, SIGNAL(zRotationChanged(int)), rotationSlider, SLOT(setValue(int)));

    layout->addRow("Поворот:", rotationSpinBox);
    layout->addRow("Растяжение по X:", xScaleSpinBox);
    layout->addRow("Растяжение по Y:", yScaleSpinBox);
}

bool RoadElement3D::isFixed()
{
    return fixed;
}

int RoadElement3D::getLayer()
{
    return layer;
}

void RoadElement3D::setVertexArray(QVector<GLfloat> &vertexArray)
{
    this->vertexArray.clear();
    //this->vertexArray = vertexArray;
}

void RoadElement3D::setTextureArray(QVector<GLfloat> &textureArray)
{
    this->textureArray.clear();
    //this->textureArray = textureArray;
}

void RoadElement3D::setNormalArray(QVector<GLfloat> &normalArray)
{
    this->normalArray.clear();
    //this->normalArray = normalArray;
}

void RoadElement3D::setIndexArray(QVector<GLubyte> &indexArray)
{
    //this->indexArray.clear();
    //this->indexArray = indexArray;
}

void RoadElement3D::setColorArray(float red, float green, float blue)
{
    //colorArray.clear();
    //for (int i = 0; i < vertexArray.size() / 3; ++i)
    //{
    //    colorArray.push_back(red);
    //    colorArray.push_back(green);
    //    colorArray.push_back(blue);
    //}
}

void RoadElement3D::getTextures(const char *textureSource)
{
    QImage image1;

    image1.load(textureSource);
    image1 = QGLWidget::convertToGLFormat(image1);
    glGenTextures(1, textureID);
    // создаём и связываем 1-ый текстурный объект с последующим состоянием текстуры
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    // связываем текстурный объект с изображением
    glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei)image1.width(), (GLsizei)image1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.bits());

    // задаём линейную фильтрацию вблизи:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // задаём линейную фильтрацию вдали:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // задаём: цвет текселя полностью замещает цвет фрагмента фигуры
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

bool RoadElement3D::setFixed(bool fixed)
{
    this->fixed = fixed;
}

void RoadElement3D::setZRotation(double value)
{
    if (zRot != value)
    {
        zRot = value;
        emit zRotationChanged(zRot);
    }
}

void RoadElement3D::setXScale(double value)
{
    if (xScale != value)
    {
        xScale = value;
        emit xScaleChanged(xScale);
    }
}

void RoadElement3D::setYScale(double value)
{
    if (yScale != value)
    {
        yScale = value;
        emit yScaleChanged(yScale);
    }
}


std::vector<vec3> RoadElement3D::getCoordOfControl(int index)
{
      int i;
      for (i = 0; i < meshes.size(); ++i)
      {
          if (index >= meshes[i]->vertices.size() / 3)
              index -= meshes[i]->vertices.size() / 3;
          else
              break;
      }

    vec3 p(0.0f, 0.0f, 0.0f);
    if (index >= 0 && index < getNumberOfControls())
    {
        p.x = meshes[i]->vertices[index * 3];
        p.y = meshes[i]->vertices[index * 3 + 1];
        p.z = meshes[i]->vertices[index * 3 + 2];
    }
    std::vector<vec3> res;
    res.push_back(p);
    return res;
}


void RoadElement3D::clear()
{
}
