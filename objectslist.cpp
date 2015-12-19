#include "objectslist.h"
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QDir>
#include <QDebug>
#include <QApplication>

ObjectsList::ObjectsList(QWidget *parent)
    : QListWidget(parent)
{
    this->setDragEnabled(true);
    this->setDropIndicatorShown(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //QListWidgetItem* item = new QListWidgetItem(QIcon("D://QT/Tools/QtCreator/bin/GL/1.bmp"),tr("Дорога"));

    //this->addItem("Дорога простая");
    this->addItem("Дорога ломаная");
    this->addItem("Поворот дороги");
    this->addItem("Перекресток");
    //this->addItem("Закругление");
    this->addItem("Разделительная зона (разметка)");
    this->addItem("Разделительная зона (газон)");
    this->addItem("Разделительная зона (тротуар)");
    this->addItem("Сплошная");
    this->addItem("Прерывистая");
    this->addItem("Двойная сплошная");
    this->addItem("Двойная прерывистая");
    this->addItem("Пешеходный переход");
    this->addItem("Трамвайные пути");
    this->addItem("Железная дорога");
    this->addItem("Провод/растяжка");
    this->addItem("Двойной провод");
    this->addItem("Столб");
    //this->addItem("Кривая Безье");
    //this->addItem("Куб");
    this->addItem("Рекламный щит");
    this->addItem("П-образная арка");
}

ObjectsList::ObjectsList(QString folder, QString fileType, QWidget *parent)
    : QListWidget(parent)
{
    directory.setPath(QApplication::applicationDirPath() + folder);
    this->fileType = fileType;
    QStringList list(directory.entryList(QStringList() << fileType));
    for (int i = 0; i < list.size(); ++i)
        this->addItem(list.at(i));
    filePath = folder;
    //qDebug() << filePath;
}

void ObjectsList::dropEvent(QDropEvent *event)
{
    ////qDebug() << "This is my custom dropEvent() method!";
    QListWidget::dropEvent(event);
}


void ObjectsList::mousePressEvent(QMouseEvent* event)
{

    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

}

void ObjectsList::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if (!itemAt(dragStartPosition))
        return;
    //if (event->button() != Qt::LeftButton)
    //    return;
    /*
    if ((event->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;
    */
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    if (filePath.size() == 0)
    {
        mimeData->setText(itemAt(dragStartPosition)->text());
    }
    else
    {
        QByteArray csvData;
        QStringList lst;
        lst << filePath << itemAt(dragStartPosition)->text();
        QString str;
        str = lst.join('|');
        csvData.append(str);
        mimeData->setData("text/plain",csvData);
        qDebug() << lst;
    }
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

}

void ObjectsList::resetObjects()
{
    this->clear();
    QStringList list(directory.entryList(QStringList() << fileType));
    for (int i = 0; i < list.size(); ++i)
        this->addItem(list.at(i));
    filePath = directory.absolutePath();
}



void ObjectsList::mouseReleaseEvent(QMouseEvent *event)
{
    /*
    if (event->button() == Qt::LeftButton)
    {
        if (itemAt(event->pos()))
        {
        itemAt(event->pos())->setSelected(true);
        emit itemClicked(itemAt(event->pos()));
        }
    }
    */

}
