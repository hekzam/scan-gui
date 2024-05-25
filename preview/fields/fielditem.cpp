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

void FieldItem::setRect(QRectF rect)
{
  m_rect = rect;
}

QString FieldItem::getClef()
{
  return m_clef;
}

qreal FieldItem::getPenWidth()
{
  return pen.widthF();
}

fieldType FieldItem::getType()
{
  return m_Type;
}

void FieldItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsItem::mousePressEvent(event);
}

void FieldItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
  // QGraphicsItem::mouseDoubleClickEvent(e);
}

// see .h file for more info
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

QRectF FieldItem::getRect()
{
  return m_rect;
}
