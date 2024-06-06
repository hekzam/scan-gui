#include "userinformations.h"

UserInformations::UserInformations(const QString &title, QWidget *parent)
    : QGroupBox{title, parent}
{

  // QTableWidget *myTable = new QTableWidget(this);
  textZone = new QTextEdit(this);
  textZone->setReadOnly(true);
  textZone->resize(300, 200);
  textZone->setTextColor(Qt::red);
}

void UserInformations::setTextZone(QStringList &newText)
{
  // textZone->clear();
  for (const QString &text : newText)
  {
    textZone->append(text); // Ajouter chaque chaîne avec un retour à la ligne
  }
}

UserInformations::~UserInformations() {}
