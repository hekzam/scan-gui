#include "pagemask.h"

PageMask::PageMask()
{
  // setPath(m_path);
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
