#ifndef MARKERCONTROLLER_H
#define MARKERCONTROLLER_H

#include <QGraphicsItem>
#include <QPointF>

class MarkerController
{
public:
  MarkerController(QGraphicsItem *marker);
  void updatePos(int index, QPointF pos, QGraphicsItem *marker);
};

#endif // MARKERCONTROLLER_H
