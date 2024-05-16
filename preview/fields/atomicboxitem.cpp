#include "atomicboxitem.h"

atomicBoxItem::atomicBoxItem(QGraphicsItem *parent, bool checked)
    : FieldItem(parent)
{
  // get data from the JSON somehow and init the value of the atomicbox
  pen.setColor(Qt::blue);
  if (!checked)
  {
    pen.setStyle(Qt::DashLine);
  }
  setPen(pen);
}

atomicBoxItem::atomicBoxItem(QGraphicsItem *parent)
    : atomicBoxItem(parent, false)
{
}

atomicBoxItem::~atomicBoxItem() {}
