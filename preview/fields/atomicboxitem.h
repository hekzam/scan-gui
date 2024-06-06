#ifndef ATOMICBOXITEM_H
#define ATOMICBOXITEM_H

#include "fielditem.h"

class atomicBoxItem : public FieldItem
{
public:
  atomicBoxItem(QGraphicsItem *parent, bool checked);
  atomicBoxItem(QGraphicsItem *parent); // false by default
  ~atomicBoxItem();

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
  void changeValue();
  // I honestly have no idea how to keep the original value, doesn't feel
  // like I should keep it in a QgraphicsItem
  bool value;
  QPen penFieldNotChecked;
  QPen penFieldChecked;
  // bool originalValue;
};

#endif // ATOMICBOXITEM_H
