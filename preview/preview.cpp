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
  auto previewButtonLayout = new QGridLayout(previewButtonBox);

  auto opacitySlider = new QSlider(Qt::Horizontal, previewButtonBox);
  opacitySlider->setMinimum(0);
  opacitySlider->setMaximum(255); // rgba value
  opacitySlider->setPageStep(25);
  opacitySlider->setSingleStep(5);
  opacitySlider->setValue(0);
  opacitySlider->setToolTip(tr("This slider controls the opacity of the mask"));
  auto resetOpacity = new QPushButton(tr("reset Opacity"), previewButtonBox);

  auto thresholdSlider = new QSlider(Qt::Horizontal, previewButtonBox);
  thresholdSlider->setMinimum(0);
  thresholdSlider->setMaximum(100);
  thresholdSlider->setPageStep(10);
  thresholdSlider->setSingleStep(1);
  thresholdSlider->setValue(50);
  thresholdSlider->setToolTip(
      tr("Ths slider controls the detection threshold for "
         "the greyed out fields of the page"));

  auto viewWholePageButton =
      new QPushButton(tr("Show the whole page"), previewButtonBox);
  auto deletePageButton =
      new QPushButton(tr("Delete the page"), previewButtonBox);
  auto assignPageButton =
      new QPushButton(tr("Assign this page"), previewButtonBox);
  auto validatePageButton =
      new QPushButton(tr("Mark as verified/validated"), previewButtonBox);
  // auto transformButton = new QPushButton(tr("transform"), previewButtonBox);
  auto highlightFieldsCheckBox =
      new QCheckBox(tr("HighlightFields"), previewButtonBox);
  auto calibrationModeSelectorCheckBox =
      new QCheckBox(tr("Calibration Mode"), previewButtonBox);

  highlightFieldsCheckBox->setChecked(true);
  calibrationModeSelectorCheckBox->setChecked(false);

  connect(opacitySlider, &QSlider::valueChanged, baseScene,
          &ExamScene::setMaskOpacityLevel);

  connect(resetOpacity, &QPushButton::clicked, opacitySlider,
          [opacitySlider] { opacitySlider->setValue(0); });
  // connect(baseViewport, &ExamViewPort::imageRotationChanged, opacitySlider,
  //         &QSlider::setValue);

  // TODO : Threshold connect

  connect(viewWholePageButton, &QPushButton::clicked, this,
          &ExamPreview::showExternalPreview);
  connect(deletePageButton, &QPushButton::clicked, this,
          &ExamPreview::deletePage);
  connect(assignPageButton, &QPushButton::clicked, this,
          &ExamPreview::assignPage);
  connect(validatePageButton, &QPushButton::clicked, this,
          &ExamPreview::markExamSheetAsValidated);

  // connect(transformButton....)
  connect(highlightFieldsCheckBox, &QCheckBox::clicked, baseScene,
          &ExamScene::toggleFieldsVisibility);
  connect(calibrationModeSelectorCheckBox, &QCheckBox::clicked, baseScene,
          &ExamScene::toggleCalibrationMode);

  previewButtonLayout->addWidget(opacitySlider, 0, 0, 1, 2);
  previewButtonLayout->addWidget(resetOpacity, 0, 2);
  previewButtonLayout->addWidget(thresholdSlider, 0, 3);
  previewButtonLayout->addWidget(viewWholePageButton, 1, 0);
  previewButtonLayout->addWidget(deletePageButton, 1, 1);
  previewButtonLayout->addWidget(assignPageButton, 1, 2);
  previewButtonLayout->addWidget(validatePageButton, 1, 3);
  // previewButtonLayout->addWidget(transformButton);
  previewButtonLayout->addWidget(highlightFieldsCheckBox, 1, 4);
  previewButtonLayout->addWidget(calibrationModeSelectorCheckBox, 1, 5);
}

void ExamPreview::createBasePreview()
{
  basePreview = new QFrame(previewStack);
  auto basePreviewLayout = new QVBoxLayout(basePreview);

  basePreviewLayout->setContentsMargins(0, 0, 0, 0);
  basePreview->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

  baseScene = new ExamScene(basePreview);
  baseViewport = new ExamViewPort(baseScene, basePreview);

  connect(baseScene, &ExamScene::newPageLoaded, baseViewport,
          &ExamViewPort::scaleToWidgetSize);

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

  gridScene = new ExamScene(gridPreview);
  gridViewport = new ExamViewPort(gridScene, gridPreview);

  // // *** TEST ***
  // QString ut3_testfn = ":/preview/resources/logo-UT3 modif.png";
  // /*gridViewport*/ gridScene->loadImage(ut3_testfn);
  // // *** END TEST ***

  // connect(gridScene, &ExamScene::rescale, gridViewport,
  //         &ExamViewPort::scaleToWidgetSize);

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

// TODO : ask for the page number too ?
// TODO : change groupbox, external view title based on file name ?
// void ExamPreview::onAction_CurrentTableElementChanged(
//     const QString &imagePath, const mJSON::dataCopieJSON &data, const int
//     col)
// {
//   baseScene->loadImage(imagePath, data, col);
// }
void ExamPreview::onAction_CurrentTableElementChanged(
    const QStringList &imagePath, mJSON::dataCopieJSON &data,
    const QString &fieldName)
{
  if (fieldName.isEmpty())
  {
    baseScene->loadImage(imagePath, data);
  }
  else
  {
    baseScene->loadImage(imagePath, data, fieldName);
  }
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
