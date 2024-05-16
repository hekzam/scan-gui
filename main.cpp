#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <qprogressbar.h>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = QApplication::arguments();

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

    return a.exec();
}
