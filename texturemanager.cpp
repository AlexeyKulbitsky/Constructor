#include "texturemanager.h"
#include <QImage>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QGLWidget>
TextureManager* TextureManager::manager = NULL;





int TextureManager::getTextureID(QString textureSource)
{
    QImage image1;

    GLuint ID;
    image1.load(textureSource);
    image1 = QGLWidget::convertToGLFormat(image1);
    glGenTextures(1, &ID);
    // создаём и связываем 1-ый текстурный объект с последующим состоянием текстуры
    glBindTexture(GL_TEXTURE_2D, ID);
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
    //qDebug() << "TEXTURE ID " << ID;
    return ID;
}

int TextureManager::getID(QString texture)
{
    if (!textureMap.contains(texture))
        textureMap[texture] = getTextureID(texture);
    return textureMap[texture];
}
