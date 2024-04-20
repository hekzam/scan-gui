#include "preview.h"

using namespace mViewPort;

ExamPreview::ExamPreview(QWidget *parent)
    : previewBox(new QGroupBox(tr("Preview"), parent)),
      szPol(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding)
{
  QVBoxLayout *previewLayout = new QVBoxLayout(previewBox);
  setCheckable(false);
  setSizePolicy(szPol);

  createPreviewStack();
  createPreviewButtonBox();

  previewLayout->addWidget(previewStack);
  previewLayout->addWidget(previewButtonBox);
}

ExamPreview::~ExamPreview(){}

void ExamPreview::createPreviewStack()
{
  previewStack = new QStackedWidget(previewBox);
  previewStack->setSizePolicy(szPol);
  createFixedPreview();
  createFloatablePreview();
  previewStack->setCurrentIndex(1);
  previewStack->setMinimumSize(minPreviewSize);
}

void ExamPreview::createPreviewButtonBox()
{
  previewButtonBox = new QGroupBox(previewBox);
  QHBoxLayout *previewButtonLayout = new QHBoxLayout(previewButtonBox);

  auto viewWholePageButton =
      new QPushButton(tr("Show the whole page"), previewButtonBox);
  auto deletePageButton =
      new QPushButton(tr("Delete the page"), previewButtonBox);
  auto assignPageButton =
      new QPushButton(tr("Assign this page"), previewButtonBox);
  auto validatePageButton =
      new QPushButton(tr("Mark as verified/validated"), previewButtonBox);

  // this doesn't work
  connect(viewWholePageButton, &QAbstractButton::clicked, this,
          &ExamPreview::dockFloatablePreview);

  connect(deletePageButton, &QAbstractButton::clicked, this,
          &ExamPreview::deletePage);

  connect(assignPageButton, &QPushButton::clicked, this,
          &ExamPreview::assignPage);

  connect(validatePageButton, &QPushButton::clicked, this,
          &ExamPreview::markExamSheetAsValidated);

  previewButtonLayout->addWidget(viewWholePageButton);
  previewButtonLayout->addWidget(deletePageButton);
  previewButtonLayout->addWidget(assignPageButton);
  previewButtonLayout->addWidget(validatePageButton);
}

// TODO : définir la grille
void ExamPreview::createFixedPreview()
{
  QFrame *fixedPreview = new QFrame(previewStack);
  QVBoxLayout *fixedPreviewLayout = new QVBoxLayout(fixedPreview);
  fixedPreview->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  fixedPreview->setMinimumSize(minPreviewSize);

  gScene = new QGraphicsScene(fixedPreview);
  fixedView = new ExamViewPort(gScene, fixedPreview);

  fixedPreviewLayout->addWidget(fixedView);
  previewStack->addWidget(fixedPreview);
}

void ExamPreview::createFloatablePreview()
{
  floatablePreview = new QDockWidget(previewStack, Qt::Widget);
  floatablePreview->setFloating(false);
  floatablePreview->setFeatures(QDockWidget::DockWidgetFloatable |
                                QDockWidget::DockWidgetMovable);

  floatView = new ExamViewPort(gScene, floatablePreview);
  floatablePreview->setWidget(floatView);
  // TODO
  previewStack->addWidget(floatablePreview);
  connect(floatablePreview, &QDockWidget::topLevelChanged, this,
          &ExamPreview::showFieldGrid);
}

void ExamPreview::setGroupBoxTitle() {}

void ExamPreview::nextImage() {}

void ExamPreview::previousImage() {}

void ExamPreview::dockFloatablePreview()
{
  qDebug() << "xd?";
  if (!floatablePreview->isFloating())
  {
    floatablePreview->setFloating(true);
    previewStack->setCurrentIndex(0);
  }
}

void ExamPreview::showFieldGrid()
{
  qDebug() << "xd?";
  previewStack->setCurrentIndex(0);
}

void ExamPreview::deletePage()
{
  qDebug() << "xd?";
}

void ExamPreview::assignPage()
{
  qDebug() << "xd?";
}

void ExamPreview::markExamSheetAsValidated()
{
  qDebug() << "xd?";
}
