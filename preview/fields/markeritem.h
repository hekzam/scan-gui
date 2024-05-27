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

protected:
  // mouse events
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  QList<MarkerHandles *> handles;
};

#endif // MARKERITEM_H
