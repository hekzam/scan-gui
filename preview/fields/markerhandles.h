#ifndef MARKERHANDLES_H
#define MARKERHANDLES_H

#include <QGraphicsItem>
#include "markercontroller.h"

class MarkerHandles : public QGraphicsItem
{
public:
  MarkerHandles(QGraphicsItem *parent, QPointF p = QPointF(0, 0),
                int index = 0);
  ~MarkerHandles();
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = nullptr) override;

signals:
  void newCornerPos(int index, QPointF newPos);

protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;

private:
  int m_indexInFather;
  QGraphicsItem *m_parent;
  MarkerController *m_controller;
};

#endif // MARKERHANDLES_H
