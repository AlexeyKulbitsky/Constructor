#include "roadmodel.h"
#include <QDebug>
RoadModel::RoadModel(QObject *parent):QAbstractListModel(parent)
{
    //list << 33 << 77 << 88 << 2;
    roadElement = NULL;
}

RoadModel::~RoadModel()
{

}

int RoadModel::rowCount(const QModelIndex &parent) const
{
    //return list.count();
    if (roadElement == NULL)
        return 0;
    return 1;
}

QVariant RoadModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    //int value = list.at(index.row());
    if (roadElement == NULL)
        return QVariant();
    int value = roadElement->controlsForPoint();
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        //return list.at(index.row());
        return roadElement->controlsForPoint();
    case Qt::SizeHintRole:
        return QSize(0, 30);
    case Qt::ToolTipRole:
        return QString::number(list.at(index.row()));
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::FontRole:
        return QFont("Times New Roman", 12, QFont::Bold);
    case Qt::BackgroundRole:
        if (value < 30)
            return QBrush(Qt::red);
        else
            if (value < 60)
                return QBrush(Qt::green);
        else
                return QBrush(Qt::gray);
    default:
        return QVariant();
    }
}

bool RoadModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
   // if (role == Qt::EditRole)
      //  list.replace(index.row(), value.toInt());
}

Qt::ItemFlags RoadModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    //return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void RoadModel::setRoadElement(RoadElement *roadElement)
{
    if (roadElement == NULL)
        return;
    if (!roadElement->isSelected())
        return;
    this->roadElement = roadElement;
    qDebug() << "CONNECTED SUCCESFULLY";

}



