#ifndef EXAMVIEWPORT_H
#define EXAMVIEWPORT_H

#include <QGraphicsView>
#include <QObject>
#include <QGraphicsPixmapItem>

namespace mViewPort
{

class ExamViewPort : public QGraphicsView
{
  Q_OBJECT
public:
  ExamViewPort(QWidget *parent);
  ~ExamViewPort();

protected:
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *e) override;
#endif
private slots:
  void changeScale(qreal scale);
  // void displayImage(QString filename);

signals:
  void scaleChanged(qreal scale);

private:
  void loadAnswerSheet(QGraphicsView &, QGraphicsScene &);
  QRect fitImageWidthInView(QGraphicsView &);
  void fitROIInView(QRect &);

  // une scene ou deux ?
  QGraphicsPixmapItem *singleImage;
  // QGraphicsScene *gScene;

  // Data
  qreal m_scale = 1.0;
};
} // namespace mViewPort
#endif // EXAMVIEWPORT_H
