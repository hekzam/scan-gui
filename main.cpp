#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <qprogressbar.h>
#include <QString>
#include "sortTable/controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "scan-gui_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();

    /*QTableWidget *tw = new QTableWidget(2,2);
    QProgressBar *pg1 = new QProgressBar();
    QProgressBar *pg2 = new QProgressBar();
    pg1->setValue(25);
    pg2->setValue(76);
    tw->setCellWidget(0,0,pg1);
    tw->setCellWidget(1,0,pg2);
    tw->setItem(0,1,new QTableWidgetItem("hihihih"));
    tw->setItem(1,1,new QTableWidgetItem("lalala"));
    tw->setSortingEnabled(true);
    tw->show();*/

    return a.exec();
}
