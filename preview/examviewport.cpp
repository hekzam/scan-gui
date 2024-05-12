#include "examviewport.h"

using namespace mViewPort;
using namespace mJSON;

ExamViewPort::ExamViewPort(QGraphicsScene *gScene, QWidget *parent)
    : QGraphicsView(gScene, parent), dimensionToConsider{scaleToFit::height}
{
  gScene->setBackgroundBrush(Qt::gray);
  setDragMode(QGraphicsView::ScrollHandDrag);
  setRenderHints(QPainter::Antialiasing);

  // TODO : optimization flags
  setOptimizationFlags(QGraphicsView::DontSavePainterState);
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  // do we need this
  // setAttribute(Qt::WA_StaticContents);

  m_singleImage = new ExamSinglePage();
  gScene->addItem(m_singleImage);

  // test variables
  // QString exampleIFN = ":/preview/resources/jpegexample.jpg";
  // QString exJFN = "";
  // loadImage(exampleIFN, exJFN);
  connect(this, &ExamViewPort::scaleChanged, this, &ExamViewPort::changeScale);
}

ExamViewPort::~ExamViewPort() {}

void ExamViewPort::loadImage(const QString &imgfilename,
                             const dataCopieJSON &data)
{
  m_currentImageFilename = imgfilename;
  m_jsonData = &data;
  loadAnswerSheet();
}

void ExamViewPort::loadImage(const QString &imgfilename)
{
  m_currentImageFilename = imgfilename;
  loadAnswerSheet();
}

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
    // qDebug() << m_scale;
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

void ExamViewPort::changeDrawMode(bool state)
{
  drawingMode = state;
  // TODO: enable drawing of rectangles by drag and drop, exit draw mode on
  // mouse release ?
  // other things to toggle here (dragmode...)
}

void ExamViewPort::rotateImage(int value)
{
  rotate(value - m_rotation);
  m_rotation = value;
}

void ExamViewPort::toggleFieldsVisibility(bool visible)
{
  for (auto &c : m_singleImage->childItems())
  {
    c->setVisible(visible);
  }
}

// throw some error here to let the preview window know?
void ExamViewPort::loadAnswerSheet()
{
  QPixmap p;
  if (!p.load(m_currentImageFilename))
  {
    qWarning() << "error loading the image";
  }
  else
  {
    m_singleImage->setPixmap(p);
    m_singleImage->setPos(QPointF(0, 0));

    scaleToWidgetSize();
    // gv.ensureVisible(m_ROI);
  }
}

void ExamViewPort::scaleToWidgetSize()
{
  int imageDim;
  int widgetDim;
  int minWidgetDim;
  switch (dimensionToConsider)
  {
  case scaleToFit::height:
    imageDim = m_singleImage->pixmap().height();
    widgetDim = height();
    minWidgetDim = minPreviewSize.height();
    break;
  default:
    imageDim = m_singleImage->pixmap().width();
    widgetDim = width();
    minWidgetDim = minPreviewSize.width();
    break;
  }
  qreal baseScaleFactor =
      (qreal) (minWidgetDim > widgetDim ? minWidgetDim : widgetDim) / imageDim;
  auto scaleBy = baseScaleFactor / m_scale;
  changeScale(scaleBy);
  // qDebug() << ps;
  // ROI = QRect(0, 0, ps.width(), gv.height());
  // qDebug() << ROI;
  // return ROI;
}

void ExamViewPort::scaleToOneOnOne()
{
  changeScale(1 / m_scale);
}

// TODO
void ExamViewPort::mousePressEvent(QMouseEvent *e)
{
  if (drawingMode)
  {
    // TODO
  }
  else
  {
    QGraphicsView::mousePressEvent(e);
  }
}

void ExamViewPort::mouseMoveEvent(QMouseEvent *e)
{
  if (drawingMode)
  {
    // TODO
  }
  else
  {
    QGraphicsView::mouseMoveEvent(e);
  }
}

void ExamViewPort::mouseReleaseEvent(QMouseEvent *e)
{
  if (drawingMode)
  {
    // TODO
  }
  else
  {
    QGraphicsView::mouseReleaseEvent(e);
  }
}

void ExamViewPort::fitROIInView(QRect &)
{ // TODO
}
