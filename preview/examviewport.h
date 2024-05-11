#ifndef EXAMVIEWPORT_H
#define EXAMVIEWPORT_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QWheelEvent>
#include "singlepage.h"

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

  void loadImage(const QString &imgfilename, const QString &jsonfilename);

protected:
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *e) override;
#endif

private slots:
  void changeScale(qreal scale);

signals:
  void scaleChanged(qreal scale);

private:
  void loadAnswerSheet();
  void scaleToWidgetSize();
  void fitROIInView(QRect &);

  // une scene ou deux ?
  QString m_currentImageFilename = "";
  QString m_currentJsonFilename = "";
  ExamSinglePage *m_singleImage;
  // QGraphicsScene *gScene;

  scaleToFit dimensionToConsider;

  // Data
  qreal m_scale = 1.0;
  QRect m_ROI = QRect(0, 0, 0, 0); // region of interest
};
} // namespace mViewPort
#endif // EXAMVIEWPORT_H
