#include "preview.h"
#include "qevent.h"

using namespace mViewPort;

static const qreal maxScalingFactor = 5.0;

SheetViewPort::SheetViewPort(QGraphicsScene *scene) : QGraphicsView(scene) {}

ExamPreview::ExamPreview(QWidget *parent)
{
  previewBox = new QGroupBox(tr("Preview"), parent);
  QVBoxLayout *previewLayout = new QVBoxLayout(previewBox);
  setCheckable(false);
  createPreviewButtonBox();
  createPreviewStack();

  previewLayout->addWidget(previewStack);
  previewLayout->addWidget(previewButtonBox);
  connect(this, &ExamPreview::scaleChanged, this, &ExamPreview::changeScale);
}

ExamPreview::~ExamPreview()
{
  // delete im;
  // delete gScene;
  // delete gView;
  delete previewBox;
}

void ExamPreview::createPreviewStack()
{
  previewStack = new QStackedWidget(previewBox);
  createFixedPreview();
  createFloatablePreview();
  previewStack->setCurrentIndex(0);
}

void ExamPreview::createPreviewButtonBox()
{
  previewButtonBox = new QGroupBox(previewBox);
  QHBoxLayout *previewButtonLayout = new QHBoxLayout(previewButtonBox);
  previewButtonBox->setAlignment(Qt::AlignCenter);
  QPushButton *deletePageButton =
      new QPushButton(tr("Delete the page"), previewButtonBox);
  QPushButton *assignPageButton =
      new QPushButton(tr("Assign this page"), previewButtonBox);
  QPushButton *validatePageButton =
      new QPushButton(tr("Mark as verified/validated"), previewButtonBox);

  previewButtonLayout->addWidget(deletePageButton);
  // is this right
  connect(deletePageButton, &QPushButton::clicked, this,
          &ExamPreview::deletePage);

  previewButtonLayout->addWidget(assignPageButton);
  connect(assignPageButton, &QPushButton::clicked, this,
          &ExamPreview::assignPage);

  previewButtonLayout->addWidget(validatePageButton);
  connect(validatePageButton, &QPushButton::clicked, this,
          &ExamPreview::markExamSheetAsValidated);
  // TODO : connect...
}

// TODO : définir la grille
void ExamPreview::createFixedPreview()
{
  // TODO :resize event
  QFrame *fixedPreview = new QFrame(previewStack);
  QVBoxLayout *fixedPreviewLayout = new QVBoxLayout(fixedPreview);
  fixedPreview->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

  // pour remplacer tout le reste qui a été déplacé dans un constructeur
  // gView = new ExamViewPort(fixedPreview);
  QGraphicsScene *gScene = new QGraphicsScene(fixedPreview);
  gScene->setBackgroundBrush(Qt::gray);
  gView = new SheetViewPort(gScene);
  gView->setDragMode(QGraphicsView::ScrollHandDrag);
  gView->setRenderHints(QPainter::Antialiasing);
  // TODO : optimization flags
  // TODO : check those
  gView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
  gView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  gView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  singleImage = new QGraphicsPixmapItem;
  loadAnswerSheet(*gView, *gScene);

  fixedPreviewLayout->addWidget(gView);
  previewStack->addWidget(fixedPreview);
  // previewStack->setCurrentWidget(fixedPreview);
  // qDebug() << gView->transform();
  // qDebug() << gView->width();
}

void ExamPreview::createFloatablePreview()
{
  QDockWidget *floatablePreview = new QDockWidget(previewStack);
  QGraphicsView gv = new QGraphicsView(floatablePreview);
  floatablePreview->setWidget(&gv);
  // TODO
  previewStack->addWidget(floatablePreview);
}

void ExamPreview::setGroupBoxTitle() {}

#if QT_CONFIG(wheelevent)
void ExamPreview::wheelEvent(QWheelEvent *e)
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

// nevermind
// void ExamPreview::resizeEvent(QResizeEvent *e)
// {
//   // TODO : fix this
//   m_scale = qMax(qreal(0.1), qMin(maxScalingFactor,
//                                   qreal(e->size().width() /
//                                         qreal(singleImage->pixmap().width()))));
//   // m_scale = qMax(qreal(0.1), qMin(qreal(4), m_scale));
//   // emit scaleChanged((m_scale));
//   QGroupBox::resizeEvent(e);
// }

void ExamPreview::dockFloatablePreview() {}

void ExamPreview::showFieldGrid() {}

void ExamPreview::deletePage() {}

void ExamPreview::assignPage() {}

void ExamPreview::markExamSheetAsValidated() {}

void ExamPreview::changeScale(qreal scale)
{
  // whos scale ?
  gView->scale(scale, scale);
  update();
}

void ExamPreview::loadAnswerSheet(QGraphicsView &gv, QGraphicsScene &gs)
{
  // filename should be stored somewhere I can read it, probably passed to me by
  // tableview
  // load pixmap from filename
  // QString filename = xx.getfilename()
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

QRect ExamPreview::fitImageWidthInView(QGraphicsView &gv)
{
  QSize ps = singleImage->pixmap().size();
  QRect defaultViewSize(0, 0, ps.width(), gv.height());
  return defaultViewSize;
}

void ExamPreview::fitROIInView(QRect &) {}
