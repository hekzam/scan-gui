#ifndef USERINFORMATIONS_H
#define USERINFORMATIONS_H

#include <QGroupBox>
#include <QTextEdit>
#include <QVBoxLayout>

class UserInformations : public QGroupBox
{
  Q_OBJECT
public:
  explicit UserInformations(const QString &title, QWidget *parent = nullptr);

  QTextEdit *textZone;

  // void addErrors(QString &newErrors);

  ~UserInformations();

  void setTextZone(QStringList &newText);
};

#endif // USERINFORMATIONS_H
