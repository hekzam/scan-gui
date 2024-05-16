#ifndef MARKERITEM_H
#define MARKERITEM_H

#include "fielditem.h"

class MarkerItem : public FieldItem
{
public:
  MarkerItem(QGraphicsItem *parent = nullptr);
  ~MarkerItem();

  // use setPos and setPolygon instead
  // the points in parameter will be in ImageRelativeSize ??
  // void setPoints(QList<QPoint> p);
  // void setPoints(QList<QPointF> p);

  // void setPosition(const QPoint pos);
  // void setPosition(const QPointF pos);

private:
  // QList<QPoint> points;
  // QPoint position;
};

#endif // MARKERITEM_H
