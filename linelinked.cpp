#include "linelinked.h"

LineLinked::LineLinked()
{
    Logger::getLogger()->writeLog("Created LineLinked constructor: LineLinked::LineLinked()");
}

LineLinked::LineLinked(float width, float *axisVertices, int size, QString name, int layer):
    LineBroken(width, axisVertices, size, name, layer)
{
    Logger::getLogger()->writeLog("Created LineLinked constructor: LineLinked::LineLinked(float width, float *axisVertices, int size, QString name, int layer)");
}

LineLinked::LineLinked(float width, float *axisVertices, int size, float red, float green, float blue, float alpha, QString name, int layer):
    LineBroken(width, axisVertices, size, red, green, blue, alpha, name, layer)
{
    Logger::getLogger()->writeLog("Created LineLinked constructor: LineLinked::LineLinked(float width, float *axisVertices, int size, float red, float green, float blue, float alpha, QString name, int layer)");
}

LineLinked::LineLinked(float width, float *axisVertices, int size, QString source, float textureSize, QString name, int layer):
    LineBroken(width, axisVertices, size, source, textureSize, name, layer)
{
    Logger::getLogger()->writeLog("Created LineLinked constructor: LineLinked::LineLinked(float width, float *axisVertices, int size, QString source, float textureSize, QString name, int layer)");
}

LineLinked::LineLinked(float width, float *axisVertices, int size, QString source, float textureSize, QString name, int layer, QString description):
    LineBroken(width, axisVertices, size, source, textureSize, name, layer, description)
{
    Logger::getLogger()->writeLog("Created LineLinked constructor: LineLinked::LineLinked(float width, float *axisVertices, int size, QString source, float textureSize, QString name, int layer, QString description)");
}

