#include "roadelement.h"

QUndoStack* RoadElement::undoStack = NULL;

std::vector<vec3> RoadElement::getCoordOfControl(int index)
{
    std::vector<vec3> p; return p;
}


void RoadElement::setCoordForControl(int index)
{

}


QString RoadElement::getName()
{
    return name;
}


QJsonObject RoadElement::getJSONInfo()
{
    QJsonObject a; return a;
}


void RoadElement::clearProperties(QLayout *layout)

{
    /*
        QLayoutItem *item;
        while((layout->count() > 0)) {
            item = layout->itemAt(0);
            if (item->layout()) {
                clearProperties(item->layout());
                delete item->layout();
            }
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
      */
   // QLayoutItem *child;
  //  while ((child = layout->takeAt(0)) != 0) {
  //      delete child;
  //  }
}


void RoadElement::setModel(Model *model)
{
    this->model = model;
}


void RoadElement::addElement(RoadElement *element)
{

}


void RoadElement::deleteElement(int index)
{

}


int RoadElement::getNumberOfElements()
{
    return 0;
}


RoadElement *RoadElement::getElement(int index)
{
    return NULL;
}


RoadElement::~RoadElement()
{

}


void RoadElement::rotate(float angle, float x, float y, float z)
{

}


void RoadElement::setStepDialog(StepDialog *dialog)
{
    if (dialog)
        stepDialog = dialog;
}


void RoadElement::setStepDialogs(StepDialog **dialogs, int size)
{
    if (!dialogs)
        return;
    for (int i = 0; i < size; ++i)
        stepDialogs[i] = dialogs[i];
}


void RoadElement::deleteLine(RoadElement *line)
{

}

void RoadElement::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}
