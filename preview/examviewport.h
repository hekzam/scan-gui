#ifndef EXAMVIEWPORT_H
#define EXAMVIEWPORT_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QWheelEvent>
#include "singlepage.h"
#include "../json/jsonreader.h"

namespace mViewPort
{
enum scaleToFit
{
  height,
  width,
};

static const QSize minPreviewSize(680, 600);
static const qreal maxScalingFactor = 5.0;
static const qreal minScalingFactor = 0.2;

class ExamViewPort : public QGraphicsView
{
  Q_OBJECT
public:
  ExamViewPort(QGraphicsScene *gScene, QWidget *parent);
  ~ExamViewPort();

  void scaleToOneOnOne();

  // is the data really const ???
  void loadImage(const QString &imgfilename, const mJSON::dataCopieJSON &data);
  void loadImage(const QString &imgfilename);

public slots:
  void changeDrawMode(bool state);
  void rotateImage(int value);
  void toggleFieldsVisibility(bool visible);

protected:
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *e) override;
#endif

  // TODO
  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;

private slots:
  void changeScale(qreal scale);

signals:
  void scaleChanged(qreal scale);
  void imageRotationChanged(int angle);

private:
  void loadAnswerSheet();
  void scaleToWidgetSize();
  void fitROIInView(QRect &);
  // TODO
  // QPoint mapJSONCoordonateToImage();
  // QPoint mapImageCoordonateToJSON();
  // void drawRectangle(x,y,w,h);
  // void removeRectangle(???); // by clicking on one
  // void removeAllrectangles // remove all children of single page
  // comment sauvegarder ? nom du pixmap

  // une scene ou deux ?
  QString m_currentImageFilename = "";
  QString m_currentJsonFilename = "";
  mJSON::dataCopieJSON m_jsonData;
  ExamSinglePage *m_singleImage;
  // QGraphicsScene *gScene;

  scaleToFit dimensionToConsider;

  // Data
  qreal m_scale = 1.0;
  qreal m_rotation = 0;
  QRect m_ROI = QRect(0, 0, 0, 0); // region of interest

  // Drawing
  bool drawingMode = false;
  QPen pen;
};
} // namespace mViewPort
#endif // EXAMVIEWPORT_H
