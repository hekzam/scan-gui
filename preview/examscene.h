#ifndef EXAMSCENE_H
#define EXAMSCENE_H

#include "pagemask.h"
#include "singlepage.h"

#include <QGraphicsScene>

class ExamScene : public QGraphicsScene
{
  Q_OBJECT
public:
  explicit ExamScene(QObject *parent = nullptr);
  ~ExamScene();

  void loadImage(const QStringList &imgfilenames, mJSON::dataCopieJSON *data,
                 const int pageNumbertoDisplay, const QString &fieldName);
  void loadImage(const QStringList &imgfilename); // ???? one or multiple paths

signals:
  void newPageLoaded(QSize newImageSize);
  void setROI(QRectF ROI);
  void titleChanged(QString newTitle);

public slots:
  void toggleCalibrationMode(bool state);
  void setMaskOpacityLevel(int value);
  void toggleFieldsVisibility(bool state);
  void nextImage();
  void previousImage();

private:
  void loadAnswerSheet();
  bool m_initialized = false;
  QString m_currentImageFilename = "";
  QStringList m_currentCopyImageFilename;
  QString m_focusedFieldName = "";
  // this can't be const, I WANT to be able to modify this ? I'm not sure
  const mJSON::dataCopieJSON *m_jsonData = nullptr;
  int m_numberOfPages;
  int m_currentPageNum; // holds the num of the page being viewed (initialized
                        // to 1 in the constructor)
  ExamSinglePage *m_singleImage;
  PageMask *m_maskItem;

  // Drawing
  // enum ? for the mode ? edition, view and calibration
  bool calibrationMode = false;
  bool fieldsAreVisible = true;
};

#endif // EXAMSCENE_H
