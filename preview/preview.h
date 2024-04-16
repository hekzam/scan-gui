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
  // do I need Q_PROPERTY?

public:
  ExamPreview(QWidget *parent = nullptr);
  ~ExamPreview();

public slots:
  void setGroupBoxTitle(); // ??

private slots:
  // TODO
  // void nextImage();
  // void previousImage();
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

  mViewPort::ExamViewPort *gView;

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
};
#endif // PREVIEWLAYOUT_H
