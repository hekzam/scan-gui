#include "markercontroller.h"

#include "markeritem.h"

MarkerController::MarkerController(QGraphicsItem *marker) {}

void MarkerController::updatePos(int index, QPointF pos, QGraphicsItem *marker)
{
  MarkerItem *m = dynamic_cast<MarkerItem *>(marker); // idk man, this works
  m->setNewCornerPos(index, pos);
}
