#ifndef SINGLEPAGE_H
#define SINGLEPAGE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "../json/jsonreader.h"
#include "fields/atomicboxitem.h"
#include "fields/markeritem.h"

class ExamSinglePage : public QGraphicsPixmapItem
{
public:
  ExamSinglePage();
  ~ExamSinglePage();

  QPoint mapImageCoordToJSONCoord(QPoint imgPos);
  QPoint mapJSONCoordtoImageCoord(QPoint JSONPos);

  void setJSONData(const mJSON::dataCopieJSON *data);
  void setPageNum(int pageNum);
  void setImageSize(QSize imgSize);
  // void setDocSize(QSize docSize);

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
  const mJSON::dataCopieJSON *m_JSONData = nullptr;
  // one-indexed :-) I don't make the rules
  int m_pageNumber;
  QList<MarkerItem *> m_currentMarkerItems;
  QList<atomicBoxItem *> m_currentAtomicBoxItems;
  QSize m_docSize;
  QSize m_imageSize;
  qreal widthScaling;
  qreal heightScaling;
  QTransform fieldTransformMatrix;
  bool MatrixIsInvertible;

  void addKnownMarkers();
  void addKnownAtomicBoxItems();
  void setFieldItemAttributes(FieldItem *aBox, mJSON::coordinates m);
};

#endif // SINGLEPAGE_H
