#include "markerhandles.h"
#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QCursor>

MarkerHandles::MarkerHandles(QGraphicsItem *parent, QPointF p, int index)
    : QGraphicsItem(parent), m_indexInFather(index), m_parent(parent)
{
  m_controller = new MarkerController(m_parent);
  setPos(p);
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

MarkerHandles::~MarkerHandles()
{
  delete m_controller;
}

QRectF MarkerHandles::boundingRect() const
{
  return QRectF(-5, -5, 10, 10);
}

void MarkerHandles::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
  QBrush b;
  QPen p;
  p.setWidth(1);
  p.setColor(QColorConstants::Red);
  b.setStyle(Qt::SolidPattern);
  b.setColor(QColorConstants::Red);
  painter->setBrush(b);
  painter->setPen(p);
  // draws the corner like a dot
  // painter->drawPie(boundingRect(), 0, 5760); // see QPainter doc
  // draws the corner like a cross
  painter->drawLine(-5, 0, 5, 0);
  painter->drawLine(0, 5, 0, -5);
}

void MarkerHandles::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
  if (e->button() == Qt::LeftButton)
  {
    m_controller->updatePos(m_indexInFather, pos(), m_parent);
  }
  QGraphicsItem::mouseReleaseEvent(e);
}
