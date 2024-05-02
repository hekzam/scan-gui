#include "singlepage.h"

ExamSinglePage::ExamSinglePage()
    : QGraphicsPixmapItem()
{}

ExamSinglePage::~ExamSinglePage() {}

void ExamSinglePage::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    // qDebug() << event->type();
    qDebug() << event->pos();
    // TODO; round this
    QGraphicsItem::mouseDoubleClickEvent(event);
}
