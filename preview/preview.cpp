#include "preview.h"

using namespace mViewPort;

ExamPreview::ExamPreview(QWidget *parent)
    : /*previewBox = new*/ QGroupBox(tr("Preview"), parent),
      previewSizePolicy(QSizePolicy::MinimumExpanding,
                        QSizePolicy::MinimumExpanding)
{
  QVBoxLayout *previewLayout = new QVBoxLayout(this);
  setCheckable(false);
  setSizePolicy(previewSizePolicy);

  createPreviewStack();
  createPreviewButtonBox();

  previewLayout->addWidget(previewStack);
  previewLayout->addWidget(previewButtonBox);
}

QSize ExamPreview::sizeHint() const
{
  return minPreviewSize;
}

ExamPreview::~ExamPreview(){}

void ExamPreview::createPreviewStack()
{
  previewStack = new QStackedWidget(this);
  createBasePreview();
  createGridPreview();
  createDialogPreview();
  previewStack->setCurrentIndex(0);
}

void ExamPreview::createPreviewButtonBox()
{
  previewButtonBox = new QGroupBox(this);
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

  gridScene = new QGraphicsScene(gridPreview);
  gridViewport = new ExamViewPort(gridScene, gridPreview);

  // *** TEST ***
  QString ut3_testfn = ":/preview/resources/logo-UT3 modif.png";
  gridViewport->loadImage(ut3_testfn);
  // *** END TEST ***

  gridPreviewLayout->addWidget(gridViewport);
  previewStack->addWidget(gridPreview);
}

void ExamPreview::createDialogPreview()
{
  externalPreview = new externalPreviewDialog(this);
  connect(externalPreview, &externalPreviewDialog::dialogClosed, this,
          &ExamPreview::onAction_DialogClosedTriggered);
}

void ExamPreview::setGroupBoxTitle() {}

void ExamPreview::onAction_DialogClosedTriggered()
{
  showExternalPreview();
}

// TODO : change groupbox, external view title based on file name ?
void ExamPreview::onAction_CurrentTableElementChanged(
    const QString &imagePath, const mJSON::dataCopieJSON &data, const int)
{
  baseViewport->loadImage(imagePath, data);
}

void ExamPreview::nextImage() {}

void ExamPreview::previousImage() {}

// Ping-pong
void ExamPreview::showExternalPreview()
{
  if (externalPreview->isHidden())
  {
    externalPreview->layout()->addWidget(baseViewport);
    externalPreview->show();
  }
  else
  {
    basePreview->layout()->addWidget(baseViewport);
    externalPreview->hide();
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
