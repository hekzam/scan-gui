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

class ExamPreview : public QGroupBox
{
  Q_OBJECT

public:
  explicit ExamPreview(QWidget *parent = nullptr);
  ~ExamPreview();

public slots:
  void setGroupBoxTitle();

private slots:
  // TODO, public ?
  void nextImage();
  void previousImage();

  void dockFloatablePreview();
  void showFieldGrid();
  void deletePage();
  void assignPage();
  void markExamSheetAsValidated();
  // void displayImage(QString filename);

private:
  void createPreviewStack();
  void createPreviewButtonBox();
  void createFixedPreview();
  void createFloatablePreview();

  QSizePolicy szPol;

  mViewPort::ExamViewPort *fixedView;
  mViewPort::ExamViewPort *floatView;
  QGraphicsScene *gScene;

  QGroupBox *previewBox;

  QStackedWidget *previewStack;
  QDockWidget *floatablePreview;
  QGroupBox *previewButtonBox;

  // show Grid
  // QGraphicsView *gridView;
  // QGraphicsScene *gridScene;
};
#endif // PREVIEWLAYOUT_H
