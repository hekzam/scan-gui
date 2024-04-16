#ifndef PREVIEWLAYOUT_H
#define PREVIEWLAYOUT_H

#include <QDockWidget>
#include <QGraphicsView>
#include <QGroupBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QGraphicsPixmapItem>
#include "examviewport.h"

class SheetViewPort : public QGraphicsView
{
public:
  SheetViewPort(QGraphicsScene *scene);
};

class ExamPreview : public QGroupBox
{
  Q_OBJECT
  // do I need Q_PROPERTY?

public:
  ExamPreview(QWidget *parent = nullptr);
  ~ExamPreview();

public slots:
  void setGroupBoxTitle(); // ??

protected:
  // TODO
  // void resizeEvent(QResizeEvent *e) override;
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *e) override;
#endif

private slots:
  // TODO
  // void nextImage();
  // void previousImage();
  void dockFloatablePreview();
  void showFieldGrid();
  void deletePage();
  void assignPage();
  void markExamSheetAsValidated();
  void changeScale(qreal scale);
  // void displayImage(QString filename);

signals:
  void scaleChanged(qreal scale);

private:
  void createPreviewStack();
  void createPreviewButtonBox();
  void createFixedPreview();
  void createFloatablePreview();

  void loadAnswerSheet(QGraphicsView &, QGraphicsScene &);
  QRect fitImageWidthInView(QGraphicsView &);
  void fitROIInView(QRect &);

  // une scene ou deux ?
  QGraphicsPixmapItem *singleImage;
  // QGraphicsScene *gScene;
  // mViewPort::ExamViewPort *gView;
  SheetViewPort *gView;

  QGroupBox *previewBox;
  // QVBoxLayout *previewLayout;
  QStackedWidget *previewStack;
  // QDockWidget *floatablePreview;
  // QFrame *fixedPreview; // not sure if QLabel or QWidget
  // QScrollArea *scrollArea;
  // show Grid
  // QGraphicsView *gridView;
  // QGraphicsScene *gridScene;
  QGroupBox *previewButtonBox;
  // QHBoxLayout *previewButtonLayout;

  // Buttons
  // QPushButton *deletePageButton;
  // QPushButton *assignPageButton;
  // QPushButton *validatePageButton;

  // Data
  qreal m_scale = 1.0;
};
#endif // PREVIEWLAYOUT_H
