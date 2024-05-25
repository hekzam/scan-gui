#ifndef SINGLEPAGE_H
#define SINGLEPAGE_H

#include <QGraphicsPixmapItem>
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

  void toggleMarkerVisibility(bool state);
  void toggleAtomicBoxVisibility(bool state);

  QList<MarkerItem *> currentMarkerItems() const;
  QList<atomicBoxItem *> currentAtomicBoxItems() const;

  QRectF getFieldPos(QString fieldName);

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
  const mJSON::dataCopieJSON *m_JSONData = nullptr;
  int m_pageNumber; // page number is one-indexed ? I don't make the rules
  QList<MarkerItem *> m_currentMarkerItems;
  QList<atomicBoxItem *> m_currentAtomicBoxItems;

  QSize m_docSize;
  QSize m_imageSize;
  qreal m_widthScaling;
  qreal m_heightScaling;
  QTransform m_fieldTransformMatrix;
  bool MatrixIsInvertible;

  void addKnownMarkers();
  void addKnownAtomicBoxItems();
  void setFieldItemAttributes(FieldItem *fieldItem, mJSON::dataFieldJSON m);
};

#endif // SINGLEPAGE_H
