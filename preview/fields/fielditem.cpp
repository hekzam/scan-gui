#include "fielditem.h"

FieldItem::FieldItem(QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
  pen.setWidth(4);
  pen.setColor(Qt::red);
  setPen(pen);
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

QMap<QString, QList<QVariant>> FieldItem::sendNewDataToLib(fieldType type,
                                                           QString identifier,
                                                           QList<QVariant> data)
{
  QMap<QString, QList<QVariant>> map;

  /* maybe there's no need for a switch case if there's no further manipulations
   * to be done on the data
   */

  switch (type)
  {
  case marker:
    map[identifier] << QPoint(10, 10) << QPoint(10, 20) << QPoint(20, 20)
                    << QPoint(20, 10);
    break;
  case atomicBox:
    map[identifier] << false;
    break;
  case ocr:
    map[identifier] << QString("what the hell");
    break;
  default:
    qWarning() << "what is you doing";
  }
  return map;
}

QPoint FieldItem::getFieldDocPosition()
{
  return fieldPositionDocRel;
}

QPoint FieldItem::getFieldImgPosition()
{
  return fieldPositionImageRelative;
}

QSize FieldItem::getSizeImageRelative()
{
  return fieldSizeImageRel;
}

QSize FieldItem::getSizeDocRelative()
{
  return fieldSizeDocRel;
}
