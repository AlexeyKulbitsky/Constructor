#include "mapplane.h"
#include <QFileDialog>

MapPlane::MapPlane(GLfloat width, GLfloat height, QString &texture)
{
    name = "MapPlane";
    selected = fixed = false;
    elementX = elementY = 0.0f;
    this->texture = texture;
    this->width = width;
    this->height = height;
    vertexArray[0][0] = -width / 2.0f;
    vertexArray[0][1] = -height / 2.0f;
    vertexArray[0][2] = -0.02f;

    vertexArray[1][0] = -width / 2.0f;
    vertexArray[1][1] = height / 2.0f;
    vertexArray[1][2] = -0.02f;

    vertexArray[2][0] = width / 2.0f;
    vertexArray[2][1] = height / 2.0f;
    vertexArray[2][2] = -0.02f;

    vertexArray[3][0] = width / 2.0f;
    vertexArray[3][1] = -height / 2.0f;
    vertexArray[3][2] = -0.02f;

    textureID = TextureManager::getInstance()->getID(texture);

    textureArray[0][0] = 0.0f;
    textureArray[0][1] = 0.0f;

    textureArray[1][0] = 0.0f;
    textureArray[1][1] = 1.0f;

    textureArray[2][0] = 1.0f;
    textureArray[2][1] = 1.0f;

    textureArray[3][0] = 1.0f;
    textureArray[3][1] = 0.0f;

    layer = 4;
}

MapPlane::MapPlane(GLfloat width, GLfloat height, QImage &image)
{
    name = "MapPlane";
    selected = fixed = false;
    elementX = elementY = 0.0f;
    this->width = width;
    this->height = height;
    vertexArray[0][0] = -width / 2.0f;
    vertexArray[0][1] = -height / 2.0f;
    vertexArray[0][2] = -0.02f;

    vertexArray[1][0] = -width / 2.0f;
    vertexArray[1][1] = height / 2.0f;
    vertexArray[1][2] = -0.02f;

    vertexArray[2][0] = width / 2.0f;
    vertexArray[2][1] = height / 2.0f;
    vertexArray[2][2] = -0.02f;

    vertexArray[3][0] = width / 2.0f;
    vertexArray[3][1] = -height / 2.0f;
    vertexArray[3][2] = -0.02f;

    textureArray[0][0] = 0.0f;
    textureArray[0][1] = 0.0f;

    textureArray[1][0] = 0.0f;
    textureArray[1][1] = 1.0f;

    textureArray[2][0] = 1.0f;
    textureArray[2][1] = 1.0f;

    textureArray[3][0] = 1.0f;
    textureArray[3][1] = 0.0f;

    layer = 4;

    this->image = QImage(image);
    QImage image1 = QGLWidget::convertToGLFormat(image);
    glGenTextures(1, &textureID);
    // создаём и связываем 1-ый текстурный объект с последующим состоянием текстуры
    glBindTexture(GL_TEXTURE_2D, textureID);
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

MapPlane::MapPlane(const MapPlane &source)
{
    for (int i = 0; i < 4; ++i)
    {
        vertexArray[i][0] = source.vertexArray[i][0];
        vertexArray[i][1] = source.vertexArray[i][1];
        vertexArray[i][2] = source.vertexArray[i][2];

        textureArray[i][0] = source.textureArray[i][0];
        textureArray[i][1] = source.textureArray[i][1];
    }
    name = source.name;
    textureID = source.textureID;
    texture = source.texture;
    selected = fixed = false;
    elementX = source.elementX;
    elementY = source.elementY;
    width = source.width;
    height = source.height;
    layer = 4;
}

MapPlane::~MapPlane()
{
    glDeleteTextures(1, &textureID);
}



bool MapPlane::isSelected()
{
    return selected;
}

void MapPlane::setSelectedStatus(bool status)
{
    selected = status;
}

void MapPlane::drawFigure(QGLWidget *)
{
    glDisable(GL_LIGHTING);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    for (int i = 3; i >= 0; --i)
    {
        glTexCoord2f(textureArray[i][0], textureArray[i][1]);
        glVertex3f(vertexArray[i][0], vertexArray[i][1], vertexArray[i][2]);
    }
    glEnd();
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);
}

void MapPlane::drawSelectionFrame()
{
}

void MapPlane::drawMeasurements(QGLWidget *)
{
}

void MapPlane::move(float, float, float)
{
}

void MapPlane::drawControlElement(int, float, float)
{
}

QCursor MapPlane::getCursorForControlElement(int)
{
    return Qt::CrossCursor;
}

void MapPlane::resizeByControl(int, float, float, float, float)
{
}

int MapPlane::getNumberOfControls()
{
    return 1;
}

int MapPlane::controlsForPoint()
{
    return 1;
}

void MapPlane::changeColorOfSelectedControl(int)
{
}

void MapPlane::getProperties(QVBoxLayout *, QGLWidget *)
{
}

bool MapPlane::isFixed()
{
    return fixed;
}

int MapPlane::getLayer()
{
    return layer;
}

void MapPlane::clear()
{
}

RoadElement *MapPlane::getCopy()
{
    MapPlane *copyElement = new MapPlane(*this);
    return copyElement;
}

bool MapPlane::setFixed(bool fixed)
{
    this->fixed = fixed;
    return true;
}


QJsonObject MapPlane::getJSONInfo()
{
    QJsonObject element;
    element["Name"] = name;
    element["Layer"] = layer;
    element["Id"] = Id;
    element["Width"] = width;
    element["Height"] = height;
    if (texture.size() == 0)
    {
        QString fileName = QFileDialog::getSaveFileName(0, tr("Сохранить изображение захваченного участка карты"), QApplication::applicationDirPath(), tr("JPEG files (*.jpg)") );
        while (fileName.size() == 0)
        {
            fileName = QFileDialog::getSaveFileName(0, tr("Сохранить изображение захваченного участка карты"), QApplication::applicationDirPath(), tr("JPEG files (*.jpg)") );
        }
        texture = fileName;
        image.save(fileName);
    }
    element["Texture"] = texture;

    return element;
}
