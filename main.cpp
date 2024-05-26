#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <qprogressbar.h>
#include <QString>

void showHelp() {
  std::cout << "Usage: scan-gui [options]\n"
            << "Options:\n"
            << "  --help       Show the list of options\n"
            << "  --version    Show the program version\n";
}

void showVersion() {
  std::cout << "scan-gui version 2.0.0\n"; // Replace with your actual version
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QStringList args = QApplication::arguments();

  if (args.contains("--help")) {
    showHelp();
    return 0;
  } else if (args.contains("--version")) {
    showVersion();
    return 0;
  }

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages)
  {
    const QString baseName = "scan-gui_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName))
    {
      a.installTranslator(&translator);
      break;
    }
  }
  qRegisterMetaType<CopyInfo *>("CopyInfo*");
  MainWindow w;
  w.show();

  return a.exec();
}
