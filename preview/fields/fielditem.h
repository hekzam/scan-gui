#ifndef FIELDITEM_H
#define FIELDITEM_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>

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
  void setRect(QRectF rect);
  QString getClef();
  qreal getPenWidth();
  fieldType getType();
  QRectF getRect();

protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
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

  bool visible = true; // true by default ?
  QRectF m_rect;
  QString m_clef;
  fieldType m_Type;

  QPen pen;
  QBrush brush;
};

#endif // FIELDITEM_H
