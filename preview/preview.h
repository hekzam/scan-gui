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
#include "externalpreviewdialog.h"

class ExamPreview : public QGroupBox
{
  Q_OBJECT

public:
  explicit ExamPreview(QWidget *parent = nullptr);
  virtual QSize sizeHint() const override;
  ~ExamPreview();

public slots:
  void setGroupBoxTitle();
  void on_actionDialogClosedTriggered();

protected:
  // ?
  // void closeEvent(QCloseEvent *) override;
  // bool eventFilter(QObject *, QEvent *) override; // ?
private slots:
  // TODO, public ?
  void nextImage();
  void previousImage();

  void showExternalPreview();
  void deletePage();
  void assignPage();
  void markExamSheetAsValidated();
  // void displayImage(QString filename);

private:
  void createPreviewStack();
  void createPreviewButtonBox();
  void createDialogPreview();
  void createBasePreview();
  void createGridPreview();

  QSizePolicy previewSizePolicy;

  mViewPort::ExamViewPort *baseViewport;
  mViewPort::ExamViewPort *gridViewport;
  QGraphicsScene *baseScene;
  QGraphicsScene *gridScene;

  // QGroupBox *previewBox;

  QGroupBox *previewButtonBox;
  QStackedWidget *previewStack;
  QFrame *basePreview;
  externalPreviewDialog *externalPreview;

  // show Grid
  // QGraphicsView *gridView;
  // QGraphicsScene *gridScene;
};
#endif // PREVIEWLAYOUT_H
