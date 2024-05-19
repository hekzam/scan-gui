#ifndef PAGEMASK_H
#define PAGEMASK_H

#include <QGraphicsPathItem>
#include <QPainter>

class PageMask : public QGraphicsPathItem
{
public:
  PageMask();
  ~PageMask();

  void setMaskSize(QSize);

private:
  QSize m_maskSize;
  QPainterPath m_path;
  QBrush m_brush;
};

#endif // PAGEMASK_H
