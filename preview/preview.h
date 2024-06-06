/*
 * Layout of the preview module
 *
 * - QStackedWidget pour passer entre les 2 Vues
 *  - ExamViewPort * 2 : 2 sous-classes de QGraphicsView pour montrer les scènes
 *   - ExamScene * 2 : 2 sous-classes de QGraphicsScene pour montrer les objets
 *    - PageMask : Un masque plus ou moins transparent pour cacher les items
 *      indésirables. Il est possible de montrer plusieurs items à la fois et
 *      cacher le reste de la page
 *    - SinglePage : Une page d'une copie, on ne peut montrer qu'une page à la
 * fois, La page contient des champs(Fields).
 *
 *     - FieldItems : Les champs associés à chaque page, séparés en différentes
 *       sous-classes pour différencier les marqueurs, les Atomic Boxes et
 *       autres
 *       - AtomicBoxItems : boites dont la "valeur" est un booléen
 *       - MarkerItem : boite servant à calibrer la page, se place sur les QR
 *         codes aux 4 coins de la page
 *     - MarkerController/MarkerHandles : classes servant à repositionner les
 *       marqueurs en mode calibration
 *
 * - QButtonBox pour controler la vue (transparence du max, visibilité des
 *   champs ...)
 */

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
