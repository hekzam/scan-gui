#include "atomicboxitem.h"

atomicBoxItem::atomicBoxItem(QGraphicsItem *parent, bool checked)
    : FieldItem(parent), value(checked), penFieldChecked(pen),
      penFieldNotChecked(pen)
{
  penFieldNotChecked.setColor(QColorConstants::Gray);
  penFieldNotChecked.setStyle(Qt::DashLine);
  penFieldChecked.setColor(QColorConstants::DarkBlue);
  penFieldChecked.setStyle(Qt::SolidLine);

  setPen(value ? penFieldChecked : penFieldNotChecked);
  m_Type = fieldType::ItemTypeAtomicBox;
}

atomicBoxItem::atomicBoxItem(QGraphicsItem *parent)
    : atomicBoxItem(parent, false)
{
}

atomicBoxItem::~atomicBoxItem() {}

void atomicBoxItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
  if (e->button() == Qt::LeftButton)
  {
    changeValue();
  }
  FieldItem::mouseDoubleClickEvent(e);
}

void atomicBoxItem::changeValue()
{
  value = !value; // inv
  setPen(value ? penFieldChecked : penFieldNotChecked);
  QList<QVariant> data;
  data << value;
  sendNewDataToLib(m_Type, m_clef, data);
}
