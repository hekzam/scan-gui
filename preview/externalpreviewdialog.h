#ifndef EXTERNALPREVIEWDIALOG_H
#define EXTERNALPREVIEWDIALOG_H

#include <QDialog>

class externalPreviewDialog : public QDialog
{
  Q_OBJECT
public:
  externalPreviewDialog(QWidget *parent = nullptr);
  ~externalPreviewDialog();

protected:
  void closeEvent(QCloseEvent *e) override;
  void showEvent(QShowEvent *e) override;
  void hideEvent(QHideEvent *e) override;

signals:
  void dialogClosed();

private:
  void savePosition();
  QPoint m_savedPosition;
  QSize m_savedGeometry;
  bool init;
};

#endif // EXTERNALPREVIEWDIALOG_H
