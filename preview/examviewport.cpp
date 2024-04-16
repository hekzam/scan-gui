#include "examviewport.h"
#include <QWheelEvent>
#include <QGraphicsScene>

using namespace mViewPort;

ExamViewPort::ExamViewPort(QWidget *parent) : QGraphicsView(parent)
{
  QGraphicsScene *gScene = new QGraphicsScene(parent);
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

  // fixedPreviewLayout->addWidget(gView);
  // previewStack->addWidget(fixedPreview);
  // previewStack->setCurrentWidget(fixedPreview);
  // qDebug() << gView->transform();
  // qDebug() << gView->width();
}

ExamViewPort::~ExamViewPort() {}

#if QT_CONFIG(wheelevent)
void ExamViewPort::wheelEvent(QWheelEvent *e)
{
  // TODO finetune this
  // if (Qt::ControlModifier && e->modifiers())
  {
    m_scale += e->angleDelta().y();
    qDebug() << m_scale;
    emit scaleChanged(m_scale);
    e->accept();
  }
}
#endif

void ExamViewPort::changeScale(qreal scale)
{
  // whos scale ?
  this->scale(scale, scale);
  update();
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
    // scene takes ownership here
    gs.addItem(singleImage);
    gv.fitInView(fitImageWidthInView(gv), Qt::KeepAspectRatio);
  }
}

QRect ExamViewPort::fitImageWidthInView(QGraphicsView &gv)
{
  QSize ps = singleImage->pixmap().size();
  QRect defaultViewSize(0, 0, ps.width(), gv.height());
  return defaultViewSize;
}

void ExamViewPort::fitROIInView(QRect &)
{ // TODO
}
