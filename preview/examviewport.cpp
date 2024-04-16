#include "examviewport.h"
#include <QWheelEvent>
#include <QGraphicsScene>

using namespace mViewPort;

ExamViewPort::ExamViewPort(QGraphicsScene *gScene, QWidget *parent)
    : QGraphicsView(gScene, parent)
{
  gScene->setBackgroundBrush(Qt::gray);
  setDragMode(QGraphicsView::ScrollHandDrag);
  setRenderHints(QPainter::Antialiasing);
  // TODO : optimization flags
  // TODO : check those
  setOptimizationFlags(QGraphicsView::DontSavePainterState);
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  singleImage = new QGraphicsPixmapItem;
  loadAnswerSheet(*this, *gScene);
  // qDebug() << gView->transform();
  // qDebug() << gView->width();
  connect(this, &ExamViewPort::scaleChanged, this, &ExamViewPort::changeScale);
}

ExamViewPort::~ExamViewPort() {}

#if QT_CONFIG(wheelevent)
void ExamViewPort::wheelEvent(QWheelEvent *e)
{
  if (Qt::ControlModifier & e->modifiers())
  {
    // TODO finetune this
    // m_scale += e->angleDelta().y() / qreal(600);
    // m_scale = qMax(qreal(0.2), qMin(maxScalingFactor, m_scale));
    // qDebug() << m_scale;
    // emit scaleChanged(m_scale);
    if (e->angleDelta().y() > 0)
    {
      emit scaleChanged(1.25);
    }
    else
    {
      emit scaleChanged(0.8);
    }
    // e->accept();
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
  // update();
}

void ExamViewPort::loadAnswerSheet(QGraphicsView &gv, QGraphicsScene &gs)
{
  // filename should be stored somewhere I can read it, probably passed to me
  // by tableview load pixmap from filename QString filename =
  // xx.getfilename()
  QString filename =
      "/home/nathan/workspace/be_projet_gui_hekzam/DATASET/jpegexample.jpg";
  QPixmap p;
  if (!p.load(filename))
  {
    qWarning() << "error loading the image";
  }
  else
  {
    singleImage->setPixmap(p);
    singleImage->setPos(QPointF(0, 0));
    // scene takes ownership here
    gs.addItem(singleImage);
    // qDebug() << gs.height();
    // something wrong here, fit in view makes the image way too small
    // gv.fitInView(fitImageWidthInView(gv), Qt::KeepAspectRatio);
    gv.centerOn(singleImage);
  }
}

QRect ExamViewPort::fitImageWidthInView(QGraphicsView &gv)
{
  QSize ps = singleImage->pixmap().size();
  ROI = QRect(0, 0, ps.width(), 500);
  qDebug() << ps;
  return ROI;
}

void ExamViewPort::fitROIInView(QRect &)
{ // TODO
}
