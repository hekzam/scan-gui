#include "ocritem.h"

OCRItem::OCRItem(QGraphicsItem *parent) : FieldItem(parent)
{

  pen.setColor(Qt::green);
  m_Type = fieldType::ocr;
}

OCRItem::~OCRItem() {}
