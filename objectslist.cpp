#include "objectslist.h"
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QDir>
ObjectsList::ObjectsList(QWidget *parent)
    : QListWidget(parent)
{
    this->setDragEnabled(true);
    this->setDropIndicatorShown(true);

    //QListWidgetItem* item = new QListWidgetItem(QIcon("D://QT/Tools/QtCreator/bin/GL/1.bmp"),tr("Дорога"));

    this->addItem("Дорога простая");
    this->addItem("Дорога ломаная");
    //this->addItem("Круговой перекресток");
    this->addItem("Поворот дороги");
    this->addItem("Перекресток");
    this->addItem("Закругление");
    this->addItem("Разделительная зона");
    this->addItem("Сплошая");
    this->addItem("Прерывистая");
    this->addItem("Двойная сплошая");
    this->addItem("Двойная прерывистая");
    this->addItem("Пешеходный переход");
    this->addItem("Трамвайные пути");
    this->addItem("Железная дорога");

   // this->addItem("Куб");
    this->addItem("BMW M3");
    this->addItem("Audi Q7");
    //this->addItem("ВАЗ-2104");
    this->addItem("ВАЗ-2106");
    //this->addItem("Dodge_Ram_2007");
    this->addItem("Автобус ПАЗ");
    this->addItem("Микроавтобус Ford");
    this->addItem("Грузовик Ford");
    this->addItem("Остановка");
    //this->addItem("Знак");
    //this->addItem("Человек");
    //this->addItem("");

}

ObjectsList::ObjectsList(QDir directory, QWidget *parent): QListWidget(parent)
{
    QStringList list(directory.entryList(QStringList() << "*.3ds"));
    for (int i = 0; i < list.size(); ++i)
        this->addItem(list.at(i));
    filePath = directory.absolutePath();
}

void ObjectsList::dropEvent(QDropEvent *event)
{
    //qDebug() << "This is my custom dropEvent() method!";
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
        lst << filePath + "/" << itemAt(dragStartPosition)->text();
        QString str;
        str = lst.join(' ');
        csvData.append(str);
        mimeData->setData("text/plain",csvData);
    }
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

}



void ObjectsList::mouseReleaseEvent(QMouseEvent *event)
{
    itemAt(event->pos())->setSelected(true);
    emit itemClicked(itemAt(event->pos()));

}
