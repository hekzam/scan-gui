#ifndef SINGLEPAGE_H
#define SINGLEPAGE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class ExamSinglePage : public QGraphicsPixmapItem
{
public:
    ExamSinglePage();
    ~ExamSinglePage();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // SINGLEPAGE_H
