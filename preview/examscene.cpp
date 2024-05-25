#include "examscene.h"
using namespace mJSON;

ExamScene::ExamScene(QObject *parent)
    : QGraphicsScene{parent},
      m_currentPageNum(1) // initialized here to avoid errors, one-indexed
{
  setBackgroundBrush(Qt::gray);
  m_singleImage = new ExamSinglePage();
  m_maskItem = new PageMask();
  addItem(m_singleImage);
  addItem(m_maskItem);
}

ExamScene::~ExamScene() {}

void ExamScene::loadImage(const QStringList &imgfilename, dataCopieJSON *data,
                          const int pageNumbertoDisplay,
                          const QString &fieldName)
{
  // reset
  m_currentCopyImageFilename = {""};
  m_numberOfPages = 0;
  m_currentPageNum = 1;
  m_jsonData = nullptr;
  m_focusedFieldName = "";

  m_currentCopyImageFilename = imgfilename;
  m_numberOfPages = imgfilename.size();
  m_currentPageNum = pageNumbertoDisplay;
  m_jsonData = data;
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
void ExamScene::loadAnswerSheet()
{
  QPixmap p;
  if (!p.load(m_currentCopyImageFilename[m_currentPageNum - 1]))
  {
    qWarning() << "error loading the image";
  }
  else // si on a une image
  {
    m_singleImage->setPixmap(p);
    m_singleImage->setImageSize(p.size());
    m_maskItem->setMaskSize(p.size());

    if (m_jsonData) // si on a un JSON lié...
    {
      m_singleImage->setPageNum(m_currentPageNum);
      m_singleImage->setJSONData(m_jsonData);

      toggleCalibrationMode(calibrationMode);
      toggleFieldsVisibility(fieldsAreVisible);

      if (!m_focusedFieldName.isEmpty()) // si on a cliqué sur un champ...
      {
        emit setROI(m_singleImage->getFieldPos(m_focusedFieldName));
        for (auto &a : m_singleImage->currentAtomicBoxItems())
        {
          if (a->getClef() == m_focusedFieldName) // on ne montre que ce champ
          {
            m_maskItem->addFieldToHighlight(a->polygon());
          }
          emit titleChanged(m_focusedFieldName); // change groupbox title here
        }
      }
      else // sinon on montre tout
      {
        for (auto &a : m_singleImage->currentAtomicBoxItems())
        {
          m_maskItem->addFieldToHighlight(a->polygon());
        }
        emit titleChanged(tr("currently viewing a page"));
      }
      emit newPageLoaded(p.size());
    }
    }
  }
}
