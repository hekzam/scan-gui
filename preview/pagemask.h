#ifndef PAGEMASK_H
#define PAGEMASK_H

#include <QGraphicsPathItem>
#include <QPainter>

class PageMask : public QGraphicsPathItem
{
public:
  PageMask();
  ~PageMask();

  void setMaskSize(QSize size);
  void addFieldToHighlight(QPolygonF PS);

private:
  QSize m_maskSize;
  QPainterPath m_path;
  QPainterPath fieldHighlightPath;
  QBrush m_brush;
  QPen m_pen;
};

#endif // PAGEMASK_H
