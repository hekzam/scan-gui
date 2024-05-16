#ifndef ATOMICBOXITEM_H
#define ATOMICBOXITEM_H

#include "fielditem.h"

class atomicBoxItem : public FieldItem
{
public:
  atomicBoxItem(QGraphicsItem *parent, bool checked);
  atomicBoxItem(QGraphicsItem *parent);
  ~atomicBoxItem();

private:
  bool value;
};

#endif // ATOMICBOXITEM_H
