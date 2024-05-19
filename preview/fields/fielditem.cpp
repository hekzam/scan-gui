#include "fielditem.h"

FieldItem::FieldItem(QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
  pen.setWidth(4);
  pen.setColor(QColorConstants::Red); // can be replaced by Qt::red
  brush.setStyle(Qt::NoBrush);
  setPen(pen);
  setBrush(brush);
}

FieldItem::~FieldItem() {}

void FieldItem::setClef(QString clef)
{
  m_clef = clef;
}

qreal FieldItem::getPenWidth()
{
  return pen.widthF();
}

fieldType FieldItem::getType()
{
  return m_Type;
}

void FieldItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
  // QGraphicsItem::mouseDoubleClickEvent(e);
}

QMap<QString, QList<QVariant>> FieldItem::sendNewDataToLib(fieldType type,
                                                           QString identifier,
                                                           QList<QVariant> data)
{
  QMap<QString, QList<QVariant>> map;

  /* no need for a switch case if there's no further manipulations
   * to be done on the data
   */
  switch (type)
  {
  case ItemTypeMarker:
    break;
  case ItemTypeAtomicBox:
    break;
  case ItemTypeOCR:
    break;
  default:
    qWarning() << "what is you doing";
  }
  map[identifier] = data;
  qDebug() << map;
  return map;
}

QPointF FieldItem::getFieldImgPosition()
{
  return pos();
}
