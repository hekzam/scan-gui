#ifndef MARKERITEM_H
#define MARKERITEM_H

#include "fielditem.h"
#include "markerhandles.h"

class MarkerItem : public FieldItem
{
public:
  MarkerItem(QGraphicsItem *parent = nullptr);
  ~MarkerItem();

  void initCorners();

  void setNewCornerPos(int index, QPointF newPos);
  // use setPos and setPolygon instead
  // the points in parameter will be in ImageRelativeSize ??
  // void setPoints(QList<QPoint> p);
  // void setPoints(QList<QPointF> p);

  // void setPosition(const QPoint pos);
  // void setPosition(const QPointF pos);

protected:
  // mouse events
  // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
  // QList<QPoint> points;
  // QPoint position;
  QList<MarkerHandles *> handles;
};

#endif // MARKERITEM_H
