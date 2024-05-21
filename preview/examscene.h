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

  // is the data really const ???
  void loadImage(const QString &imgfilename, const mJSON::dataCopieJSON &data,
                 const int col);
  void loadImage(const QString &imgfilename);

signals:
  void newPageLoaded(QSize newImageSize);

public slots:
  void toggleCalibrationMode(bool state);
  void setMaskOpacityLevel(int value);
  void toggleFieldsVisibility(bool state);

private:
  void loadAnswerSheet();

  QString m_currentImageFilename = "";
  const mJSON::dataCopieJSON *m_jsonData = nullptr;
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
