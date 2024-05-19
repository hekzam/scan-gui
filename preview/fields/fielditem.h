#ifndef FIELDITEM_H
#define FIELDITEM_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>

// #include "../../json/jsonreader.h"

enum fieldType
{
  ItemTypeUndef,
  ItemTypeMarker,
  ItemTypeAtomicBox,
  ItemTypeOCR,
};

class FieldItem : public QGraphicsPolygonItem
{
public:
  FieldItem(QGraphicsItem *parent = nullptr);
  ~FieldItem();

  void setClef(QString clef);
  qreal getPenWidth();
  fieldType getType();

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

  /*
   * This will send data to the TypstLib :
   * -QString is the ID (?) of the field that has been modified
   * - QVariant can be (data types I can think of)
   *    - list of Floating points coordinates to indicate the new boundaries of
   *      a marker that has been moved by the user
   *    - new bool value of an atomic box
   *    - char or QString to modify the interpretation of OCR-generated fields
   */
  QMap<QString, QList<QVariant>>
  sendNewDataToLib(fieldType type, QString identifier, QList<QVariant> data);

  // relative to the image ?
  QPointF getFieldImgPosition();

  bool visible = true; // true by default ?
  // maybe not useful
  QPoint fieldPositionDocRel;
  QPoint fieldPositionImageRelative;
  QSize fieldSizeDocRel;
  QSize fieldSizeImageRel;
  QString m_clef;
  fieldType m_Type;

  QPen pen;
  QBrush brush;
};

#endif // FIELDITEM_H
