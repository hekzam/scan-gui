#include "externalpreviewdialog.h"

#include <QVBoxLayout>

externalPreviewDialog::externalPreviewDialog(QWidget *parent)
    : QDialog(parent), init(false)
{
  setWindowTitle("Whole sheet Preview - scan-gui");
  setModal(false);
  auto dialogLayout = new QVBoxLayout(this);
}

QSize externalPreviewDialog::minimumSizeHint() const
{
  return minDialogSize;
}

externalPreviewDialog::~externalPreviewDialog() {}

void externalPreviewDialog::closeEvent(QCloseEvent *e)
{
  savePosition();
  emit dialogClosed();
  QDialog::closeEvent(e);
}

void externalPreviewDialog::showEvent(QShowEvent *e)
{
  if (init)
  {
    move(m_savedPosition);
    resize(m_savedGeometry);
  }
  QDialog::showEvent(e);
}

void externalPreviewDialog::hideEvent(QHideEvent *e)
{
  savePosition();
  QDialog::hideEvent(e);
}

void externalPreviewDialog::savePosition()
{
  init = true;
  m_savedPosition = pos();
  m_savedGeometry = size();
}
