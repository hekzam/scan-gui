// settings.cpp
#include "settings.h"

Settings::Settings() : settings("UT3", "Hekzam")
{
  loadSettings();
}

void Settings::loadSettings()
{
  settings.beginGroup("MainWindow");
  int x = settings.value("windowPositionX")
              .toInt(); // Valeurs par défaut raisonnables
  int y = settings.value("windowPositionY").toInt() + 31;
  windowPosition = QPoint(x, y);

  int width =
      settings.value("windowWidth").toInt(); // Valeurs par défaut raisonnables
  int height = settings.value("windowHeight").toInt();
  windowSize = QSize(width, height);
  settings.endGroup();
}

void Settings::saveSettings()
{
  settings.beginGroup("MainWindow");
  settings.setValue("windowPositionX", windowPosition.x());
  settings.setValue("windowPositionY", windowPosition.y());
  settings.setValue("windowWidth", windowSize.width());
  settings.setValue("windowHeight", windowSize.height());
  settings.endGroup();
}

QPoint Settings::getWindowPosition() const
{
  return windowPosition;
}

void Settings::setWindowPosition(const QPoint &position)
{
  windowPosition = position;
}

QSize Settings::getWindowSize() const
{
  return windowSize;
}

void Settings::setWindowSize(const QSize &size)
{
  windowSize = size;
}
