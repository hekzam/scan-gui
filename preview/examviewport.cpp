#include "examviewport.h"

using namespace mViewPort;

ExamViewPort::ExamViewPort(QGraphicsScene *gScene, QWidget *parent)
    : QGraphicsView(gScene, parent)
{
  // setMinimumSize(minPreviewSize);

  gScene->setBackgroundBrush(Qt::gray);
  setDragMode(QGraphicsView::ScrollHandDrag);
  setRenderHints(QPainter::Antialiasing);

  // TODO : optimization flags
  setOptimizationFlags(QGraphicsView::DontSavePainterState);
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  singleImage = new ExamSinglePage();
  gScene->addItem(singleImage);

  // test variables
  QString exampleIFN = ":/preview/resources/jpegexample.jpg";
  QString exJFN = "";
  loadImage(exampleIFN, exJFN);
  connect(this, &ExamViewPort::scaleChanged, this, &ExamViewPort::changeScale);
}

ExamViewPort::~ExamViewPort() {}

void ExamViewPort::loadImage(QString &imgfilename, QString &jsonfilename)
{
  currentImageFilename = imgfilename;
  currentJsonFilename = jsonfilename;
  loadAnswerSheet(*this, *this->scene());
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
        m_scale *= 1.25;
        emit scaleChanged(1.25);
      }
    }
    else
    {
      if (m_scale > minScalingFactor)
      {
        m_scale *= 0.8;
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
  this->scale(scale, scale);
}

// throw some error here to let the preview window know?
void ExamViewPort::loadAnswerSheet(QGraphicsView &gv, QGraphicsScene &gs)
{
  QPixmap p;
  if (!p.load(currentImageFilename))
  {
    qWarning() << "error loading the image";
  }
  else
  {
    singleImage->setPixmap(p);
    singleImage->setPos(QPointF(0, 0));

    scaleToWidgetSize(gv, gs);
    gv.ensureVisible(ROI);
  }
}

void ExamViewPort::scaleToWidgetSize(QGraphicsView &gv, QGraphicsScene &gs)
{
  QSize ps = singleImage->pixmap().size();
  qreal scalefactor = (qreal) minPreviewSize.width() / ps.width();
  m_scale = scalefactor;
  this->scale(scalefactor, scalefactor);
  // qDebug() << ps;
  // ROI = QRect(0, 0, ps.width(), gv.height());
  // qDebug() << ROI;
  // return ROI;
}

void ExamViewPort::fitROIInView(QRect &)
{ // TODO
}
