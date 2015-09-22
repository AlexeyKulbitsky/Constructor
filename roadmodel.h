#ifndef ROADMODEL_H
#define ROADMODEL_H

#include <QAbstractListModel>
#include <QSize>
#include <QBrush>
#include <QFont>
#include <QStringList>
#include "roadelement.h"
#include "road.h"


#include "roundingcrossroad.h"

class RoadModel: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit RoadModel(QObject* parent = 0);
    ~RoadModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;


private:
    QList<int> list;
    RoadElement* roadElement;
signals:
    void modelReseted();
public slots:
    void setRoadElement(RoadElement* roadElement);
};

#endif // ROADMODEL_H
