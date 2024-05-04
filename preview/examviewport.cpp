#include "examviewport.h"

using namespace mViewPort;

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
                             const QString &jsonfilename)
{
  m_currentImageFilename = imgfilename;
  m_currentJsonFilename = jsonfilename;
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
    qDebug() << m_scale;
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

void ExamViewPort::fitROIInView(QRect &)
{ // TODO
}
