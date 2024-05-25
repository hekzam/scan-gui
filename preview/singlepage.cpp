#include "singlepage.h"

// todo : add atomic boxes
ExamSinglePage::ExamSinglePage()
    : QGraphicsPixmapItem(), m_imageSize(0, 0), m_docSize(0, 0)
{
  setZValue(0.0);
}

ExamSinglePage::~ExamSinglePage() {}

QPoint ExamSinglePage::mapImageCoordToJSONCoord(QPoint imgPos)
{
  QPoint scaled(0, 0);
  if (m_JSONData)
  {
    QTransform invScaleMatrix =
        m_fieldTransformMatrix.inverted(&MatrixIsInvertible);
    scaled = invScaleMatrix.map(imgPos);
  }
  return scaled;
}

// return QPoint(0,0) if no JSON was provided
QPoint ExamSinglePage::mapJSONCoordtoImageCoord(QPoint JSONPos)
{
  QPoint scaled(0, 0);
  if (m_JSONData)
  {
    scaled = m_fieldTransformMatrix.map(JSONPos);
  }
  return scaled;
}

// return value needed ?
void ExamSinglePage::setJSONData(const mJSON::dataCopieJSON *data)
{
  m_JSONData = data;
  if (m_JSONData)
  {
    m_docSize = m_JSONData->documentSizes->at(m_pageNumber - 1).pS;
    m_fieldTransformMatrix.setMatrix(
        (qreal) m_imageSize.width() / m_docSize.width(), 0, 0, 0,
        (qreal) m_imageSize.height() / m_docSize.height(), 0, 0, 0, 1);
    // it should be invertible
    MatrixIsInvertible = m_fieldTransformMatrix.isInvertible();

    addKnownMarkers();
    addKnownAtomicBoxItems();
  }
  else
  {
    qWarning() << "No JSON data received !";
  }
}

void ExamSinglePage::setPageNum(int pageNum)
{
  m_pageNumber = pageNum;
}

void ExamSinglePage::setImageSize(QSize imgSize)
{
  m_imageSize = imgSize;
}

void ExamSinglePage::toggleMarkerVisibility(bool state)
{
  for (auto &m : m_currentMarkerItems) // MarkerItem*
  {
    m->setVisible(state);
  }
}

void ExamSinglePage::toggleAtomicBoxVisibility(bool state)
{
  for (auto &m : m_currentAtomicBoxItems) // AtomicBoxItem *
  {
    m->setVisible(state);
  }
}

void ExamSinglePage::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  // needed ?
  // event->accept();
  QPoint eventPoint = event->pos().toPoint();
  qDebug() << eventPoint;
  // qDebug() << mapImageCoordToJSONCoord(eventPoint);

  QGraphicsItem::mouseDoubleClickEvent(event);
}

QList<atomicBoxItem *> ExamSinglePage::currentAtomicBoxItems() const
{
  return m_currentAtomicBoxItems;
}

QRectF ExamSinglePage::getFieldPos(QString fieldName)
{
  for (auto &m : m_currentAtomicBoxItems) // AtomicBoxItem *
  {
    if (m->getClef() == fieldName)
      return m->getRect();
  }
  return QRectF(0, 0, 2, 2); // should never happen
}

QList<MarkerItem *> ExamSinglePage::currentMarkerItems() const
{
  return m_currentMarkerItems;
}

// I delete and create the list everytime, could be improved ?
// we already checked that we have JSONdata
void ExamSinglePage::addKnownMarkers()
{
  for (auto m : m_currentMarkerItems)
  {
    delete m;
  }
  m_currentMarkerItems.clear();

  for (auto &m : *m_JSONData->documentMarkers)
  {
    if (m.pagenum == m_pageNumber)
    {
      auto *marker = new MarkerItem(this);
      m_currentMarkerItems.append(marker);
      setFieldItemAttributes(marker, m);
      marker->setVisible(false); // markers are invisible by default
      marker->initCorners();
    }
  }
}

void ExamSinglePage::addKnownAtomicBoxItems()
{
  for (auto m : m_currentAtomicBoxItems)
  {
    delete m;
  }
  m_currentAtomicBoxItems.clear();

  for (auto &m : *m_JSONData->documentFields)
  {
    if (m.pagenum == m_pageNumber)
    {
      auto *aBox = new atomicBoxItem(this);
      m_currentAtomicBoxItems.append(aBox);
      setFieldItemAttributes(aBox, m);
    }
  }
}

// the test JSON files we had only had (x,y) and (width, height) attributes,
// meaning every field is supposed to be a square. But it may not be a perfectly
// aligned so we may need to rotate with transform.rotate

// For a better preview, it would be sensible to apply an offset of
// pen.width / 2 using get field.getPenWidth
void ExamSinglePage::setFieldItemAttributes(FieldItem *fieldItem,
                                            mJSON::coordinates m)
{
  QPointF img_xy = mapJSONCoordtoImageCoord(QPoint(m.x, m.y));
  // this is not real QPoint btw, it just hold the sizes :^)
  QPointF wid_hei = mapJSONCoordtoImageCoord(QPoint(m.w, m.h));

  auto rect = QRectF(img_xy.x(), img_xy.y(), wid_hei.x(), wid_hei.y());

  fieldItem->setClef(m.clef);
  fieldItem->setRect(rect);

  QList<QPointF> p;
  p << img_xy << QPointF(img_xy.x() + wid_hei.x(), (img_xy.y()))
    << QPointF(img_xy.x() + wid_hei.x(), img_xy.y() + wid_hei.y())
    << QPointF(img_xy.x(), img_xy.y() + wid_hei.y());
  auto polygon = QPolygonF(p);

  fieldItem->setPolygon(polygon);
}
