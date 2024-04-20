#ifndef EXAMVIEWPORT_H
#define EXAMVIEWPORT_H

#include <QGraphicsView>
#include <QObject>
#include <QGraphicsPixmapItem>

namespace mViewPort
{
static const QSize minPreviewSize(600, 400);
static const qreal maxScalingFactor = 5.0;
static const qreal minScalingFactor = 0.2;

class ExamViewPort : public QGraphicsView
{
  Q_OBJECT
public:
  ExamViewPort(QGraphicsScene *gScene, QWidget *parent);
  ~ExamViewPort();

  void loadImage(QString &imgfilename, QString &jsonfilename);

protected:
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *e) override;
#endif
private slots:
  void changeScale(qreal scale);

signals:
  void scaleChanged(qreal scale);

private:
  void loadAnswerSheet(QGraphicsView &, QGraphicsScene &);
  void scaleToWidgetSize(QGraphicsView &, QGraphicsScene &);
  void fitROIInView(QRect &);

  // une scene ou deux ?
  QString currentImageFilename = "";
  QString currentJsonFilename = "";
  QGraphicsPixmapItem *singleImage;
  // QGraphicsScene *gScene;

  // Data
  qreal m_scale = 1.0;
  QRect ROI = QRect(0, 0, 0, 0); // region of interest
};
} // namespace mViewPort
#endif // EXAMVIEWPORT_H
