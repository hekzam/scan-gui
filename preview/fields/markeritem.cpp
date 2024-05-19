#include "markeritem.h"

MarkerItem::MarkerItem(QGraphicsItem *parent) : FieldItem(parent)
{
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemIsSelectable);
  m_Type = fieldType::ItemTypeMarker;
}

MarkerItem::~MarkerItem() {}

// no need ?

// void MarkerItem::setPoints(QList<QPoint> p)
// {
//   // setPolygon(p);
// }

// void MarkerItem::setPoints(QList<QPointF> p)
// {
//   // QList<QPoint> rounded;
//   // for (auto &pp : p)
//   // {
//   //   // Rounds the coordinates of this point to the nearest integer
//   //   rounded << pp.toPoint();
//   // }
//   // setPoints(rounded);
//   setPolygon(p);
// }

// // the points in parameter will be in ImageRelativeSize
// void MarkerItem::setPosition(const QPoint pos)
// {
//   setPos(pos);
// }

// void MarkerItem::setPosition(const QPointF pos)
// {
//   // setPosition(pos.toPoint());
//   setPos(pos);
// }
