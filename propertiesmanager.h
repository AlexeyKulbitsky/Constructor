#ifndef PROPERTIESMANAGER_H
#define PROPERTIESMANAGER_H

#include <QObject>

class PropertiesManager : public QObject
{
    Q_OBJECT
public:
    explicit PropertiesManager(QObject *parent = 0);

signals:

public slots:
};

#endif // PROPERTIESMANAGER_H
