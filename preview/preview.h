#ifndef PREVIEWLAYOUT_H
#define PREVIEWLAYOUT_H

#include <QDockWidget>
#include <QGraphicsView>
#include <QGroupBox>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QGraphicsPixmapItem>
#include "../json/jsonreader.h"
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
  void setGroupBoxTitle(QString title);
  void onAction_DialogClosedTriggered();
  void onAction_CurrentTableElementChanged(const QStringList &imagePaths,
                                           mJSON::dataCopieJSON *data,
                                           const int pageNumbertoDisplay,
                                           const QString &fieldName);

protected:
private slots:
  // TODO, public ?
  void nextImage();
  void previousImage();

  void showExternalPreview();
  void deletePage();
  void assignPage();
  void markExamSheetAsValidated();

private:
  void createPreviewStack();
  void createPreviewButtonBox();
  void createDialogPreview();
  void createBasePreview();
  void createGridPreview();

  QSizePolicy previewSizePolicy;

  mViewPort::ExamViewPort *baseViewport;
  mViewPort::ExamViewPort *gridViewport;
  ExamScene *baseScene;
  ExamScene *gridScene;

  QGroupBox *previewButtonBox;
  QStackedWidget *previewStack;
  QFrame *basePreview;
  externalPreviewDialog *externalPreview;
};
#endif // PREVIEWLAYOUT_H
