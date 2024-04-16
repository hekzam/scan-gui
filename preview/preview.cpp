#include "preview.h"

using namespace mViewPort;

static const qreal maxScalingFactor = 5.0;
static const QSize minPreviewSize(600, 500);

ExamPreview::ExamPreview(QWidget *parent)
{
  previewBox = new QGroupBox(tr("Preview"), parent);
  QVBoxLayout *previewLayout = new QVBoxLayout(previewBox);
  setCheckable(false);
  createPreviewButtonBox();
  createPreviewStack();

  previewLayout->addWidget(previewStack);
  previewLayout->addWidget(previewButtonBox);
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

  QGraphicsScene *gScene = new QGraphicsScene(fixedPreview);
  gView = new ExamViewPort(gScene, fixedPreview);

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
