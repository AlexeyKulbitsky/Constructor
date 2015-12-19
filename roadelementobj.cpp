#include "roadelementobj.h"

RoadElementOBJ::RoadElementOBJ()
{
    name = "RoadElementOBJ";
    selected = false;
    fixed = false;
    layer = 2;
    listNumber = -1;
    deltaX = deltaY = 0.0f;
    xRot = yRot = zRot = 0.0f;
    xScale = yScale = zScale = 1.0f;
    indexOfSelectedControl = -1;
    figureList = selectedFigureList = 0;
    deltaZ = 0.0f;
}

RoadElementOBJ::RoadElementOBJ(float x, float y, QString folder, QString filename)
{
    name = "RoadElementOBJ";
    deltaX = x;
    deltaY = y;
    elementX = deltaX;
    elementY = deltaY;
    selected = false;
    fixed = false;
    layer = 2;
    listNumber = -1;
    xRot = yRot = zRot = 0.0f;
    xScale = yScale = zScale = 1.0f;
    indexOfSelectedControl = -1;
    figureList = selectedFigureList = 0;
    zRadius = 2.0f;
    this->folder = folder;
    this->filename = filename;
    deltaZ = 0.0f;
    setZRotVertexArray();
    setZRotColorArray(0.0f, 1.0f, 0.0f);
    setZRotIndexArray();
}

RoadElementOBJ::RoadElementOBJ(const RoadElementOBJ &source)
{
    name = "RoadElementOBJ";
    deltaX = source.deltaX;
    deltaY = source.deltaY;
    elementX = deltaX;
    elementY = deltaY;
    selected = source.selected;
    fixed = source.fixed;
    layer = 2;
    listNumber = -1;
    xRot = source.xRot;
    yRot = source.yRot;
    zRot = source.zRot;
    xScale = source.xScale;
    yScale = source.yScale;
    zScale = source.zScale;
    indexOfSelectedControl = -1;
    figureList = selectedFigureList = 0;
    zRadius = 2.0f;
    this->folder = source.folder;
    this->filename = source.filename;
    scaleFactor = source.scaleFactor;
    deltaZ = source.deltaZ;
    setZRotVertexArray();
    setZRotColorArray(0.0f, 1.0f, 0.0f);
    setZRotIndexArray();

    for (int i = 0; i < source.meshes.size(); ++i)
    {
        Mesh* mesh = new Mesh();
        strncpy(mesh->name, source.meshes[i]->name,20);
        mesh->Ka[0] = source.meshes[i]->Ka[0];
        mesh->Ka[1] = source.meshes[i]->Ka[1];
        mesh->Ka[2] = source.meshes[i]->Ka[2];

        mesh->Kd[0] = source.meshes[i]->Kd[0];
        mesh->Kd[1] = source.meshes[i]->Kd[1];
        mesh->Kd[2] = source.meshes[i]->Kd[2];

        mesh->Ks[0] = source.meshes[i]->Ks[0];
        mesh->Ks[1] = source.meshes[i]->Ks[1];
        mesh->Ks[2] = source.meshes[i]->Ks[2];

        mesh->d = source.meshes[i]->d;
        mesh->Ns = source.meshes[i]->Ns;
        mesh->illum = source.meshes[i]->illum;

        strncpy(mesh->map_Ka, source.meshes[i]->map_Ka, 255);
        strncpy(mesh->map_Kd, source.meshes[i]->map_Kd, 255);
        strncpy(mesh->map_Ks, source.meshes[i]->map_Ks, 255);

        mesh->map_Ka_ID = source.meshes[i]->map_Ka_ID;
        mesh->map_Kd_ID = source.meshes[i]->map_Kd_ID;
        mesh->map_Ks_ID = source.meshes[i]->map_Ks_ID;


        for (int j = 0; j < source.meshes[i]->vertices.size(); ++j)
        {
            Vertex vertex;
            for (int k = 0; k < 3; ++k)
            {
                vertex.position[k] = source.meshes[i]->vertices[j].position[k];
                vertex.color[k] = source.meshes[i]->vertices[j].color[k];
                vertex.normal[k] = source.meshes[i]->vertices[j].normal[k];
            }
            vertex.texture[0] = source.meshes[i]->vertices[j].texture[0];
            vertex.texture[1] = source.meshes[i]->vertices[j].texture[1];
            mesh->vertices.push_back(vertex);
        }
        meshes.push_back(mesh);
    }


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

    glTranslatef(deltaX, deltaY, deltaZ);

    //glRotatef(xRot, 1.0f, 0.0f, 0.0f); // поворот по X
    //glRotatef(90, 0.0f, 1.0f, 0.0f); // поворот по Y
    glRotatef(zRot, 0.0f, 0.0f, 1.0f); // поворот по Z
    //glRotatef(3000, 0.0f, 0.0f, 1.0f);
    //glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // поворот по X

    glScalef(scaleFactor, scaleFactor, scaleFactor);
    //glScalef(xScale, yScale, zScale);
    //glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);


    //qDebug() << "Number of meshes: " << meshes.size();
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
    if (selected)
    {
        glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);
    }
}

void RoadElementOBJ::drawSelectionFrame()
{
    for (int i = 0; i < getNumberOfControls(); ++i)
    {
        drawControlElement(i, 5.0f, 10.0f);
    }
}

void RoadElementOBJ::drawMeasurements(QGLWidget *render)
{
}

void RoadElementOBJ::move(float dx, float dy, float dz)
{
    deltaX += dx;
    deltaY += dy;
    elementX = deltaX;
    elementY = deltaY;
}

void RoadElementOBJ::drawControlElement(int index, float lineWidth, float pointSize)
{
    glPushMatrix();
    glTranslatef(deltaX, deltaY, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f); // поворот по Z
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    switch (index)
    {
    case 0:
//        glPointSize(pointSize + 5.0f);
//        glBegin(GL_POINTS);
//        glColor3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, yMax, 0.0f);
//        glEnd();
        glLineWidth(lineWidth);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glEnableClientState(GL_COLOR_ARRAY);

        glColorPointer(3, GL_FLOAT,0,zRotColorArray.begin());
        glVertexPointer(3, GL_FLOAT, 0, zRotVertexArray.begin());
        glDrawElements(GL_LINE_STRIP,zRotIndexArray.size(),GL_UNSIGNED_INT,zRotIndexArray.begin());
        break;
    case 1:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
    }
    default:
        break;
    }

    glPopMatrix();
}

QCursor RoadElementOBJ::getCursorForControlElement(int index)
{    
    switch(index)
    {
        case 0:
            return Qt::CrossCursor;
        case 1:
            return Qt::CrossCursor;
        default:
            return Qt::ArrowCursor;
    }


}

void RoadElementOBJ::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
    {
        return;
    }
    switch (index)
    {
    case 0:
    {
        float X1 = 0.0f;
        float Y1 = zRadius;
        float X2 = 0.0f;
        float Y2 = 0.0f;
        float X3 = X1 + dx;
        float Y3 = Y1 + dy;
        float dx1 = X1 - X2;
        float dy1 = Y1 - Y2;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = X3 - X2;
        float dy2 = Y3 - Y2;
        float r2 = sqrt(dx2*dx2 + dy2*dy2);
        float pi = 3.14159265f;
        float t = dx1 / r1;
        if (t > 1)
            t = 1.0f;
        if (t < -1)
            t = -1.0f;
        float angle1 = acos(t);
        if (dy1 < 0)
            angle1 = 2.0f * pi - angle1;
        t = dx2 / r2;
        if (t > 1)
            t = 1.0f;
        if (t < -1)
            t = -1.0f;
        float angle2 = acos(t);
        if (dy2 < 0)
            angle2 = 2.0f * pi - angle2;
        float angle = angle2 - angle1;
        setZRotation(zRot + angle * 180.0f / pi);
    }
        break;
    default:
        break;
    }
}

int RoadElementOBJ::getNumberOfControls()
{
    return 2;
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
    clearProperties(layout);
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

    QDoubleSpinBox* zTranslationSpinBox = new QDoubleSpinBox();
    zTranslationSpinBox->setKeyboardTracking(false);
    zTranslationSpinBox->setValue(deltaZ);
    connect(zTranslationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZTranslation(double)));
    connect(this, SIGNAL(zTranslationChanged(double)), zTranslationSpinBox, SLOT(setValue(double)));
    if (render)
    {
        connect(rotationSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(scaleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(xScaleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(yScaleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(zScaleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(zTranslationSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    }

    layout->addRow("Поворот:", rotationSpinBox);
    layout->addRow("Масштабирование", scaleSpinBox);
    layout->addRow("Масштаб по X", xScaleSpinBox);
    layout->addRow("Масштаб по Y", yScaleSpinBox);
    layout->addRow("Масштаб по Z", zScaleSpinBox);
    layout->addRow("Высота", zTranslationSpinBox);
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

void RoadElementOBJ::setZTranslation(double translation)
{
    if (deltaZ == translation)
        return;
    deltaZ = translation;
    emit zTranslationChanged(translation);
}


void RoadElementOBJ::clear()
{
}

RoadElementOBJ::setZRotVertexArray()
{
    float pi = 3.14159265f;
    int numberOfSides = 40;
    for (int i = 0; i <= numberOfSides; ++i)
    {
        float angle = 2.0f * pi / (float(numberOfSides)) * float(i);
        zRotVertexArray.push_back(zRadius * cosf(angle));
        zRotVertexArray.push_back(zRadius * sinf(angle));
        zRotVertexArray.push_back(0.0f);
    }
}

RoadElementOBJ::setZRotColorArray(float r, float g, float b)
{
    zRotColorArray.resize(zRotVertexArray.size());
    for (int i = 0; i < zRotVertexArray.size() / 3; ++i)
    {
        zRotColorArray[i * 3] = r;
        zRotColorArray[i * 3 + 1] = g;
        zRotColorArray[i * 3 + 2] = b;
    }
}

RoadElementOBJ::setZRotIndexArray()
{
    zRotIndexArray.resize(zRotVertexArray.size() / 3);
    for (int i = 0; i < zRotVertexArray.size() / 3; ++i)
    {
        zRotIndexArray[i] = i;
    }
}


RoadElement *RoadElementOBJ::getCopy()
{
    RoadElementOBJ* copyElement = new RoadElementOBJ(*this);
    return copyElement;
}


void RoadElementOBJ::clearProperties(QLayout *layout)
{
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


QJsonObject RoadElementOBJ::getJSONInfo()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoadBroken::getJSONInfo()\n";
    QJsonObject element;

    element["Name"] = name;
    element["Layer"] = layer;

    element["Fixed"] = fixed;
    element["Id"] = Id;

    element["Folder"] = folder;
    element["Filemane"] = filename;

    element["ScaleFactor"] = scaleFactor;
    element["DeltaX"] = deltaX;
    element["DeltaY"] = deltaY;
    element["DeltaZ"] = deltaZ;
    element["ZRadius"] = zRadius;
    element["XRot"] = xRot;
    element["YRot"] = yRot;
    element["ZRot"] = zRot;
    element["XScale"] = xScale;
    element["YScale"] = yScale;
    element["ZScale"] = zScale;

    return element;

}


std::vector<vec3> RoadElementOBJ::getCoordOfControl(int index)
{
    std::vector<vec3> res;
    switch (index)
    {
    case 0:
        break;
    case 1:
    {
        vec3 p(deltaX, deltaY, deltaZ);
        res.push_back(p);
    }
        break;
    default:
        break;
    }
    return res;
}

void RoadElementOBJ::setCoordForControl(int index, std::vector<vec3> &controls)
{
}
