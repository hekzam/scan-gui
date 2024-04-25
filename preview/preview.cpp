#include "preview.h"

using namespace mViewPort;

ExamPreview::ExamPreview(QWidget *parent)
    : /*previewBox = new*/ QGroupBox(tr("Preview"), parent),
      previewSizePolicy(QSizePolicy::MinimumExpanding,
                        QSizePolicy::MinimumExpanding)
{
  QVBoxLayout *previewLayout = new QVBoxLayout(this /*previewBox*/);
  setCheckable(false);
  setSizePolicy(previewSizePolicy);

  createPreviewStack();
  createPreviewButtonBox();

  previewLayout->addWidget(previewStack);
  previewLayout->addWidget(previewButtonBox);
}

QSize ExamPreview::sizeHint() const
{
  return QSize(640, 480);
}

ExamPreview::~ExamPreview(){}

void ExamPreview::createPreviewStack()
{
  previewStack = new QStackedWidget(this /*previewBox*/);
  // previewStack->setSizePolicy(previewSizePolicy);
  createBasePreview();
  createGridPreview();
  createDialogPreview();
  previewStack->setCurrentIndex(0);
  // previewStack->setMinimumSize(minPreviewSize);
  // qDebug() << "current previewstack widget :" <<
  // previewStack->currentWidget();
}

void ExamPreview::createPreviewButtonBox()
{
  previewButtonBox = new QGroupBox(this /*previewBox*/);
  auto previewButtonLayout = new QHBoxLayout(previewButtonBox);

  auto viewWholePageButton =
      new QPushButton(tr("Show the whole page"), previewButtonBox);
  auto deletePageButton =
      new QPushButton(tr("Delete the page"), previewButtonBox);
  auto assignPageButton =
      new QPushButton(tr("Assign this page"), previewButtonBox);
  auto validatePageButton =
      new QPushButton(tr("Mark as verified/validated"), previewButtonBox);

  connect(viewWholePageButton, &QPushButton::clicked, this,
          &ExamPreview::showExternalPreview);

  connect(deletePageButton, &QPushButton::clicked, this,
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

void ExamPreview::createBasePreview()
{
  basePreview = new QFrame(previewStack);
  auto basePreviewLayout = new QVBoxLayout(basePreview);

  basePreviewLayout->setContentsMargins(0, 0, 0, 0);
  basePreview->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  // basePreview->setMinimumSize(minPreviewSize);

  baseScene = new QGraphicsScene(basePreview);
  baseViewport = new ExamViewPort(baseScene, basePreview);

  basePreviewLayout->addWidget(baseViewport);
  previewStack->addWidget(basePreview);
}

// TODO : définir la grille
void ExamPreview::createGridPreview()
{
  auto gridPreview = new QFrame(previewStack);
  auto gridPreviewLayout = new QVBoxLayout(gridPreview);

  gridPreviewLayout->setContentsMargins(0, 0, 0, 0);
  gridPreview->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  // gridPreview->setMinimumSize(minPreviewSize);

  gridScene = new QGraphicsScene(gridPreview);
  gridViewport = new ExamViewPort(gridScene, gridPreview);

  // *** TEST ***
  QString ut3_testfn = ":/preview/resources/logo-UT3 modif.png";
  gridViewport->loadImage(ut3_testfn, ut3_testfn);
  // *** END TEST ***

  gridPreviewLayout->addWidget(gridViewport);
  previewStack->addWidget(gridPreview);
}

// TODO: do we need to save the window position, if so, how ?
// TODO: detect closeEvent to show the correct viewport in the correct place
void ExamPreview::createDialogPreview()
{
  // subclass ?
  floatableDialogPreview = new QDialog(this);
  floatableDialogPreview->setWindowTitle("Whole sheet Preview - scan-gui");
  floatableDialogPreview->setModal(false);
  auto dialogLayout = new QVBoxLayout(floatableDialogPreview);
}

void ExamPreview::setGroupBoxTitle() {}

void ExamPreview::nextImage() {}

void ExamPreview::previousImage() {}

// Ping-pong
void ExamPreview::showExternalPreview()
{
  if (floatableDialogPreview->isHidden())
  {
    floatableDialogPreview->layout()->addWidget(baseViewport);
    floatableDialogPreview->show();
  }
  else
  {
    basePreview->layout()->addWidget(baseViewport);
    floatableDialogPreview->hide();
  }
  previewStack->setCurrentIndex((previewStack->currentIndex() + 1) % 2);
}

void ExamPreview::deletePage()
{
}

void ExamPreview::assignPage()
{
}

void ExamPreview::markExamSheetAsValidated()
{
}
