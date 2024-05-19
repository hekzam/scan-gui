#include "pagemask.h"

PageMask::PageMask()
{
  setZValue(2.0); // in front of everything
  m_brush.setColor(QColor(255, 255, 255, 200));
  setBrush(m_brush);
}

PageMask::~PageMask() {}

void PageMask::setMaskSize(QSize size)
{
  m_maskSize = size;
  m_path.clear();
  m_path.addRect(0, 0, m_maskSize.width(), m_maskSize.height());
  m_path.closeSubpath();
  setPath(m_path);
}

void PageMask::addFieldToHighlight(QPolygonF PS)
{
  // baseRectPath.moveTo(PS.topLeft());
  m_path.addPolygon(PS);
  m_path.closeSubpath();
  setPath(m_path);
}

// TODO add transparent rects on items we want to focus on
