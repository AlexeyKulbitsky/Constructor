#ifndef LINELINKED_H
#define LINELINKED_H

#include "linebroken.h"

enum LinkType {DEFAULT = 0, PERPENDICULAR, PARALLEL};

struct LinkParameters
{
    int linkIndex;
    LinkType type;
    float distance;

};

class LineLinked: public LineBroken
{
    Q_OBJECT
public:
    LineLinked();
    LineLinked(float width, float* axisVertices, int size, QString name, int layer);
    LineLinked(float width, float* axisVertices, int size, float red, float green, float blue, float alpha, QString name, int layer);
    LineLinked(float width, float* axisVertices, int size, QString source, float textureSize, QString name, int layer);
    LineLinked(float width, float* axisVertices, int size, QString source, float textureSize, QString name, int layer, QString description);

private:

};

#endif // LINELINKED_H
