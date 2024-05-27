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
  sendNewDataToLib(m_Type, m_clef, data);
}
