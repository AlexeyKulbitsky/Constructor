#include "sliderdelegate.h"
#include <QApplication>
#include <QStyleOptionProgressBar>



SliderDelegate::SliderDelegate(QObject *parent):QStyledItemDelegate(parent)
{

}

QWidget *SliderDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSlider* editor = new QSlider(parent);
    editor->setOrientation(Qt::Horizontal);
    editor->setAutoFillBackground(true);

    return editor;
}

void SliderDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSlider* slider = static_cast<QSlider*>(editor);
    slider->setValue(index.data().toInt());
}

void SliderDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSlider* slider = static_cast<QSlider*>(editor);
    model->setData(index, slider->value());

}

void SliderDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void SliderDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //QStyledItemDelegate::paint(painter, option, index);
    int value = index.data().toInt();

    QStyleOptionProgressBar progressBar;
    progressBar.minimum = 0;
    progressBar.maximum = 100;
    progressBar.rect = option.rect;
    progressBar.progress = value;
    progressBar.text = QString::number(value);
    progressBar.textVisible = true;

    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBar, painter);
}
