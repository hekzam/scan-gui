/*
 * The Viewport can be associated with a scene to show the items visible on that
 * scene, by default items are visible and enabled. Here you can perform
 * operation that do not change the scene or the objects but how they are viewed
 * (namely zoom operations)
 */
#ifndef EXAMVIEWPORT_H
#define EXAMVIEWPORT_H

#include <QGraphicsView>
#include <QObject>
#include <QWheelEvent>
#include "examscene.h"

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
  ExamViewPort(ExamScene *gScene, QWidget *parent);
  ~ExamViewPort();

  void scaleToOneOnOne();

public slots:
  void rotateImage(int value);
  void onAction_PageLoadedRescaleToWidgetSize(QSize newImageSize);
  void fitROIInView(QRectF rect);

protected:
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *e) override;
#endif

  // TODO ?
  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;

private slots:
  void changeScale(qreal scale);

signals:
  void scaleChanged(qreal scale);
  void imageRotationChanged(int angle);

private:
  scaleToFit dimensionToConsider;

  // Data
  qreal m_scale = 1.0;
  qreal m_rotation = 0;
  QRect m_ROI = QRect(0, 0, 0, 0); // region of interest
};
} // namespace mViewPort
#endif // EXAMVIEWPORT_H
