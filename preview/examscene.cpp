#include "examscene.h"
using namespace mJSON;

ExamScene::ExamScene(QObject *parent)
    : QGraphicsScene{parent}, m_currentPageNum(1) //
{
  setBackgroundBrush(Qt::gray);
  m_singleImage = new ExamSinglePage();
  m_maskItem = new PageMask();
  addItem(m_singleImage);
  addItem(m_maskItem);
}

ExamScene::~ExamScene() {}

void ExamScene::loadImage(const QStringList &imgfilename, dataCopieJSON &data)
{
  m_currentCopyImageFilename = imgfilename;
  m_jsonData = &data;
  // m_currentPageNum = ??? // set the correct page number here if it exists
  loadAnswerSheet();
}

void ExamScene::loadImage(const QStringList &imgfilename, dataCopieJSON &data,
                          const QString &fieldName)
{
  m_currentCopyImageFilename = imgfilename;
  m_jsonData = &data;
  // TODO look for [field name] and zoom on it
  m_focusedFieldName = fieldName;
  loadAnswerSheet();
}

void ExamScene::loadImage(const QStringList &imgfilename)
{
  m_currentCopyImageFilename = imgfilename;
  loadAnswerSheet();
}

void ExamScene::toggleCalibrationMode(bool state)
{
  calibrationMode = state;
  m_singleImage->toggleMarkerVisibility(state);
  // TODO ? : enable drawing of polygons by drag and drop, exit draw mode on
  // mouse release ?
  // other things to toggle here (dragmode...)
}

void ExamScene::setMaskOpacityLevel(int value)
{
  m_maskItem->setBrush(QColor(255, 255, 255, value));
}

void ExamScene::toggleFieldsVisibility(bool state)
{
  fieldsAreVisible = state;
  m_singleImage->toggleAtomicBoxVisibility(state);
}

// throw some error here to let the preview window know?
void ExamScene::loadAnswerSheet() // TODO load first elem of the QStringList
{
  QPixmap p;
  if (!p.load(m_currentCopyImageFilename[0]))
  {
    qWarning() << "error loading the image";
  }
  else
  {
    // TODO : prendre num de page
    m_singleImage->setPixmap(p);
    m_singleImage->setImageSize(p.size());
    m_maskItem->setMaskSize(p.size());
  }
  if (m_dataVariant.toBool())
  {
  }
  if (m_jsonData)
  {
    m_singleImage->setPageNum(m_currentPageNum);
    m_singleImage->setJSONData(m_jsonData);
    toggleCalibrationMode(calibrationMode);
    toggleFieldsVisibility(fieldsAreVisible);
    for (auto &a : m_singleImage->currentAtomicBoxItems())
    {
      m_maskItem->addFieldToHighlight(a->polygon());
    }
    if (!m_focusedFieldName.isEmpty())
    {
      // TODO FOCUS ON THE FIELD HERE
    }
  }
  emit newPageLoaded(p.size()); // DO SOMETHING HERE
}
