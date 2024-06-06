#include "ocritem.h"

OCRItem::OCRItem(QGraphicsItem *parent) : FieldItem(parent)
{

  pen.setColor(Qt::green);
  setPen(pen);
  // setBrush(brush);
  m_Type = fieldType::ItemTypeOCR;
}

OCRItem::~OCRItem() {}
