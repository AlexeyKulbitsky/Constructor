#include "roadelement.h"

bool RoadElement::Ids[SIZE] = {false};

QUndoStack* RoadElement::undoStack = NULL;

std::vector<vec3> RoadElement::getCoordOfControl(int index)
{
    std::vector<vec3> p; return p;
}


void RoadElement::setCoordForControl(int index, std::vector<vec3> &controls)
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


RoadElement::RoadElement()
{
    for (int i = 0; i < SIZE; ++i)
    {
        if (Ids[i] == false)
        {
            Id = i;
            Ids[i] = true;
            break;
        }
    }
    showMeasurements = true;
}

RoadElement::~RoadElement()
{
    Ids[Id] = false;
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

void RoadElement::getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadElement::getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz)\n";
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
    y = viewport[3]-y;
    gluUnProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
}

void RoadElement::setId(int Id)
{
    Ids[this->Id] = false;
    this->Id = Id;
    Ids[Id] = true;
}

RoadElement::setShowMeasurements(bool status)
{
    showMeasurements = status;
}


RoadElementMimeData::RoadElementMimeData(RoadElement *element)
{
    this->elements << element->getCopy();
    myFormats << "constructor/roadelement";
}

RoadElementMimeData::RoadElementMimeData(QList<RoadElement *> &elements)
{
    for (int i = 0; i < elements.size(); ++i)
        this->elements << elements[i]->getCopy();
    myFormats << "constructor/roadelement";
}

RoadElementMimeData::RoadElementMimeData()
{
    myFormats << "constructor/roadelement";
}

RoadElementMimeData::~RoadElementMimeData()
{
    //QMessageBox::about(0, "JHF","RoadElementMimeData::~RoadElementMimeData()");
    for (int i = 0; i < elements.size(); ++i)
    {
        if (elements[i])
            delete elements[i];
        elements[i] = NULL;
    }
}

QList<RoadElement *> RoadElementMimeData::getElements() const
{
    return elements;
}


QVariant RoadElementMimeData::retrieveData(const QString &mimetype, QVariant::Type preferredType) const
{
    if (mimetype == "constructor/roadelement")
        return QVariant::fromValue(elements);
    else
        return QMimeData::retrieveData(mimetype, preferredType);
}

QStringList RoadElementMimeData::formats() const
{
    return myFormats;
}


bool RoadElementMimeData::hasFormat(const QString &mimetype) const
{
    if (mimetype == "constructor/roadelement")
        return true;
    else
        return false;
}
