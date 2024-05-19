#include "examscene.h"
using namespace mJSON;

ExamScene::ExamScene(QObject *parent)
    : QGraphicsScene{parent}, m_currentPageNum(1)
{
  setBackgroundBrush(Qt::gray);
  m_singleImage = new ExamSinglePage();
  m_mask = new PageMask();
  addItem(m_singleImage);
  addItem(m_mask);
}

ExamScene::~ExamScene() {}

void ExamScene::loadImage(const QString &imgfilename, const dataCopieJSON &data,
                          const int col)
{
  m_currentImageFilename = imgfilename;
  m_jsonData = &data;
  loadAnswerSheet();
}

void ExamScene::loadImage(const QString &imgfilename)
{
  m_currentImageFilename = imgfilename;
  loadAnswerSheet();
}

void ExamScene::toggleCalibrationMode(bool state)
{
  calibrationMode = state;
  // if (fieldsAreVisible && calibrationMode)
  // {
  //   toggleFieldsVisibility(false);
  // }
  m_singleImage->toggleMarkerVisibility(state);
  // TODO: enable drawing of polygons by drag and drop, exit draw mode on
  // mouse release ?
  // other things to toggle here (dragmode...)
}

void ExamScene::setMaskOpacityLevel(int value)
{
  m_mask->setBrush(QColor(255, 255, 255, value));
}

void ExamScene::toggleFieldsVisibility(bool state)
{
  fieldsAreVisible = state;
  m_singleImage->toggleAtomicBoxVisibility(state);
}

// throw some error here to let the preview window know?
void ExamScene::loadAnswerSheet()
{
  QPixmap p;
  if (!p.load(m_currentImageFilename))
  {
    qWarning() << "error loading the image";
  }
  else
  {
    m_singleImage->setPixmap(p);
    m_singleImage->setImageSize(p.size());
    m_mask->setMaskSize(p.size());
  }
  if (m_jsonData)
  {
    m_singleImage->setPageNum(m_currentPageNum);
    m_singleImage->setJSONData(m_jsonData);
    toggleCalibrationMode(calibrationMode);
    toggleFieldsVisibility(fieldsAreVisible);
  }
  emit newPageLoaded(p.size());
}
