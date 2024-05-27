#include "markeritem.h"

MarkerItem::MarkerItem(QGraphicsItem *parent) : FieldItem(parent)
{
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemIsSelectable);
  m_Type = fieldType::ItemTypeMarker;
}

MarkerItem::~MarkerItem() {}

void MarkerItem::initCorners()
{
  int index = 0;
  for (auto &p : polygon())
  {
    MarkerHandles corner = new MarkerHandles(this, p, index);
    handles.append(&corner);
    index++;
  }
}

// meant to be used by MarkerController only
void MarkerItem::setNewCornerPos(int index, QPointF newPos)
{
  QPolygonF pol = polygon();
  pol.replace(index, newPos);
  setPolygon(pol);
  QList<QVariant> data;
  for (auto &p : polygon())
  {
    data << p;
  }
  // careful here, I forgot to convert the data back into mm before sending it
  // (with mapImageCoordToJSONCoord).
  sendNewDataToLib(m_Type, m_clef, data);
}

void MarkerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    QPointF start = event->buttonDownScenePos(Qt::LeftButton);
    QPointF stop = event->scenePos();
    QPointF diff = stop - start;
    qDebug() << "in markeritems.cpp, see .cpp for more info :" << start << stop
             << diff;
    /*
     * What to do here :
     * Take the new coordinates of the marker after move and inform the
     * lib of the change. This is a different movement from setNewCornerPos
     * since we're moving all four corners at once.
     * Problem comes from the marker being an item located at QPointF(0,0) in
     * parent coordinates but having a polygon (AKA a visible object) at a
     * different place, sorry about that.
     * Changing the pos() of the object my
     * fix all your later problems though so you could try to modify the
     * setFieldItemAttributes function in singlepage.cpp to init the item at
     * pos(imgxy) and make the polygon just based on width and height. But this
     * will break a lot of things, notably the mask being put in the wrong
     * place.And don't forget to convert back the coordinates to mm with
     * mapImageCoordToJSONCoord (maybe move this function elsewhere ? Anyway,
     * good luck)
     */
    qDebug() << "new position of the "
                "marker relative to the starting point:"
             << pos();
    FieldItem::mouseReleaseEvent(event); // important
  }
}
