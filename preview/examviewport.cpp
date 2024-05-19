#include "examviewport.h"

using namespace mViewPort;
using namespace mJSON;

ExamViewPort::ExamViewPort(ExamScene *gScene, QWidget *parent)
    : QGraphicsView(gScene, parent), dimensionToConsider{scaleToFit::height}
{
  setDragMode(QGraphicsView::ScrollHandDrag);
  setRenderHints(QPainter::Antialiasing);

  // TODO : optimization flags
  setOptimizationFlags(QGraphicsView::DontSavePainterState);
  // FullViewPortUpdate is used to avoid small errors when rendering the
  // fielditems
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  // do we need this
  // setAttribute(Qt::WA_StaticContents);

  connect(this, &ExamViewPort::scaleChanged, this, &ExamViewPort::changeScale);
}

ExamViewPort::~ExamViewPort() {}

#if QT_CONFIG(wheelevent)
void ExamViewPort::wheelEvent(QWheelEvent *e)
{
  if (Qt::ControlModifier & e->modifiers())
  {
    if (e->angleDelta().y() > 0)
    {
      if (m_scale < maxScalingFactor)
      {
        emit scaleChanged(1.25);
      }
    }
    else
    {
      if (m_scale > minScalingFactor)
      {
        emit scaleChanged(0.8);
      }
    }
  }
  else if (Qt::ShiftModifier & e->modifiers())
  {
    if (e->angleDelta().y() > 0)
    {
      if (m_rotation > -180)
        emit imageRotationChanged(m_rotation - 30);
    }
    else
    {
      if (m_rotation < 180)
        emit imageRotationChanged(m_rotation + 30);
    }
  }
  else
  {
    QGraphicsView::wheelEvent(e);
  }
}
#endif

void ExamViewPort::changeScale(qreal scale)
{
  m_scale *= scale;
  this->scale(scale, scale);
}

void ExamViewPort::rotateImage(int value)
{
  rotate(value - m_rotation);
  m_rotation = value;
}

void ExamViewPort::scaleToWidgetSize(QSize newImageSize)
{
  int imageDim;
  int widgetDim;
  int minWidgetDim;
  switch (dimensionToConsider)
  {
  case scaleToFit::height:
    imageDim = newImageSize.height();
    widgetDim = height();
    minWidgetDim = minPreviewSize.height();
    break;
  default:
    imageDim = newImageSize.width();
    widgetDim = width();
    minWidgetDim = minPreviewSize.width();
    break;
  }
  qreal baseScaleFactor =
      (qreal) (minWidgetDim > widgetDim ? minWidgetDim : widgetDim) / imageDim;
  auto scaleBy = baseScaleFactor / m_scale;
  changeScale(scaleBy);
}

void ExamViewPort::scaleToOneOnOne()
{
  changeScale(1 / m_scale);
}

// TODO
void ExamViewPort::mousePressEvent(QMouseEvent *e)
{
  // if (calibrationMode)
  // {
  //   // TODO
  // }
  // else
  // {
  QGraphicsView::mousePressEvent(e);
  // }
}

void ExamViewPort::mouseMoveEvent(QMouseEvent *e)
{
  // if (calibrationMode)
  // {
  //   // TODO
  // }
  // else
  // {
  QGraphicsView::mouseMoveEvent(e);
  // }
}

void ExamViewPort::mouseReleaseEvent(QMouseEvent *e)
{
  // if (calibrationMode)
  // {
  //   // TODO
  // }
  // else
  // {
  QGraphicsView::mouseReleaseEvent(e);
  // }
}

void ExamViewPort::fitROIInView(QRect &)
{ // TODO
}
